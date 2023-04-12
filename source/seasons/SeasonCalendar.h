#ifndef SEASONCALENDAR_H
#define SEASONCALENDAR_H

#include <QVector>
#include "../competitions/CompetitionInfo.h"

class SeasonCalendar
{
public:
    SeasonCalendar();

private:
    QVector<CompetitionInfo> competitions;

public:
    QVector<CompetitionInfo> getCompetitions() const;
    void setCompetitions(const QVector<CompetitionInfo> &newCompetitions);
};

#endif // SEASONCALENDAR_H
