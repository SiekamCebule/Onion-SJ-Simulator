#include "SimulationSave.h"
#include "../global/SeasonDatabaseObjectsManager.h"
#include <QFile>

extern SeasonDatabaseObjectsManager seasonObjectsManager;

SimulationSave::SimulationSave() :
    ClassWithID()
{
    actualSeason = nullptr;
}

SimulationSave::~SimulationSave()
{
    for(auto & hill : hills)
        delete hill;
    for(auto & jumper : jumpers)
        delete jumper;
    for(auto & season : seasons)
    {
        for(auto & competition : season.getCalendarReference().getCompetitionsReference())
            delete competition;
        for(auto & classification : season.getCalendarReference().getClassificationsReference())
            delete classification;
    }
}

SimulationSave * SimulationSave::getFromJson(QJsonObject obj)
{
    SeasonDatabaseObjectsManager * sdom = &seasonObjectsManager;
    SimulationSave * save = new SimulationSave();
    save->setID(obj.value("id").toString().toULong());
    save->setName(obj.value("name").toString());

    QJsonArray jumpersArray = obj.value("jumpers").toArray();
    for(auto val : jumpersArray){
        //qDebug()<<val;
        Jumper * jumper = new Jumper(Jumper::getFromJson(val.toObject()));
        save->getJumpersReference().push_back(jumper);
    }
    seasonObjectsManager.fill(&save->getJumpersReference());

    QJsonArray hillsArray = obj.value("hills").toArray();
    for(auto val : hillsArray){
        Hill * hill = new Hill(Hill::getFromJson(val.toObject()));
        save->getHillsReference().push_back(hill);
    }
    seasonObjectsManager.fill(&save->getHillsReference());

    QJsonArray rulesArray = obj.value("rules").toArray();
    for(auto val : rulesArray){
        CompetitionRules rules = CompetitionRules::getFromJson(val.toObject());
        save->getCompetitionRulesReference().push_back(rules);
    }
    seasonObjectsManager.fill(&save->getCompetitionRulesReference());

    QJsonArray seasonsArray = obj.value("seasons").toArray();
    for(auto val : seasonsArray){
        Season season = Season::getFromJson(val.toObject());
        save->getSeasonsReference().push_back(season);
    }
    seasonObjectsManager.fill(&save->getSeasonsReference());

    save->setActualSeason(static_cast<Season *>(seasonObjectsManager.getObjectByID(obj.value("actual-season-id").toString().toULong())));
    save->setNextCompetitionIndex(obj.value("next-competition-index").toInt());
    save->setNextCompetition(save->getActualSeason()->getCalendarReference().getCompetitionsReference()[save->getNextCompetitionIndex()]);

    return save;
}

QJsonObject SimulationSave::getJsonObject(SimulationSave &save)
{
    QJsonObject object;
    object.insert("id", QString::number(save.getID()));
    object.insert("name", save.getName());

    QJsonArray jumpersArray;
    for(auto & jumper : save.getJumpersReference()){
        jumpersArray.push_back(Jumper::getJsonObject(*jumper));
    }
    object.insert("jumpers", jumpersArray);

    QJsonArray hillsArray;
    for(auto & hill : save.getHillsReference()){
        hillsArray.push_back(Hill::getJsonObject(*hill));
    }
    object.insert("hills", hillsArray);

    QJsonArray rulesArray;
    for(auto & rules : save.getCompetitionRulesReference()){
        rulesArray.push_back(CompetitionRules::getJsonObject(rules));
    }
    object.insert("rules", rulesArray);

    QJsonArray seasonsArray;
    for(auto & season : save.getSeasonsReference()){
        seasonsArray.push_back(Season::getJsonObject(season));
    }
    object.insert("seasons", seasonsArray);

    object.insert("actual-season-id", QString::number(save.getActualSeason()->getID()));
    object.insert("next-competition-index", save.getNextCompetitionIndex());

    return object;
}

bool SimulationSave::saveToFile(QString dir)
{
    QJsonDocument document;
    QJsonObject mainObject;
    mainObject.insert("simulation-save", SimulationSave::getJsonObject(*this));
    document.setObject(mainObject);

    QFile file(dir + getName() + ".json");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox message(QMessageBox::Icon::Critical, "Nie można otworzyć pliku z zapisem symulacji " + getName(), "Nie udało się otworzyć pliku " + dir + getName() + ".json" + "\nUpewnij się, że istnieje tam taki plik lub ma on odpowiednie uprawnienia",  QMessageBox::StandardButton::Ok);
        message.setModal(true);
        message.exec();
        return false;
    }
    file.resize(0);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

void SimulationSave::updateNextCompetitionIndex()
{
    nextCompetitionIndex = 0;
    for(auto & comp : getActualSeason()->getCalendarReference().getCompetitionsReference())
    {
        if(comp->getPlayed() == false)
            break;
        nextCompetitionIndex++;
    }
    nextCompetition = getActualSeason()->getCalendarReference().getCompetitionsReference()[nextCompetitionIndex];
}

CompetitionInfo *SimulationSave::getNextCompetition() const
{
    return nextCompetition;
}

void SimulationSave::setNextCompetition(CompetitionInfo *newNextCompetition)
{
    nextCompetition = newNextCompetition;
}

int SimulationSave::getNextCompetitionIndex() const
{
    return nextCompetitionIndex;
}

void SimulationSave::setNextCompetitionIndex(int newNextCompetitionIndex)
{
    nextCompetitionIndex = newNextCompetitionIndex;
}

Season *SimulationSave::getActualSeason() const
{
    return actualSeason;
}

void SimulationSave::setActualSeason(Season *newActualSeason)
{
    actualSeason = newActualSeason;
}

void SimulationSave::setCompetitionRules(const QVector<CompetitionRules> &newCompetitionRules)
{
    competitionRules = newCompetitionRules;
}

QVector<Hill *> SimulationSave::getHills() const
{
    return hills;
}

QVector<CompetitionRules> &SimulationSave::getCompetitionRulesReference()
{
    return competitionRules;
}

void SimulationSave::setHills(const QVector<Hill *> &newHills)
{
    hills = newHills;
}

QVector<Jumper *> SimulationSave::getJumpers() const
{
    return jumpers;
}

QVector<Jumper *> &SimulationSave::getJumpersReference()
{
    return jumpers;
}

void SimulationSave::setJumpers(const QVector<Jumper *> &newJumpers)
{
    jumpers = newJumpers;
}

QVector<Hill *> &SimulationSave::getHillsReference()
{
    return hills;
}

QVector<Season> SimulationSave::getSeasons() const
{
    return seasons;
}

QVector<Season> &SimulationSave::getSeasonsReference()
{
    return seasons;
}

void SimulationSave::setSeasons(const QVector<Season> &newSeasons)
{
    seasons = newSeasons;
}

QString SimulationSave::getName() const
{
    return name;
}

void SimulationSave::setName(const QString &newName)
{
    name = newName;
}
