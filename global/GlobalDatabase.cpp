#include "GlobalDatabase.h"

#include <QFile>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>

#include <QMessageBox>


GlobalDatabase* GlobalDatabase::m_globalDatabase = nullptr;

GlobalDatabase::GlobalDatabase()
{

}

GlobalDatabase::~GlobalDatabase()
{
}

QVector<Hill> GlobalDatabase::getGlobalHills() const
{
    return globalHills;
}

void GlobalDatabase::setGlobalHills(const QVector<Hill> &newGlobalHills)
{
    globalHills = newGlobalHills;
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
    globalJumpers.clear();
    globalHills.clear();

    if(loadJumpers() == false)
        return false;

    return true;
}

bool GlobalDatabase::writeToJson()
{
    if(writeJumpers() == false)
        return false;
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
    QByteArray bytes = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &error);
    if(error.error != QJsonParseError::NoError)
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Błąd przy wczytytywaniu zawodników", "Nie udało się wczytać zawodników z pliku userData/GlobalDatabase/globalJumpers.json\nTreść błędu: " + error.errorString(), QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    QJsonObject object = document.object();
    QJsonValue value = object.value("jumpers");
    QJsonArray array = value.toArray();

    for(const auto & val : array)
    {
        QJsonObject obj = val.toObject();
        Jumper jumper;
        jumper.setName(obj.value("name").toString());
        jumper.setSurname(obj.value("surname").toString());
        jumper.setCountryCode(obj.value("country-code").toString());
        jumper.getJumperSkillsPointer()->setTakeoffPower(obj.value("takeoff-power").toDouble());
        jumper.getJumperSkillsPointer()->setTakeoffTechnique(obj.value("takeoff-technique").toDouble());
        jumper.getJumperSkillsPointer()->setFlightTechnique(obj.value("flight-technique").toDouble());
        jumper.getJumperSkillsPointer()->setFlightStyle(obj.value("flight-style").toDouble());
        jumper.getJumperSkillsPointer()->setLandingStyle(obj.value("landing-style").toDouble());
        jumper.getJumperSkillsPointer()->setForm(obj.value("form").toDouble());

        QJsonArray characteristicsArray = obj.value("characteristics").toArray();
        for(const auto & val : characteristicsArray){
            jumper.getJumperSkillsPointer()->insertCharacteristic(val.toObject().value("type").toString(), val.toObject().value("level").toDouble());
        }

        globalJumpers.push_back(jumper);
    }

    for(const auto & jp : globalJumpers)
    {
        qDebug()<<jp.getNameAndSurname()<<" ("<<jp.getCountryCode()<<")";
        JumperSkills * jps = jp.getJumperSkillsPointer();
        qDebug()<<jps->getTakeoffPower()<<", "<<jps->getTakeoffTechnique()<<", "<<jps->getFlightTechnique()<<", "<<jps->getFlightStyle()<<", "<<jps->getLandingStyle()<<", "<<jps->getForm();
        for(const auto & ch : jps->getCharacteristics())
            qDebug()<<ch.getType()<<", "<<ch.getLevel();
        qDebug()<<"\n\n";
    }

    return true;
}

bool GlobalDatabase::writeJumpers()
{
    QJsonDocument document;
    QJsonObject mainObject;
    QJsonArray array;
    for(const auto & jumper : getGlobalJumpers())
    {
        QJsonObject object;
        object.insert("name", jumper.getName());
        object.insert("surname", jumper.getSurname());
        object.insert("country-code", jumper.getCountryCode());
        object.insert("takeoff-power", jumper.getJumperSkills().getTakeoffPower());
        object.insert("takeoff-technique", jumper.getJumperSkills().getTakeoffTechnique());
        object.insert("flight-technique", jumper.getJumperSkills().getFlightTechnique());
        object.insert("flight-style", jumper.getJumperSkills().getFlightStyle());
        object.insert("landing-style", jumper.getJumperSkills().getLandingStyle());
        object.insert("form", jumper.getJumperSkills().getForm());

        QJsonArray characteristicsArray;
        for(const auto & characteristic : jumper.getJumperSkills().getCharacteristics())
        {
            QJsonObject characteristicObject;
            characteristicObject.insert("type", characteristic.getType());
            characteristicObject.insert("level", characteristic.getLevel());
            characteristicsArray.push_back(characteristicObject);
        }
        object.insert("characteristics", characteristicsArray);

        array.push_back(QJsonValue(object));
    }
    mainObject.insert("jumpers", QJsonValue(array));
    document.setObject(mainObject);

    QFile file("userData/GlobalDatabase/globalJumpers.json");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zawodnikami", "Nie udało się otworzyć pliku userData/GlobalDatabase/globalJumpers.json\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    qDebug()<<document.toJson(QJsonDocument::Indented);

    return true;
}


