#include "SeasonCalendar.h"
#include "../utilities/functions.h"
#include "../global/DatabaseObjectsManager.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QtConcurrent>

SeasonCalendar::SeasonCalendar(QString name) : name(name)
{

}

SeasonCalendar::~SeasonCalendar()
{
}

void SeasonCalendar::fixCompetitionsClassifications()
{
    for(auto & comp : competitions){
        int type = comp->getRulesPointer()->getCompetitionType();
        for(auto & classification : comp->getClassificationsReference()){
            if(classification == nullptr)
                MyFunctions::removeFromVector(comp->getClassificationsReference(), classification);
            else if(MyFunctions::vectorContainsByID(classifications, classification) == false){// || ((type != classification->getClassificationType()) && classification->getPunctationType() == Classification::PointsForPlaces && classification->getClassificationType() == Classification::Individual)){
                MyFunctions::removeFromVectorByID(comp->getClassificationsReference(), classification->getID());
            }
        }
    }
}

void SeasonCalendar::fixCompetitionsHills(QVector<Hill *> *hillsList, Hill * defaultHill)
{
    for(auto & comp : competitions){
        if(comp->getHill() == nullptr)
        {
            comp->setHill(defaultHill);
            continue;
        }
        ulong hillID = comp->getHill()->getID();
        bool contains = false;
        Hill * hillWhichContains = nullptr;
        for(auto & hill : qAsConst(*hillsList)){
            if(hill->getID() == hillID){
                contains = true;
                hillWhichContains = hill;
                break;
            }
        }
        if(contains){
            comp->setHill(hillWhichContains);
        }
        else{
            comp->setHill(defaultHill);
        }
    }
}

void SeasonCalendar::fixAdvancementCompetitions()
{
    for(auto & comp : competitions)
    {
        if(comp->getAdvancementCompetition() != nullptr)
        {
            if(MyFunctions::vectorContains(competitions, comp->getAdvancementCompetition()) == false
                || comp->getAdvancementCompetition()->getRulesPointer()->getCompetitionType() != comp->getRulesPointer()->getCompetitionType())
            {
                comp->setAdvancementCompetition(nullptr);
            }
        }
    }
}

void SeasonCalendar::fixAdvancementClassifications()
{
    for(auto & comp : competitions)
    {
        if(comp->getAdvancementClassification() != nullptr)
        {
            if(MyFunctions::vectorContains(classifications, comp->getAdvancementClassification()) == false
                || comp->getAdvancementClassification()->getClassificationType() != comp->getRulesPointer()->getCompetitionType())
            {
                comp->setAdvancementClassification(nullptr);
            }
        }
    }
}

void SeasonCalendar::updateCompetitionsQualifyingCompetitions()
{
    for(auto & comp : competitions)
    {
        comp->updateQualifyingCompetitions(this);
    }
}

SeasonCalendar SeasonCalendar::getFromJson(QJsonObject json, DatabaseObjectsManager * objectsManager)
{
    SeasonCalendar calendar;
    calendar.setID(json.value("id").toString().toULong());
    calendar.setName(json.value("name").toString());

    QJsonArray classificationsArray = json.value("classifications").toArray();
    for(auto val : classificationsArray){
         calendar.getClassificationsReference().push_back(Classification::getFromJson(val.toObject(), objectsManager));
    }
    if(objectsManager != nullptr)
        objectsManager->fill(&calendar.getClassificationsReference());

    QJsonArray competitionsArray = json.value("competitions").toArray();
    for(auto val : competitionsArray){
        CompetitionInfo * c = new CompetitionInfo(CompetitionInfo::getFromJson(val.toObject(), objectsManager));
        calendar.getCompetitionsReference().push_back(c);
        if(objectsManager != nullptr)
            objectsManager->addObject(c);
    }
    calendar.updateCompetitionsQualifyingCompetitions();

    if(objectsManager != nullptr){
        for(auto val : classificationsArray){
            Classification * classification = static_cast<Classification *>(objectsManager->getObjectByID(val.toObject().value("id").toString().toULong()));
            for(auto & singleResult : classification->getResultsReference())
            {
                singleResult->setClassification(classification);
                QJsonArray singleResultsArray = val.toObject().value("results").toArray();
                for(auto jsonRes : singleResultsArray)
                {
                    if(jsonRes.toObject().value("id").toString().toULong() == singleResult->getID())
                    {
                        QJsonArray compsIds = jsonRes.toObject().value("competitions-results-ids").toArray();
                        for(auto id : compsIds)
                            singleResult->getCompetitionsResultsReference().push_back(static_cast<CompetitionResults *>(objectsManager->getObjectByID(id.toString().toULong())));
                    }
                }
                singleResult->updateSingleResults();
                singleResult->updatePointsSum();
            }
            classification->sortInDescendingOrder();
        }
    }

    return calendar;
}

