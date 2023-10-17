#include "GlobalDatabase.h"

#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QCoreApplication>
#include <QJsonValue>
#include <QSysInfo>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>
#include <QProgressDialog>

#include "../global/CountryFlagsManager.h"

GlobalDatabase* GlobalDatabase::m_globalDatabase = nullptr;
extern IDGenerator globalIDGenerator;

GlobalDatabase::GlobalDatabase()
{
    loadedSimulationSaves = false;
}

GlobalDatabase::~GlobalDatabase()
{
    for(auto & save : globalSimulationSaves)
        delete save;
    for(auto & calendarPreset : globalCalendarPresets){
        for(auto & comp : calendarPreset.getCalendarReference().getCompetitionsReference())
            delete comp;
        for(auto & cls : calendarPreset.getCalendarReference().getClassificationsReference())
            delete cls;
    }
    delete m_globalDatabase;
}

QVector<QPair<QString, QString> > GlobalDatabase::getCountries() const
{
    return countries;
}

QVector<QPair<QString, QString> > &GlobalDatabase::getCountriesReference()
{
    return countries;
}

void GlobalDatabase::setCountries(const QVector<QPair<QString, QString> > &newCountries)
{
    countries = newCountries;
}

QVector<JumpsImportancePreset> GlobalDatabase::getJumpsImportancePresets() const
{
    return jumpsImportancePresets;
}

QVector<JumpsImportancePreset> &GlobalDatabase::getJumpsImportancePresetsReference()
{
    return jumpsImportancePresets;
}

void GlobalDatabase::setJumpsImportancePresets(const QVector<JumpsImportancePreset> &newJumpsImportancePresets)
{
    jumpsImportancePresets = newJumpsImportancePresets;
}

QVector<JumperFormGeneratorSettingsPreset> GlobalDatabase::getFormGeneratorPresets() const
{
    return formGeneratorPresets;
}

QVector<JumperFormGeneratorSettingsPreset> &GlobalDatabase::getEditableFormGeneratorPresets()
{
    return formGeneratorPresets;
}

void GlobalDatabase::setFormGeneratorPresets(const QVector<JumperFormGeneratorSettingsPreset> &newFormGeneratorPresets)
{
    formGeneratorPresets = newFormGeneratorPresets;
}

bool GlobalDatabase::getLoadedSimulationSaves() const
{
    return loadedSimulationSaves;
}

void GlobalDatabase::setLoadedSimulationSaves(bool newLoadedSimulationSaves)
{
    loadedSimulationSaves = newLoadedSimulationSaves;
}

QVector<SeasonCalendarPreset> GlobalDatabase::getGlobalCalendarPresets() const
{
    return globalCalendarPresets;
}

QVector<SeasonCalendarPreset> &GlobalDatabase::getEditableGlobalCalendarPresets()
{
    return globalCalendarPresets;
}

void GlobalDatabase::setGlobalCalendarPresets(const QVector<SeasonCalendarPreset> &newGlobalCalendarPresets)
{
    globalCalendarPresets = newGlobalCalendarPresets;
}

QVector<PointsForPlacesPreset> GlobalDatabase::getGlobalPointsForPlacesPresets() const
{
    return globalPointsForPlacesPresets;
}

QVector<PointsForPlacesPreset> &GlobalDatabase::getEditableGlobalPointsForPlacesPresets()
{
    return globalPointsForPlacesPresets;
}

void GlobalDatabase::setGlobalPointsForPlacesPresets(const QVector<PointsForPlacesPreset> &newGlobalPointsForPlacesPresets)
{
    globalPointsForPlacesPresets = newGlobalPointsForPlacesPresets;
}

QVector<SimulationSave *> GlobalDatabase::getGlobalSimulationSaves() const
{
    return globalSimulationSaves;
}

QVector<SimulationSave *> &GlobalDatabase::getEditableGlobalSimulationSaves()
{
    return globalSimulationSaves;
}

void GlobalDatabase::setGlobalSimulationSaves(const QVector<SimulationSave *> &newGlobalSimulationSaves)
{
    globalSimulationSaves = newGlobalSimulationSaves;
}

QVector<CompetitionRules> GlobalDatabase::getGlobalCompetitionsRules() const
{
    return globalCompetitionsRules;
}

