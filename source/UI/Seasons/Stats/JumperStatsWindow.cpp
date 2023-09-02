#include "JumperStatsWindow.h"
#include "ui_JumperStatsWindow.h"
#include "../../../global/CountryFlagsManager.h"
#include "../../../utilities/functions.h"

JumperStatsWindow::JumperStatsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JumperStatsWindow)
{
    ui->setupUi(this);

    rangeComboBoxes = new CompetitionsRangeComboBoxesWidget(this);
    ui->verticalLayout_rangeComboBoxes->addWidget(rangeComboBoxes);

    serieTypesCheckBoxes = new SerieTypesComboBoxesWidget(this);
    ui->verticalLayout_serieTypesCheckBoxes->addWidget(serieTypesCheckBoxes);

    classificationsCheckBoxes = new ClassificationsCheckBoxesWidget(this);
    ui->verticalLayout_classificationsComboBoxes->addWidget(classificationsCheckBoxes);

    hillTypesCheckBoxes = new HillTypesCheckBoxesWidget(this);
    ui->verticalLayout_hillTypesCheckBoxes->addWidget(hillTypesCheckBoxes);

    /*layout_takeoffRatingChart = new QVBoxLayout();
    ui->verticalLayout_charts->addItem(layout_takeoffRatingChart);
    layout_flightRatingChart = new QVBoxLayout();
    ui->verticalLayout_charts->layout()->addItem(layout_flightRatingChart);*/

    setupCharts();
    emit ui->checkBox->stateChanged(0);
}

JumperStatsWindow::~JumperStatsWindow()
{
    delete ui;
}

void JumperStatsWindow::setupConnections()
{
    connect(rangeComboBoxes, &CompetitionsRangeComboBoxesWidget::changed, this, &JumperStatsWindow::fillWindow);
    connect(serieTypesCheckBoxes, &SerieTypesComboBoxesWidget::changed, this, &JumperStatsWindow::fillWindow);
    connect(classificationsCheckBoxes, &ClassificationsCheckBoxesWidget::changed, this, &JumperStatsWindow::fillWindow);
    connect(hillTypesCheckBoxes, &HillTypesCheckBoxesWidget::changed, this, &JumperStatsWindow::fillWindow);
}

void JumperStatsWindow::fillWindow()
{
    ui->label_jumperNameAndSurname->setText(jumper->getNameAndSurname());
    ui->label_jumperFlag->setPixmap(CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(jumper->getCountryCode().toLower())).scaled(ui->label_jumperFlag->size()));

    QVector<CompetitionInfo *> competitions = CompetitionInfo::getCompetitionsByStartAndEnd(CompetitionInfo::mergeSeasonsCompetitions(rangeComboBoxes->getSeasonsList()),
                                                                                            rangeComboBoxes->getCompetition(1), rangeComboBoxes->getCompetition(2));
    singleResults = CompetitionSingleResult::getFilteredSingleResults(competitions, jumper,
        serieTypesCheckBoxes->getSerieTypes(), hillTypesCheckBoxes->getHillTypesSet(), classificationsCheckBoxes->getClassifications(), classificationsCheckBoxes->allUnchecked());

    fillJumperApperancesChart();
    fillJudgesPointsChart();
    fillJumperFormChart();
    fillTakeoffRatingChart();
    fillFlightRatingChart();
}

