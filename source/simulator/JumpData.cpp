#include "JumpData.h"
#include "JumpSimulator.h"
#include "wind-generation/WindsGenerator.h"
#include "../global/SeasonDatabaseObjectsManager.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QJsonArray>
#include <QMessageBox>

extern SeasonDatabaseObjectsManager seasonObjectsManager;

JumpData::JumpData(Jumper *jumper, Hill *hill) : jumper(jumper),
    hill(hill),
    ClassWithID()
{
    DSQ = false;
    DNS = false;
}

bool JumpData::getDSQ() const
{
    return DSQ;
}

void JumpData::setDSQ(bool newDSQ)
{
    DSQ = newDSQ;
}

bool JumpData::getBeats95HSPercents() const
{
    return beats95HSPercents;
}

void JumpData::setBeats95HSPercents(bool newBeats95HSPercents)
{
    beats95HSPercents = newBeats95HSPercents;
}

int JumpData::getCoachGate() const
{
    return coachGate;
}

void JumpData::setCoachGate(int newCoachGate)
{
    coachGate = newCoachGate;
}

bool JumpData::getHasCoachGate() const
{
    return hasCoachGate;
}

void JumpData::setHasCoachGate(bool newHasCoachGate)
{
    hasCoachGate = newHasCoachGate;
}

bool JumpData::getDNS() const
{
    return DNS;
}

void JumpData::setDNS(bool newDNS)
{
    DNS = newDNS;
}

QVector<Wind> JumpData::getWinds() const
{
    return winds;
}

QVector<Wind> *JumpData::getWindsPointer()
{
    return &winds;
}

void JumpData::setWinds(const QVector<Wind> &newWinds)
{
    winds = newWinds;
}

JumpSimulationData JumpData::getSimulationData() const
{
    return simulationData;
}

void JumpData::setSimulationData(const JumpSimulationData &newSimulationData)
{
    simulationData = newSimulationData;
}

double JumpData::getAveragedWind() const
{
    return averagedWind;
}

void JumpData::setAveragedWind(double newAveragedWind)
{
    averagedWind = newAveragedWind;
}

int JumpData::getGate() const
{
    return gate;
}

void JumpData::setGate(int newGate)
{
    gate = newGate;
}

JumpSimulator *JumpData::getSimulator() const
{
    return simulator;
}

void JumpData::setSimulator(JumpSimulator *newSimulator)
{
    simulator = newSimulator;
}

void JumpData::reset()
{
    distance = points = gateCompensation = windCompensation = totalCompensation = gate = judgesPoints = 0;
    landing = Landing();
    judges.clear();
    judges.fill(0, 5);
    winds.clear();
    averagedWind = 0;
    jumper = nullptr;
    hill = nullptr;
    simulator = nullptr;
    DSQ = false;
    simulationData.reset();
}

QJsonObject JumpData::getJsonObject(const JumpData &jumpData)
{
    QJsonObject object;
    object.insert("id", QString::number(jumpData.getID()));
    object.insert("distance", jumpData.getDistance());
    object.insert("points", jumpData.getPoints());
    object.insert("gate", jumpData.getGate());
    object.insert("averaged-wind", jumpData.getAveragedWind());
    object.insert("gate-compensation", jumpData.getGateCompensation());
    object.insert("wind-compensation", jumpData.getWindCompensation());
    object.insert("total-compensation", jumpData.getTotalCompensation());
    object.insert("judges-points", jumpData.getJudgesPoints());
    object.insert("landing-type", jumpData.getLanding().getType());
    object.insert("landing-imbalance", jumpData.getLanding().getImbalance());
    object.insert("dsq", jumpData.getDSQ());
    object.insert("dns", jumpData.getDNS());
    object.insert("has-coach-gate", jumpData.getHasCoachGate());
    object.insert("coach-gate", jumpData.getCoachGate());
    object.insert("beats-95-hs-percents", jumpData.getBeats95HSPercents());
    QJsonArray judgesArray;
    for(const auto & jg : jumpData.getJudges())
    {
        judgesArray.push_back(QJsonValue(jg));
    }
    object.insert("judges", judgesArray);
    QJsonObject simulationDataObject;
    simulationDataObject.insert("takeoff-rating", jumpData.getSimulationData().getTakeoffRating());
    simulationDataObject.insert("flight-rating", jumpData.getSimulationData().getFlightRating());
    simulationDataObject.insert("judges-rating", jumpData.getSimulationData().getJudgesRating());
    simulationDataObject.insert("dsq-probability", jumpData.getSimulationData().getDSQProbability());
    simulationDataObject.insert("inrun-snow", jumpData.getSimulationData().getInrunSnow());
    object.insert("simulation-data", simulationDataObject);
    QJsonArray windsArray;
    int i=0;
    for(const auto & wind : jumpData.getWinds())
    {
        QJsonObject windObject;
        windObject.insert("range", QJsonValue(QString::number(WindsGenerator::getRangeOfWindSector(i+1, jumpData.getHill()->getKPoint()).first) + " - " + QString::number(WindsGenerator::getRangeOfWindSector(i+1, jumpData.getHill()->getKPoint()).second)));
        windObject.insert("direction", wind.getDirection());
        windObject.insert("strength", wind.getStrength());
        windsArray.push_back(windObject);
        i++;
    }
    object.insert("winds", windsArray);

    object.insert("jumper-id", QString::number(jumpData.getJumper()->getID()));
    object.insert("hill-id", QString::number(jumpData.getHill()->getID()));
    return object;
}

