﻿#ifndef JUMPSIMULATOR_H
#define JUMPSIMULATOR_H

#include <QVector>
#include <QPair>

#include "Jumper.h"
#include "JumpData.h"
#include "WindsInfo.h"
#include "Hill.h"
#include "Landing.h"

#include <QMap>

class Competition;

class JumpSimulator
{
public:
    JumpSimulator(Jumper *jumper = nullptr, const WindsInfo & conditionsInfo = WindsInfo(), Hill *hill = nullptr);
private:
    Jumper * jumper;
    JumperSkills * jumperSkills; //automatycznie od jumper
    WindsInfo windsInfo;
    Hill * hill;
    int * gate;

public:
    //skok
    void simulateJump();
    void resetTemporaryParameters();

    bool jumperCharacteristicsContains(const Characteristic & characteristics);

    void updateJumperSkills();

    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    Hill *getHill() const;
    void setHill(Hill *newHill);
    double getTakeoffRating() const;
    void setTakeoffRating(double newTakeoffRating);
    double getFlightRating() const;
    void setFlightRating(double newFlightRating);
    JumperSkills *getJumperSkills() const;
    QVector<double> getJudges() const;
    void setJudges(const QVector<double> &newJudges);
    JumpData getJumpData();
    void setJumpData(const JumpData &newJumpData);
    int *getGate() const;
    void setGate(int *newGate);
    WindsInfo getWindsInfo() const;
    void setWindsInfo(const WindsInfo &newWindsInfo);
    short getWindAverageCalculatingType() const;
    void setWindAverageCalculatingType(short newWindAverageCalculatingType);

private:
    void generateTakeoffRating();
    void generateFlightRating();
    double getMultiplierForFlightStyleEffect();

    void generateDistance();
    void generateWindEffects(); // np. zmiana odległości przez wiatr, albo w przyszłości jakieś nierówności w locie przez taki boczny wiatr.
    void generateLanding();
    void generateJudges();
    void calculateCompensations();
    void calculatePoints();

    //symulacja
    double takeoffRating;
    double flightRating;
    double judgesRating;

    JumpData jumpData;
    void setupJumpData();

    short windAverageCalculatingType;
};

#endif // JUMPSIMULATOR_H