void JumperStatsWindow::fillJumperApperancesChart()
{
    jumperApperancesLineSeries->clear();
    delete jumperApperancesLineSeries;
    jumperApperancesChart->removeAllSeries();
    jumperApperancesLineSeries = new QLineSeries(this);
    jumperApperancesLineSeries->setPen(QPen(QBrush(QColor(qRgb(77, 179, 230))), 3.5));
    connect(jumperApperancesLineSeries, &QLineSeries::hovered, this, &JumperStatsWindow::updateChartCompetitionByJumpDataForJumperApperances);

    int bestPosition = 100000000;
    int worstPosition = 1;
    double averagePosition = 0;
    QVector<int> positions;
    int i=1;
    for(auto & result : singleResults)
    {
        jumperApperancesLineSeries->append(i, result->getPosition());

        if(result->getPosition() < bestPosition)
            bestPosition = result->getPosition();
        else if(result->getPosition() > worstPosition)
            worstPosition = result->getPosition();
        averagePosition += result->getPosition();
        positions.push_back(result->getPosition());
        i++;
    }
    averagePosition /= singleResults.count();
    double standardDev = MyFunctions::standardDeviation(positions, averagePosition);
    standardDev = roundDoubleToTwoPlaces(standardDev);
    averagePosition = roundDoubleToTwoPlaces(averagePosition);

    jumperApperancesChart->addSeries(jumperApperancesLineSeries);
    jumperApperancesChart->createDefaultAxes();
    jumperApperancesChart->axes(Qt::Vertical).first()->setReverse(true);
    jumperApperancesChart->axes(Qt::Vertical).first()->setRange(1, worstPosition);
    jumperApperancesChart->axes(Qt::Horizontal).first()->setLabelsVisible(false);
    jumperApperancesChart->zoom(1);
    jumperApperancesChart->setAnimationOptions(QChart::SeriesAnimations);
    jumperApperancesChartView->setChart(jumperApperancesChart);
    jumperApperancesChartView->setRenderHint(QPainter::Antialiasing);

    ui->label_bestPos->setText(QString::number(bestPosition));
    ui->label_worstPos->setText(QString::number(worstPosition));
    ui->label_averagePos->setText(QString::number(averagePosition));
    ui->label_standardDeviation->setText(QString::number(standardDev));
}

void JumperStatsWindow::fillJudgesPointsChart()
{
    judgesPointsLineSeries->clear();
    judgesPointsChart->removeAllSeries();
    judgesPointsLineSeries = new QLineSeries(this);
    judgesPointsLineSeries->setPen(QPen(QBrush(QColor(qRgb(77, 179, 230))), 3.5));
    connect(judgesPointsLineSeries, &QLineSeries::hovered, this, &JumperStatsWindow::updateChartCompetitionByJumpDataForJudges);

    QVector<double> judgesPoints;
    double worst = 60;
    int i = 1;
    for(auto & singleResult : singleResults)
    {
        for(auto & jump : singleResult->getJumpsReference())
        {
            if(jump.getDSQ() == false && singleResult->getCompetition()->getRulesPointer()->getHasJudgesPoints() == true){
                judgesPoints.push_back(jump.getJudgesPoints());
                judgesPointsLineSeries->append(i, jump.getJudgesPoints());
                i++;
                if(jump.getJudgesPoints() < worst)
                    worst = jump.getJudgesPoints();
            }
        }
    }  
    judgesPointsChart->addSeries(judgesPointsLineSeries);
    judgesPointsChart->createDefaultAxes();
    judgesPointsChart->axes(Qt::Vertical).first()->setRange(worst, 60);
    judgesPointsChart->axes(Qt::Horizontal).first()->setLabelsVisible(false);
    judgesPointsChart->zoom(1);
    judgesPointsChart->setAnimationOptions(QChart::SeriesAnimations);
    judgesPointsChartView->setChart(judgesPointsChart);
    judgesPointsChartView->setRenderHint(QPainter::Antialiasing);
}

void JumperStatsWindow::fillJumperFormChart()
{
    jumperFormLineSeries->clear();
    jumperFormChart->removeAllSeries();
    jumperFormLineSeries = new QLineSeries(this);
    jumperFormLineSeries->setPen(QPen(QBrush(QColor(qRgb(77, 179, 230))), 3.5));
    connect(jumperFormLineSeries, &QLineSeries::hovered, this, &JumperStatsWindow::updateChartCompetitionByJumpDataForForm);

    QVector<double> forms;
    double worst;
    int i = 1;
    for(auto & singleResult : singleResults)
    {
        if(singleResult->getJumpsReference().count() > 0){
            forms.push_back(singleResult->getJumpsReference().first().getJumperForm());
            jumperFormLineSeries->append(i, singleResult->getJumpsReference().first().getJumperForm());
            i++;
        }
    }
    jumperFormChart->addSeries(jumperFormLineSeries);
    jumperFormChart->createDefaultAxes();
    jumperFormChart->axes(Qt::Horizontal).first()->setLabelsVisible(false);
    jumperFormChart->zoom(1);
    jumperFormChart->setAnimationOptions(QChart::SeriesAnimations);
    jumperFormChartView->setChart(jumperFormChart);
    jumperFormChartView->setRenderHint(QPainter::Antialiasing);
}

