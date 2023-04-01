#ifndef JUMPMANIPULATOR_H
#define JUMPMANIPULATOR_H

#include "JumperSkills.h"
#include "WindsCalculator.h"
#include <QPair>


class JumpManipulator
{
public:
    JumpManipulator();

private:
    QPair<double, double> distanceRange; //min, max
    QPair<double, double> landingInstabilityRange;
    QPair<double, double> takeoffRatingRange;
    QPair<double, double> flightRatingRange;
    QPair<double, double> judgesRatingRange;
    QPair<double, double> averagedWindRange;
    bool averagedWindRangeEnabled;

    double distanceBonus;
    double landingInstabilityBonus;
    double takeoffRatingBonus;
    double flightRatingBonus;
    double judgesRatingBonus;
    double averagedWindBonus;

    int exactLandingType;
    QVector<double> exactJudges;
    QVector<Wind> exactWinds;
    int exactDSQProbability;


public:
    QPair<double, double> getDistanceRange() const;
    void setDistanceRange(const QPair<double, double> &newDistanceRange);
    QPair<double, double> getLandingInstabilityRange() const;
    void setLandingInstabilityRange(const QPair<double, double> &newLandingInstabilityRange);
    QPair<double, double> getTakeoffRatingRange() const;
    void setTakeoffRatingRange(const QPair<double, double> &newTakeoffRatingRange);
    QPair<double, double> getFlightRatingRange() const;
    void setFlightRatingRange(const QPair<double, double> &newFlightRatingRange);
    QPair<double, double> getJudgesRatingRange() const;
    void setJudgesRatingRange(const QPair<double, double> &newJudgesRatingRange);
    QPair<double, double> getAveragedWindRange() const;
    void setAveragedWindRange(const QPair<double, double> &newAveragedWindRange);
    double getDistanceBonus() const;
    void setDistanceBonus(double newDistanceBonus);
    double getLandingInstabilityBonus() const;
    void setLandingInstabilityBonus(double newLandingInstabilityBonus);
    double getTakeoffRatingBonus() const;
    void setTakeoffRatingBonus(double newTakeoffRatingBonus);
    double getFlightRatingBonus() const;
    void setFlightRatingBonus(double newFlightRatingBonus);
    double getJudgesRatingBonus() const;
    void setJudgesRatingBonus(double newJudgesRatingBonus);
    double getAveragedWindBonus() const;
    void setAveragedWindBonus(double newAveragedWindBonus);
    int getExactLandingType() const;
    void setExactLandingType(int newExactLandingType);
    QVector<double> getExactJudges() const;
    void setExactJudges(const QVector<double> &newExactJudges);
    QVector<Wind> getExactWinds() const;
    void setExactWinds(const QVector<Wind> &newExactWinds);
    int getExactDSQProbability() const;
    void setExactDSQProbability(int newExactDSQProbability);
    bool getAveragedWindRangeEnabled() const;
    void setAveragedWindRangeEnabled(bool newAveragedWindRangeEnabled);
};

#endif // JUMPMANIPULATOR_H
