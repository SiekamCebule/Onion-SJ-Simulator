#ifndef JUMPSIMULATIONDATA_H
#define JUMPSIMULATIONDATA_H

#include "../utilities/ClassWithID.h"

class JumpSimulationData : ClassWithID
{
    friend class JumpSimulator;
public:
    JumpSimulationData();

    void reset();

    double getTakeoffRating() const;
    void setTakeoffRating(double newTakeoffRating);
    double getFlightRating() const;
    void setFlightRating(double newFlightRating);
    double getJudgesRating() const;
    void setJudgesRating(double newJudgesRating);
    int getDSQProbability() const;
    void setDSQProbability(int newDSQProbability);

private:
    double flightRating;
    double takeoffRating;
    double judgesRating;
    int DSQProbability;
};

#endif // JUMPSIMULATIONDATA_H