void JumperStatsWindow::fillTakeoffRatingChart()
{
    takeoffRatingLineSeries->clear();
    takeoffRatingChart->removeAllSeries();
    takeoffRatingLineSeries = new QLineSeries(this);
    takeoffRatingLineSeries->setPen(QPen(QBrush(QColor(qRgb(77, 179, 230))), 3.5));
    connect(takeoffRatingLineSeries, &QLineSeries::hovered, this, &JumperStatsWindow::updateChartCompetitionByJumpDataForTakeoffRating);
    QVector<double> ratings;
    int i = 1;
    for(auto & singleResult : singleResults)
    {
        for(auto & jump : singleResult->getJumpsReference())
        {
            if(jump.getDSQ() == false)
            {
                ratings.push_back(jump.getSimulationDataReference().getTakeoffRating());
                takeoffRatingLineSeries->append(i, ratings.last());
                i++;
            }
        }
    }
    takeoffRatingChart->addSeries(takeoffRatingLineSeries);
    takeoffRatingChart->createDefaultAxes();
    takeoffRatingChart->axes(Qt::Horizontal).first()->setLabelsVisible(false);
    takeoffRatingChart->zoom(1);
    takeoffRatingChart->setAnimationOptions(QChart::SeriesAnimations);
    takeoffRatingChartView->setChart(takeoffRatingChart);
    takeoffRatingChartView->setRenderHint(QPainter::Antialiasing);
}

void JumperStatsWindow::fillFlightRatingChart()
{
    flightRatingLineSeries->clear();
    flightRatingChart->removeAllSeries();
    flightRatingLineSeries = new QLineSeries(this);
    flightRatingLineSeries->setPen(QPen(QBrush(QColor(qRgb(77, 179, 230))), 3.5));
    connect(flightRatingLineSeries, &QLineSeries::hovered, this, &JumperStatsWindow::updateChartCompetitionByJumpDataForFlightRating);

    QVector<double> ratings;
    int i = 1;
    for(auto & singleResult : singleResults)
    {
        for(auto & jump : singleResult->getJumpsReference())
        {
            if(jump.getDSQ() == false)
            {
                ratings.push_back(jump.getSimulationDataReference().getFlightRating());
                flightRatingLineSeries->append(i, ratings.last());
                i++;
            }
        }
    }
    flightRatingChart->addSeries(flightRatingLineSeries);
    flightRatingChart->createDefaultAxes();
    flightRatingChart->axes(Qt::Horizontal).first()->setLabelsVisible(false);
    flightRatingChart->zoom(1);
    flightRatingChart->setAnimationOptions(QChart::SeriesAnimations);
    flightRatingChartView->setChart(flightRatingChart);
    flightRatingChartView->setRenderHint(QPainter::Antialiasing);
}

QCheckBox *JumperStatsWindow::getShowFormCheckBox()
{
    return ui->checkBox;
}