JumpData JumpData::getFromJson(QJsonObject obj)
{
    JumpData jump;
    jump.setID(obj.value("id").toString().toULong());
    jump.setDistance(obj.value("distance").toDouble());
    jump.setPoints(obj.value("points").toDouble());
    jump.setGate(obj.value("gate").toInt());
    jump.setAveragedWind(obj.value("averaged-wind").toDouble());
    jump.setGateCompensation(obj.value("gate-compensation").toDouble());
    jump.setWindCompensation(obj.value("wind-compensation").toDouble());
    jump.setTotalCompensation(obj.value("total-compensation").toDouble());
    jump.setJudgesPoints(obj.value("judges-points").toDouble());
    jump.setLanding(Landing(obj.value("landing-type").toInt(), obj.value("landing-imbalance").toDouble()));
    jump.setDSQ(obj.value("dsq").toBool());
    jump.setDNS(obj.value("dns").toBool());
    jump.setHasCoachGate(obj.value("has-coach-gate").toBool());
    jump.setCoachGate(obj.value("coach-gate").toInt());
    jump.setBeats95HSPercents(obj.value("beats-95-hs-percents").toBool());
    QJsonArray judgesArray = obj.value("judges").toArray();
    QVector<double> judges;
    for(const auto & jg : judgesArray)
    {
        judges.push_back(jg.toDouble());
    }
    jump.setJudges(judges);
    QJsonObject simulationDataObject = obj.value("simulation-data").toObject();
    JumpSimulationData simulationData;
    simulationData.setTakeoffRating(simulationDataObject.value("takeoff-rating").toDouble());
    simulationData.setFlightRating(simulationDataObject.value("flight-rating").toDouble());
    simulationData.setJudgesRating(simulationDataObject.value("judges-rating").toDouble());
    simulationData.setDSQProbability(simulationDataObject.value("dsq-probability").toInt());
    simulationData.setInrunSnow(simulationDataObject.value("inrun-snow").toDouble());
    jump.setSimulationData(simulationData);
    QJsonArray windsArray = obj.value("winds").toArray();
    QVector<Wind> winds;
    int i=0;
    for(const auto & wind : windsArray)
    {
        QJsonObject windObject = wind.toObject();
        Wind w;
        w.setDirection(windObject.value("direction").toInt());
        w.setStrength(windObject.value("strength").toDouble());
        winds.push_back(w);
        i++;
    }
    jump.setWinds(winds);

    jump.setJumper(static_cast<Jumper *>(seasonObjectsManager.getObjectByID(obj.value("jumper-id").toString().toULong())));
    jump.setHill(static_cast<Hill *>(seasonObjectsManager.getObjectByID(obj.value("hill-id").toString().toULong())));

    return jump;
}

Jumper *JumpData::getJumper() const
{
    return jumper;
}

void JumpData::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

Hill *JumpData::getHill() const
{
    return hill;
}

void JumpData::setHill(Hill *newHill)
{
    hill = newHill;
}

double JumpData::getJudgesPoints() const
{
    return judgesPoints;
}

void JumpData::setJudgesPoints(double newJudgesPoints)
{
    judgesPoints = newJudgesPoints;
}

QVector<double> JumpData::getJudges() const
{
    return judges;
}

void JumpData::setJudges(const QVector<double> &newJudges)
{
    judges = newJudges;
}

Landing JumpData::getLanding() const
{
    return landing;
}

void JumpData::setLanding(const Landing &newLanding)
{
    landing = newLanding;
}

double JumpData::getTotalCompensation() const
{
    return totalCompensation;
}

void JumpData::setTotalCompensation(double newTotalCompensation)
{
    totalCompensation = newTotalCompensation;
}

double JumpData::getWindCompensation() const
{
    return windCompensation;
}

void JumpData::setWindCompensation(double newWindCompensation)
{
    windCompensation = newWindCompensation;
}

double JumpData::getGateCompensation() const
{
    return gateCompensation;
}

void JumpData::setGateCompensation(double newGateCompensation)
{
    gateCompensation = newGateCompensation;
}

double JumpData::getPoints() const
{
    return points;
}

void JumpData::setPoints(double newPoints)
{
    points = newPoints;
}

double JumpData::getDistance() const
{
    return distance;
}

void JumpData::setDistance(double newDistance)
{
    distance = newDistance;
}

QDebug operator<<(QDebug d, const JumpData & jumpData)
{
    Jumper * jumper = jumpData.getJumper();
    d <<jumper->getNameAndSurname()<<" ("<<jumper->getCountryCode()<<")";
    d<<jumpData.getDistance()<<"m ("<<jumpData.getPoints()<<" pts) --> Wiatr: "<<WindsCalculator::getAveragedWind(jumpData.getWinds(), jumpData.getSimulator()->getCompetitionRules()->getWindAverageCalculatingType()).getStrengthToAveragedWind()<<"m/s, ("<<jumpData.getWindCompensation()<<" pts), Belka "<<jumpData.getGate()<<" ("<<jumpData.getGateCompensation()<<"),   |"<<jumpData.getJudges().at(0)<<"|"<<jumpData.getJudges().at(1)<<"|"<<jumpData.getJudges().at(2)<<"|"<<jumpData.getJudges().at(3)<<"|"<<jumpData.getJudges().at(4)<<"|"<<",   Lądowanie: "<<jumpData.getLanding().getTextLandingType();

    return d;
}