void GlobalDatabase::setGlobalCompetitionsRules(const QVector<CompetitionRules> &newGlobalCompetitionsRules)
{
    globalCompetitionsRules = newGlobalCompetitionsRules;
}

QVector<CompetitionRules> &GlobalDatabase::getEditableCompetitionRules()
{
    return globalCompetitionsRules;
}

QVector<Hill> GlobalDatabase::getGlobalHills() const
{
    return globalHills;
}

void GlobalDatabase::setGlobalHills(const QVector<Hill> &newGlobalHills)
{
    globalHills = newGlobalHills;
}

QVector<Hill> &GlobalDatabase::getEditableGlobalHills()
{
    return globalHills;
}

QVector<Jumper> GlobalDatabase::getGlobalJumpers() const
{
    return globalJumpers;
}

void GlobalDatabase::setGlobalJumpers(const QVector<Jumper> &newGlobalJumpers)
{
    globalJumpers = newGlobalJumpers;
}

QVector<Jumper> &GlobalDatabase::getEditableGlobalJumpers()
{
    return globalJumpers;
}

GlobalDatabase *GlobalDatabase::get()
{
    if(m_globalDatabase == nullptr)
        m_globalDatabase = new GlobalDatabase();
    return m_globalDatabase;
}

void GlobalDatabase::removeJumper(int index)
{
    globalJumpers.remove(index);
}

bool GlobalDatabase::loadFromJson()
{
    return (loadJumpers() && loadHills() && loadCompetitionsRules() && loadPointsForPlacesPresets() && loadCalendarPresets() && loadFormGeneratorPresets() && loadJumpsImportancePresets() && loadCountries());
}

bool GlobalDatabase::writeToJson()
{
    return (writeJumpers() && writeHills() && writeCompetitionsRules() && writeSimulationSaves() && writePointsForPlacesPresets() && writeCalendarPresets() && writeFormGeneratorPresets() && writeJumpsImportancePresets() && writeCountries());
}

bool GlobalDatabase::loadJumpers()
{
    QFile file("userData/GlobalDatabase/globalJumpers.json");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zawodnikami", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalJumpers.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    /*QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject obj = doc.object();
    QJsonArray jumpersArray = obj.value("jumpers");*/
    globalJumpers = Jumper::getVectorFromJson(file.readAll());
    file.close();
    return true;
}

bool GlobalDatabase::loadHills()
{
    QFile file("userData/GlobalDatabase/globalHills.json");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku ze skoczniami", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalHills.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    globalHills = Hill::getVectorFromJson(file.readAll());
    file.close();
    return true;
}

bool GlobalDatabase::loadCompetitionsRules()
{
    QFile file("userData/GlobalDatabase/globalCompetitionsRules.json");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zasadami konkursów", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalCompetitionsRules.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    globalCompetitionsRules = CompetitionRules::getCompetitionRulesVectorFromJson(file.readAll());
    file.close();
    return true;
}

bool GlobalDatabase::loadSimulationSaves(bool progressDialog)
{
    globalSimulationSaves.clear();

    QDir dir(QCoreApplication::applicationDirPath());
    if(dir.exists("userData") == false && QSysInfo::productType() == "windows")
        dir.cdUp();
    dir.setPath(dir.path() + "/simulationSaves");
    dir.setNameFilters(QStringList() << "*.json");

    QStringList fileNames = dir.entryList();

    QProgressDialog dialog;
    dialog.setStyleSheet("QProgressDialog{background-color: white; color: black;}");
    dialog.setMinimum(0);
    dialog.setMaximum(fileNames.count() * 5);
    dialog.setMinimumDuration(0);
    dialog.setValue(0);
    dialog.setWindowTitle(QObject::tr("Wczytywanie zapisów symulacji"));
    //dialog.setLabelText(QString(QObject::tr("Postęp wczytywania zapisów symulacji: %1 z %2")).arg(QString::number(dialog.value())).arg(QString::number(dialog.maximum())));
    dialog.setModal(true);
    dialog.setWindowModality(Qt::WindowModal);

    bool ok = true;

    globalIDGenerator.setFreezed(true);
    QFuture<SimulationSave *> future = QtConcurrent::mapped(fileNames, SimulationSave::loadFromFile);

    // Czekamy na zakończenie zadania i pobieramy wyniki do listy persons
    future.waitForFinished();
    globalSimulationSaves = future.results();
    globalIDGenerator.setFreezed(false);
    return ok;
}

