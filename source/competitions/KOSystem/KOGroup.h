#ifndef KOGROUP_H
#define KOGROUP_H

#include <QVector>
#include <QJsonObject>
#include "../../simulator/Jumper.h"
#include "../../simulator/Team.h"
#include "../../utilities/ClassWithID.h"

class CompetitionInfo;
class CompetitionRules;
class RoundInfo;

class KOGroup : public ClassWithID
{
public:
    KOGroup();

    enum GroupsSelectionType{
        Classic, //50-1, 49-2, 30-21
        ForLargerGroups, //1, 11, 21, 31, 41
        BasketsDraw,
        Random
    };

    static QJsonObject getJsonObject(KOGroup &group);
    static KOGroup getFromJson(const QJsonObject & json);

    static QVector<KOGroup> constructDefaultKOGroups(RoundInfo * roundInfo, QVector<Jumper *> * jumpers, int selectionType, CompetitionInfo *competition);

private:
    CompetitionInfo * competition;

    int number;

    //Aktywny jeden z tych vectorów w zależności competitionType
    QVector<Jumper *> jumpers;
    QVector<Team *> teams;
public:
    CompetitionInfo *getCompetition() const;
    void setCompetition(CompetitionInfo *newCompetition);
    int getNumber() const;
    void setNumber(int newNumber);
    QVector<Jumper *> getJumpers() const;
    QVector<Jumper *> & getJumpersReference();
    void setJumpers(const QVector<Jumper *> &newJumpers);
    QVector<Team *> & getTeamsReference();
    void setTeams(const QVector<Team *> &newTeams);
};

#endif // KOGROUP_H