void JumperStatsWindow::updateChartCompetitionBySingleResult(const QPointF &point, bool state, int type)
{
    if(state)
    {
        if((point.x() + 0.4 > int(point.x()) && point.x() - 0.4 < int(point.x())) && (point.y() + 0.4 > point.y() && point.y() - 0.4 < point.y()))
        {
            CompetitionSingleResult * result = singleResults[int(point.x() - 1)];
            Hill * hill = result->getCompetition()->getHill();

            Season * season = nullptr;
            for(auto & s : *rangeComboBoxes->getSeasonsList())
                if(s.getCalendarReference().getCompetitionsReference().contains(result->getCompetition()))
                    season = &s;

            QString string = QString::number(season->getSeasonNumber()) + "/" + QString::number(season->getCalendarReference().getCompetitionsReference().indexOf(result->getCompetition()) + 1)
                             + ": " + hill->getName() + " HS" + QString::number(hill->getHSPoint());
            switch(result->getCompetition()->getSerieType())
            {
            case CompetitionInfo::Competition:
                string += tr(" (Konkurs)");
                break;
            case CompetitionInfo::Qualifications:
                string += tr(" (Kwalifikacje)");
                break;
            case CompetitionInfo::TrialRound:
                string += tr(" (Seria próbna)");
                break;
            case CompetitionInfo::Training:
            {
                CompetitionInfo * mainCompetition = nullptr;
                for(int i=season->getCalendarReference().getCompetitionsReference().indexOf(result->getCompetition()); i<season->getCalendarReference().getCompetitionsReference().count(); i++)
                {
                    CompetitionInfo * c = season->getCalendarReference().getCompetitionsReference()[i];
                    if(c->getSerieType() == CompetitionInfo::Qualifications || c->getSerieType() == CompetitionInfo::Competition)
                    {
                        mainCompetition = season->getCalendarReference().getCompetitionsReference()[i];
                        break;
                    }
                }
                int trainingIndex = 0;
                if(mainCompetition != nullptr)
                    trainingIndex = mainCompetition->getTrainingsReference().indexOf(result->getCompetition()) + 1;
                string += tr(" (Trening ") + QString::number(trainingIndex) + ")";
                break;
            }
            }
            ui->label_chartCompetition->setText(string);
            if(type == Positions)
                ui->label_chartStat->setText(QString::number(result->getPosition()) + tr(" miejsce"));
            else if(type == Form)
                ui->label_chartStat->setText(QString::number(result->getJumpsReference().first().getJumperForm()));

            QPixmap pixmap = CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(hill->getCountryCode().toLower())).scaled(ui->label_hillFlag->size());
            ui->label_hillFlag->setPixmap(pixmap);
        }
    }
}

void JumperStatsWindow::updateChartCompetitionByJumpData(const QPointF &point, bool state, int type)
{
    if(state)
    {
        if((point.x() + 0.4 > int(point.x()) && point.x() - 0.4 < int(point.x())) && (point.y() + 0.4 > point.y() && point.y() - 0.4 < point.y()))
        {
            int jumpDataIndex = point.x();
            int remaining = jumpDataIndex;
            JumpData * jumpData = nullptr;
            CompetitionSingleResult * result = nullptr;
            for(auto & res : singleResults)
            {
                for(auto & jump : res->getJumpsReference())
                {
                    bool ok = res->getCompetition()->getRulesPointer()->getHasJudgesPoints();
                    if(type != JudgesPoints)
                        ok = true;
                    if(jump.getDSQ() == false && ok){
                        remaining--;
                        if(remaining == 0)
                        {
                            jumpData = &jump;
                            result = res;
                            break;
                        }
                    }
                }
            }
            int whichJump = MyFunctions::getIndexOfItemInVector(result->getJumpsReference(), jumpData) + 1;

            Hill * hill = result->getCompetition()->getHill();

            Season * season = nullptr;
            for(auto & s : *rangeComboBoxes->getSeasonsList())
                if(s.getCalendarReference().getCompetitionsReference().contains(result->getCompetition()))
                    season = &s;

            QString string = QString::number(season->getSeasonNumber()) + "/" + QString::number(season->getCalendarReference().getCompetitionsReference().indexOf(result->getCompetition()) + 1)
                             + ": " + hill->getName() + " HS" + QString::number(hill->getHSPoint());
            switch(result->getCompetition()->getSerieType())
            {
            case CompetitionInfo::Competition:
                string += tr(" (Konkurs)");
                break;
            case CompetitionInfo::Qualifications:
                string += tr(" (Kwalifikacje)");
                break;
            case CompetitionInfo::TrialRound:
                string += tr(" (Seria próbna)");
                    break;
            case CompetitionInfo::Training:
            {
                CompetitionInfo * mainCompetition = nullptr;
                for(int i=season->getCalendarReference().getCompetitionsReference().indexOf(result->getCompetition()); i<season->getCalendarReference().getCompetitionsReference().count(); i++)
                {
                    CompetitionInfo * c = season->getCalendarReference().getCompetitionsReference()[i];
                    if(c->getSerieType() == CompetitionInfo::Qualifications || c->getSerieType() == CompetitionInfo::Competition)
                    {
                        mainCompetition = season->getCalendarReference().getCompetitionsReference()[i];
                        break;
                    }
                }
                int trainingIndex = 0;
                if(mainCompetition != nullptr)
                    trainingIndex = mainCompetition->getTrainingsReference().indexOf(result->getCompetition()) + 1;
                string += tr(" (Trening ") + QString::number(trainingIndex) + ")";
                break;
            }
            }
            string += tr(" (Skok ") + QString::number(whichJump) + ")";
            ui->label_chartCompetition->setText(string);
            if(type == JudgesPoints)
                ui->label_chartStat->setText(QString::number(jumpData->getJudgesPoints()) + tr(" pkt"));
            else if(type == TakeoffRating)
                ui->label_chartStat->setText(QString::number(jumpData->getSimulationDataReference().getTakeoffRating()));
            else if(type == FlightRating)
                ui->label_chartStat->setText(QString::number(jumpData->getSimulationDataReference().getFlightRating()));

            QPixmap pixmap = CountryFlagsManager::getFlagPixmap(CountryFlagsManager::convertThreeLettersCountryCodeToTwoLetters(hill->getCountryCode().toLower())).scaled(ui->label_hillFlag->size());
            ui->label_hillFlag->setPixmap(pixmap);
        }
    }
}