bool GlobalDatabase::loadPointsForPlacesPresets()
{
    globalPointsForPlacesPresets.clear();

    QDir dir(QCoreApplication::applicationDirPath());
    if(dir.exists("userData") == false && QSysInfo::productType() == "windows")
        dir.cdUp();
    dir.setPath(dir.path() + "/userData/GlobalDatabase/pointsForPlacesPresets");
    dir.setNameFilters(QStringList() << "*.json");
    QStringList fileNames = dir.entryList();

    bool ok = true;

    globalIDGenerator.setFreezed(true);
    QFuture<PointsForPlacesPreset> future = QtConcurrent::mapped(fileNames, PointsForPlacesPreset::loadFromJson);

    future.waitForFinished();
    globalPointsForPlacesPresets = future.results();
    globalIDGenerator.setFreezed(false);
    return ok;
}

bool GlobalDatabase::loadCalendarPresets()
{
    QDir dir(QCoreApplication::applicationDirPath());
    if(dir.exists("userData") == false && QSysInfo::productType() == "windows")
        dir.cdUp();
    dir.setPath(dir.path() + "/userData/GlobalDatabase/calendarPresets");
    dir.setNameFilters(QStringList() << "*.json");

    QStringList fileNames = dir.entryList();

    bool ok = true;
    for(auto & fileName : fileNames){
        QFile file("userData/GlobalDatabase/calendarPresets/" + fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zapisem symulacji", "Nie udało się otworzyć pliku simulationSaves/" + fileName +"\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
            message.setModal(true);
            message.exec();
            ok = false;
        }
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject object = doc.object().value("calendar-preset").toObject();

        DatabaseObjectsManager * manager = new DatabaseObjectsManager();
        SeasonCalendarPreset p = SeasonCalendarPreset::getFromJson(object, manager);
        delete manager;
        globalCalendarPresets.push_back(p);
        file.close();
    }
    return ok;
}

bool GlobalDatabase::loadFormGeneratorPresets()
{
    QDir dir(QCoreApplication::applicationDirPath());
    if(dir.exists("userData") == false && QSysInfo::productType() == "windows")
        dir.cdUp();
    dir.setPath(dir.path() + "/userData/GlobalDatabase/formGeneratorPresets");
    dir.setNameFilters(QStringList() << "*.json");

    QStringList fileNames = dir.entryList();

    bool ok = true;
    for(auto & fileName : fileNames){
        QFile file("userData/GlobalDatabase/formGeneratorPresets/" + fileName);
        if(!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z presetem generatora formy", "Nie udało się otworzyć pliku userData/GlobalDatabase/formGeneratorPresets/" + fileName +"\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
            message.setModal(true);
            message.exec();
            ok = false;
        }
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject object = doc.object().value("jumper-form-generator-settings-preset").toObject();
        JumperFormGeneratorSettingsPreset p = JumperFormGeneratorSettingsPreset::getFromJson(object);
        formGeneratorPresets.push_back(p);
        file.close();
    }
    return ok;
}

bool GlobalDatabase::loadJumpsImportancePresets()
{
    QDir dir(QCoreApplication::applicationDirPath());
    if(dir.exists("userData") == false && QSysInfo::productType() == "windows")
        dir.cdUp();
    dir.setPath(dir.path() + "/userData/GlobalDatabase/jumpsImportancePresets");
    dir.setNameFilters(QStringList() << "*.json");

    QStringList fileNames = dir.entryList();

    bool ok = true;
    for(auto & fileName : fileNames){
        QFile file("userData/GlobalDatabase/jumpsImportancePresets/" + fileName);
        file.open(QFile::ReadOnly | QFile::Text);
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject object = doc.object();
        JumpsImportancePreset p = JumpsImportancePreset::getFromJson(object);
        jumpsImportancePresets.push_back(p);
        file.close();
    }
    return ok;
}

