#ifndef JUMPERSTATSWINDOW_H
#define JUMPERSTATSWINDOW_H

#include <QDialog>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QLabel>
#include <QCheckBox>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "FilteringWidgets/CompetitionsRangeComboBoxesWidget.h"
#include "FilteringWidgets/SerieTypesComboBoxesWidget.h"
#include "FilteringWidgets/ClassificationsCheckBoxesWidget.h"
#include "FilteringWidgets/HillTypesCheckBoxesWidget.h"

namespace Ui {
class JumperStatsWindow;
}

class JumperStatsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit JumperStatsWindow(QWidget *parent = nullptr);
    ~JumperStatsWindow();

    enum ChartStatType{
        Positions,
        JudgesPoints,
        Form,
        TakeoffRating,
        FlightRating
    };

    void setupConnections();
    void fillWindow();

    void fillJumperApperancesChart();
    void fillJudgesPointsChart();
    void fillJumperFormChart();
    void fillTakeoffRatingChart();
    void fillFlightRatingChart();

    QCheckBox * getShowFormCheckBox();

private slots:
    void updateChartCompetitionBySingleResult(const QPointF & point, bool state, int type);
    void updateChartCompetitionByJumpData(const QPointF & point, bool state, int type);

    void updateChartCompetitionByJumpDataForJumperApperances(const QPointF & point, bool state);
    void updateChartCompetitionByJumpDataForJudges(const QPointF & point, bool state);
    void updateChartCompetitionByJumpDataForForm(const QPointF & point, bool state);
    void updateChartCompetitionByJumpDataForTakeoffRating(const QPointF & point, bool state);
    void updateChartCompetitionByJumpDataForFlightRating(const QPointF & point, bool state);

private:
    Ui::JumperStatsWindow *ui;
    CompetitionsRangeComboBoxesWidget * rangeComboBoxes;
    SerieTypesComboBoxesWidget * serieTypesCheckBoxes;
    ClassificationsCheckBoxesWidget * classificationsCheckBoxes;
    HillTypesCheckBoxesWidget * hillTypesCheckBoxes;

    Jumper * jumper;
    QVector<CompetitionSingleResult *> singleResults;

    //Jumper apperances
    QChart * jumperApperancesChart;
    QChartView * jumperApperancesChartView;
    QLineSeries * jumperApperancesLineSeries;
    //Judges points
    QChart * judgesPointsChart;
    QChartView * judgesPointsChartView;
    QLineSeries * judgesPointsLineSeries;
    //Form
    QChart * jumperFormChart;
    QChartView * jumperFormChartView;
    QLineSeries * jumperFormLineSeries;
    //Takeoff rating
    QChart * takeoffRatingChart;
    QChartView * takeoffRatingChartView;
    QLineSeries * takeoffRatingLineSeries;
    //Flight rating
    QChart * flightRatingChart;
    QChartView * flightRatingChartView;
    QLineSeries * flightRatingLineSeries;

    void setupCharts();

    //Charts UI
    //3. Ocena wybicia
    QVBoxLayout * layout_takeoffRatingChart;
    //4. Ocena lotu
    QVBoxLayout * layout_flightRatingChart;

public:
    CompetitionsRangeComboBoxesWidget *getRangeComboBoxes() const;
    void setRangeComboBoxes(CompetitionsRangeComboBoxesWidget *newRangeComboBoxes);
    Jumper *getJumper() const;
    void setJumper(Jumper *newJumper);
    ClassificationsCheckBoxesWidget *getClassificationsCheckBoxes() const;
    void setClassificationsCheckBoxes(ClassificationsCheckBoxesWidget *newClassificationsCheckBoxes);   
    HillTypesCheckBoxesWidget *getHillTypesCheckBoxes() const;
    void setHillTypesCheckBoxes(HillTypesCheckBoxesWidget *newHillTypesCheckBoxes);

private slots:
    void on_horizontalScrollBar_jumperStatsChartScrollBar_valueChanged(int value);
    void on_checkBox_stateChanged(int arg1);
};

#endif // JUMPERSTATSWINDOW_H