void JumperStatsWindow::updateChartCompetitionByJumpDataForJumperApperances(const QPointF &point, bool state)
{
    updateChartCompetitionBySingleResult(point, state, Positions);
}

void JumperStatsWindow::updateChartCompetitionByJumpDataForJudges(const QPointF &point, bool state)
{
    updateChartCompetitionByJumpData(point, state, JudgesPoints);
}

void JumperStatsWindow::updateChartCompetitionByJumpDataForForm(const QPointF &point, bool state)
{
    updateChartCompetitionBySingleResult(point, state, Form);
}

void JumperStatsWindow::updateChartCompetitionByJumpDataForTakeoffRating(const QPointF &point, bool state)
{
    updateChartCompetitionByJumpData(point, state, TakeoffRating);
}

void JumperStatsWindow::updateChartCompetitionByJumpDataForFlightRating(const QPointF &point, bool state)
{
    updateChartCompetitionByJumpData(point, state, FlightRating);
}

HillTypesCheckBoxesWidget *JumperStatsWindow::getHillTypesCheckBoxes() const
{
    return hillTypesCheckBoxes;
}

void JumperStatsWindow::setHillTypesCheckBoxes(HillTypesCheckBoxesWidget *newHillTypesCheckBoxes)
{
    hillTypesCheckBoxes = newHillTypesCheckBoxes;
}

ClassificationsCheckBoxesWidget *JumperStatsWindow::getClassificationsCheckBoxes() const
{
    return classificationsCheckBoxes;
}

void JumperStatsWindow::setClassificationsCheckBoxes(ClassificationsCheckBoxesWidget *newClassificationsCheckBoxes)
{
    classificationsCheckBoxes = newClassificationsCheckBoxes;
}

