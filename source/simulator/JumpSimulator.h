﻿#ifndef JUMPSIMULATOR_H
#define JUMPSIMULATOR_H

#include <QVector>
#include <QPair>

#include "Jumper.h"
#include "JumpData.h"
#include "WindsCalculator.h"
#include "Hill.h"
#include "Landing.h"
#include "JumpManipulator.h"

#include <QMap>

class Competition;

class JumpSimulator
{
public:
    JumpSimulator(Jumper *jumper = nullptr, const QVector<Wind> & winds = QVector<Wind>(), Hill *hill = nullptr);
private:
    Jumper * jumper;
    JumperSkills * jumperSkills; //automatycznie od jumper
    QVector<Wind> winds;
    Hill * hill;
    int * gate;
    int competitionStartGate;
    double DSQBaseProbability;
    JumpManipulator * manipulator;

    CompetitionRules * competitionRules;

public:
    //skok
    void simulateJump();

    void resetTemporaryParameters();
    void updateJumperSkills();

private:
    enum RandomParameters{
        TakeoffRating,
        FlightRating,
        LandingInstability
    };
    static double getRandomForJumpSimulation(short parameter, Jumper * jumper);

    bool generateDSQ();
    void generateTakeoffRating();
    void generateFlightRating();
    double getMultiplierForFlightStyleEffect();

    void generateDistance();
    void generateWindEffects();
    void generateLanding();
    void generateJudges();
    void calculateCompensations();
    void calculatePoints();

    ///symulacja
    JumpData jumpData;
    void setupJumpData();

    JumpSimulationData * simulationData;

    double getWindSegmentDistance();

public:
    enum WindCompensationDistanceEffect{
        Disabled,
        Original,
        Full
    };
    JumpManipulator *getManipulator() const;
    void setManipulator(JumpManipulator *newManipulator);
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
    double getDSQBaseProbability() const;
    void setDSQBaseProbability(double newDSQBaseProbability);
    CompetitionRules *getCompetitionRules() const;
    void setCompetitionRules(CompetitionRules *newCompetitionRules);
    QVector<Wind> getWinds() const;
    void setWinds(const QVector<Wind> &newWinds);
    int getCompetitionStartGate() const;
    void setCompetitionStartGate(int newCompetitionStartGate);
};

#endif // JUMPSIMULATOR_H