bool GlobalDatabase::loadCountries()
{
    QFile file("userData/GlobalDatabase/globalCountries.json");
    file.open(QFile::ReadOnly | QFile::Text);
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject object = doc.object();
    QJsonArray array = object.value("countries").toArray();
    for(auto val : array)
    {
        QPair<QString, QString> c;
        c.first = val.toObject().value("code").toString();
        c.second = val.toObject().value("name").toString();
        countries.push_back(c);
    }
    file.close();
    return true;
}

bool GlobalDatabase::writeJumpers()
{
    QJsonDocument document;
    QJsonObject mainObject;
    QJsonArray array;
    for(auto & jumper : getGlobalJumpers())
    {
        array.push_back(QJsonValue(Jumper::getJsonObject(jumper)));
    }
    mainObject.insert("jumpers", QJsonValue(array));
    document.setObject(mainObject);

    QFile file("userData/GlobalDatabase/globalJumpers.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zawodnikami", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalJumpers.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));

    file.close();
    return true;
}

bool GlobalDatabase::writeHills()
{
    QJsonDocument document;
    QJsonObject mainObject;
    QJsonArray array;
    for(auto & hill : getGlobalHills())
    {
        array.push_back(QJsonValue(Hill::getJsonObject(hill)));
    }
    mainObject.insert("hills", array);
    document.setObject(mainObject);

    QFile file("userData/GlobalDatabase/globalHills.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku ze skoczniami", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalHills.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool GlobalDatabase::writeCompetitionsRules()
{
    QJsonDocument document;
    QJsonObject mainObject;
    QJsonArray array;

    for(auto & rules : getGlobalCompetitionsRules())
    {
        array.push_back(QJsonValue(CompetitionRules::getJsonObject(rules)));
    }
    mainObject.insert("competitionsRules", array);
    document.setObject(mainObject);

    QFile file("userData/GlobalDatabase/globalCompetitionsRules.json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zasadami konkursów", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalCompetitionsRules.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool GlobalDatabase::writeSimulationSaves()
{
    bool ok = true;
    for(auto & save : globalSimulationSaves)
    {
        if(save->saveToFile("simulationSaves/") == false)
            ok = false;
    }
    return ok;
}

bool GlobalDatabase::writePointsForPlacesPresets()
{
    bool ok = true;
    for(auto & preset : globalPointsForPlacesPresets)
    {
        if(preset.saveToFile("userData/GlobalDatabase/pointsForPlacesPresets/") == false)
            ok = false;
    }
    return ok;
}

bool GlobalDatabase::writeCalendarPresets()
{
    bool ok = true;
    for(auto & preset : globalCalendarPresets)
    {
        if(preset.saveToFile("userData/GlobalDatabase/calendarPresets/") == false)
            ok = false;
    }
    return ok;
}

bool GlobalDatabase::writeFormGeneratorPresets()
{
    bool ok = true;
    for(auto & preset : formGeneratorPresets)
    {
        if(preset.saveToFile("userData/GlobalDatabase/formGeneratorPresets/") == false)
            ok = false;
    }
    return ok;
}

bool GlobalDatabase::writeJumpsImportancePresets()
{
    bool ok = true;
    for(auto & preset : jumpsImportancePresets)
    {
        if(preset.saveToFile("userData/GlobalDatabase/jumpsImportancePresets/") == false)
            ok = false;
    }
    return ok;
}

bool GlobalDatabase::writeCountries()
{
    QJsonDocument document;
    QJsonObject mainObject;
    QJsonArray array;
    for(auto & country : countries)
    {
        QJsonObject countryObject;
        countryObject.insert("code", country.first);
        countryObject.insert("name", country.second);
        array.push_back(countryObject);
    }
    mainObject.insert("countries", array);
    document.setObject(mainObject);
    QFile file("userData/GlobalDatabase/globalCountries.json");
    file.open(QFile::WriteOnly | QFile::Text);
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

void GlobalDatabase::setupJumpersFlags()
{
    for(auto & jumper : getEditableGlobalJumpers())
    {
        jumper.setFlagPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumper.getCountryCode().toLower())));
    }
}

QString GlobalDatabase::getCountryName(QString countryCode)
{
    for(auto & c : countries)
    {
        if(c.first == countryCode)
            return c.second;
    }
    return "COUNTRY-ERROR";
}