QJsonObject SeasonCalendar::getJsonObject(SeasonCalendar &calendar)
{
    QJsonObject object;
    object.insert("id", QString::number(calendar.getID()));
    object.insert("name", calendar.getName());

    QJsonArray classificationsArray;
    for(auto & cls : qAsConst(calendar.getClassificationsReference()))
        classificationsArray.push_back(Classification::getJsonObject(cls));
    object.insert("classifications", classificationsArray);

    QJsonArray competitionsArray;
    for(auto & cmp : qAsConst(calendar.getCompetitionsReference()))
        competitionsArray.push_back(CompetitionInfo::getJsonObject(*cmp));
    object.insert("competitions", competitionsArray);

    /*QFuture<QJsonObject> classificationsFuture = QtConcurrent::mapped(calendar.getClassifications(), [](Classification * p){return Classification::getJsonObject(p);});
    QJsonArray classificationsArray;
    for(auto & o : classificationsFuture.results())
        classificationsArray.append(o);
    object.insert("classifications", classificationsArray);*.

    /*QFuture<QJsonObject> competitionsFuture = QtConcurrent::mapped(calendar.getCompetitions(), [](CompetitionInfo * p){return CompetitionInfo::getJsonObject(*p);});
    QJsonArray competitionsArray;
    for(auto & o : competitionsFuture.results())
        competitionsArray.append(o);
    object.insert("competitions", competitionsArray);*/

    return object;
}

int SeasonCalendar::getCompetitionMainIndex(QVector<CompetitionInfo *> &competitions, CompetitionInfo *competition)
{
    int index = 0;
    for(auto & comp : competitions)
    {
        if(comp == competition)
            return index;
        if(comp->getSerieType() == CompetitionInfo::Competition || comp->getSerieType() == CompetitionInfo::Qualifications)
            index++;
    }
    return index;
}

CompetitionInfo *SeasonCalendar::getMainCompetitionByIndex(QVector<CompetitionInfo *> &competitions, int index)
{
    int i=0;
    for(auto & comp : competitions)
    {
        if(comp->getSerieType() == CompetitionInfo::Competition || comp->getSerieType() == CompetitionInfo::Qualifications){
            if(i == index)
                return comp;
            i++;
        }
    }
    return nullptr;
}

CompetitionInfo *SeasonCalendar::getCompetitionByIndexAndType(QVector<CompetitionInfo *> &competitions, int index, int type)
{
    int i=0;
    for(auto & comp : competitions)
    {
        if(comp->getRulesPointer()->getCompetitionType() == type)
        {
            if(i == index)
                return comp;
            i++;
        }
    }
    return nullptr;
}

CompetitionInfo *SeasonCalendar::getCompetitionAfterCompetitionFilteredByType(QVector<CompetitionInfo *> &competitions, int actualIndex, int type, int howMany)
{
    if(actualIndex + howMany > competitions.count()){
        for(int i=actualIndex; i<competitions.count(); i++)
        {
            if(competitions[i]->getRulesPointer()->getCompetitionType() == type)
            {
                if(howMany <= 0)
                    return competitions[i];
                else
                {
                    howMany--;
                }
            }
            i++;
        }
    }
    return nullptr;
}

bool SeasonCalendar::getAllPlayed()
{
    for(auto & comp : competitions)
        if(comp->getPlayed() == false)
            return false;
    return true;
}

int SeasonCalendar::howManyCompetitionsPlayed()
{
    int count=0;
    for(auto & c : competitions)
        if(c->getPlayed())
            count++;
    return count;
}

QString SeasonCalendar::getName() const
{
    return name;
}

void SeasonCalendar::setName(const QString &newName)
{
    name = newName;
}

QVector<Classification *> SeasonCalendar::getClassifications() const
{
    return classifications;
}

QVector<Classification *> &SeasonCalendar::getClassificationsReference()
{
    return classifications;
}

void SeasonCalendar::setClassifications(const QVector<Classification *> &newClassifications)
{
    classifications = newClassifications;
}

void SeasonCalendar::setCompetitions(const QVector<CompetitionInfo *> &newCompetitions)
{
    competitions = newCompetitions;
}

QVector<CompetitionInfo *> SeasonCalendar::getCompetitions() const
{
    return competitions;
}

QVector<CompetitionInfo *> &SeasonCalendar::getCompetitionsReference()
{
    return competitions;
}
