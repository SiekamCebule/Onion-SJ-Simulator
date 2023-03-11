#ifndef COMPETITIONRULESEDITORWIDGET_H
#define COMPETITIONRULESEDITORWIDGET_H

#include <QWidget>
#include "../../competitions/CompetitionRules.h"

namespace Ui {
class CompetitionRulesEditorWidget;
}

class CompetitionRulesEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompetitionRulesEditorWidget(QWidget *parent = nullptr);
    ~CompetitionRulesEditorWidget();

    void fillCompetitionRulesInputs();
    void fillRoundsInputs(bool setup = false);

    void removeSubmitButton();

    QString getNameFromInput();
    QVector<RoundInfo> getRoundsFromInput();
    bool getHas95HSRuleFromInput();
    bool getHasWindCompensations();
    bool getHasGateCompensations();
    bool getHasJudgesPoints();
    short getCompetitionTypeFromInput();

    CompetitionRules getCompetitionRulesFromWidgetInputs();

signals:
    void submitted();

private:
    Ui::CompetitionRulesEditorWidget *ui;

    CompetitionRules * competitionRules;

public:
    CompetitionRules *getCompetitionRules() const;
    void setCompetitionRules(CompetitionRules *newCompetitionRules);
private slots:
    void on_pushButton_submit_clicked();
};

#endif // COMPETITIONRULESEDITORWIDGET_H