void JumperStatsWindow::setupCharts()
{
    jumperApperancesChart = new QChart();
    jumperApperancesChart->legend()->hide();
    jumperApperancesChart->setTitle(tr("Występy zawodnika"));
    jumperApperancesChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    jumperApperancesChartView = new QChartView(this);
    jumperApperancesChartView->setRenderHint(QPainter::Antialiasing);
    jumperApperancesChartView->setRubberBand(QChartView::HorizontalRubberBand);
    ui->verticalLayout_jumperApperancesChart->addWidget(jumperApperancesChartView);
    jumperApperancesLineSeries = new QLineSeries(this);
    jumperApperancesLineSeries->setPen(QPen(QBrush(QColor(qRgb(77, 179, 230))), 3.5));
    connect(jumperApperancesLineSeries, &QLineSeries::hovered, this, &JumperStatsWindow::updateChartCompetitionByJumpDataForJumperApperances);

    judgesPointsChart = new QChart();
    judgesPointsChart->legend()->hide();
    judgesPointsChart->setTitle(tr("Punkty od sędziów"));
    judgesPointsChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    judgesPointsChartView = new QChartView(this);
    judgesPointsChartView->setRenderHint(QPainter::Antialiasing);
    judgesPointsChartView->setFixedHeight(450);
    judgesPointsChartView->setRubberBand(QChartView::HorizontalRubberBand);
    ui->verticalLayout_charts->addWidget(judgesPointsChartView);
    judgesPointsLineSeries = new QLineSeries(this);
    judgesPointsLineSeries->setPen(QPen(QBrush(QColor(qRgb(77, 179, 230))), 3.5));
    connect(judgesPointsLineSeries, &QLineSeries::hovered, this, &JumperStatsWindow::updateChartCompetitionByJumpDataForJudges);

    jumperFormChart = new QChart();
    jumperFormChart->legend()->hide();
    jumperFormChart->setTitle(tr("Forma zawodnika"));
    jumperFormChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    jumperFormChartView = new QChartView(this);
    jumperFormChartView->setRenderHint(QPainter::Antialiasing);
    jumperFormChartView->setFixedHeight(450);
    jumperFormChartView->setRubberBand(QChartView::HorizontalRubberBand);
    ui->verticalLayout_charts->addWidget(jumperFormChartView);
    jumperFormLineSeries = new QLineSeries(this);
    jumperFormLineSeries->setPen(QPen(QBrush(QColor(qRgb(77, 179, 230))), 3.5));
    connect(jumperFormLineSeries, &QLineSeries::hovered, this, &JumperStatsWindow::updateChartCompetitionByJumpDataForForm);

    takeoffRatingChart = new QChart();
    takeoffRatingChart->legend()->hide();
    takeoffRatingChart->setTitle(tr("Ocena wybicia"));
    takeoffRatingChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    takeoffRatingChartView = new QChartView(this);
    takeoffRatingChartView->setRenderHint(QPainter::Antialiasing);
    takeoffRatingChartView->setFixedHeight(450);
    takeoffRatingChartView->setRubberBand(QChartView::HorizontalRubberBand);
    ui->verticalLayout_charts->addWidget(takeoffRatingChartView);
    takeoffRatingLineSeries = new QLineSeries(this);
    takeoffRatingLineSeries->setPen(QPen(QBrush(QColor(qRgb(77, 179, 230))), 3.5));
    connect(takeoffRatingLineSeries, &QLineSeries::hovered, this, &JumperStatsWindow::updateChartCompetitionByJumpDataForTakeoffRating);

    flightRatingChart = new QChart();
    flightRatingChart->legend()->hide();
    flightRatingChart->setTitle(tr("Ocena lotu"));
    flightRatingChart->setTitleFont(QFont("Quicksand Medium", 15, 1, false));
    flightRatingChartView = new QChartView(this);
    flightRatingChartView->setRenderHint(QPainter::Antialiasing);
    flightRatingChartView->setFixedHeight(450);
    flightRatingChartView->setRubberBand(QChartView::HorizontalRubberBand);
    ui->verticalLayout_charts->addWidget(flightRatingChartView);
    flightRatingLineSeries = new QLineSeries(this);
    flightRatingLineSeries->setPen(QPen(QBrush(QColor(qRgb(77, 179, 230))), 3.5));
    connect(flightRatingLineSeries, &QLineSeries::hovered, this, &JumperStatsWindow::updateChartCompetitionByJumpDataForFlightRating);
}

Jumper *JumperStatsWindow::getJumper() const
{
    return jumper;
}

void JumperStatsWindow::setJumper(Jumper *newJumper)
{
    jumper = newJumper;
}

CompetitionsRangeComboBoxesWidget *JumperStatsWindow::getRangeComboBoxes() const
{
    return rangeComboBoxes;
}

void JumperStatsWindow::setRangeComboBoxes(CompetitionsRangeComboBoxesWidget *newRangeComboBoxes)
{
    rangeComboBoxes = newRangeComboBoxes;
}

void JumperStatsWindow::on_horizontalScrollBar_jumperStatsChartScrollBar_valueChanged(int value)
{
    jumperApperancesChart->scroll(-value * 0.7, 0);
}

void JumperStatsWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Unchecked)
    {
        jumperFormChartView->hide();
        takeoffRatingChartView->hide();
        flightRatingChartView->hide();
    }
    else if(arg1 == Qt::Checked)
    {
        jumperFormChartView->show();
        takeoffRatingChartView->show();
        flightRatingChartView->show();
    }
}

