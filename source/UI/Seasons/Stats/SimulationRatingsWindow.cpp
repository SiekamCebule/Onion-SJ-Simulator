#include "SimulationRatingsWindow.h"
#include "ui_SimulationRatingsWindow.h"
#include "../../../utilities/functions.h"
#include "../../../global/PointsForPlacesPreset.h"
#include "../../../global/GlobalDatabase.h"
#include "../../EditorWidgets/EditStartListWithJumpersListsWindow.h"
#include <QInputDialog>

SimulationRatingsWindow::SimulationRatingsWindow(SimulationSave * save, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulationRatingsWindow),
    save(save)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window);

    classificationsCheckBoxes = new ClassificationsCheckBoxesWidget(this);
    ui->verticalLayout_classificationsComboBoxes->addWidget(classificationsCheckBoxes);
    rangeComboBoxes = new CompetitionsRangeComboBoxesWidget(this);
    ui->verticalLayout_rangeComboBoxes->addWidget(rangeComboBoxes);
    serieTypesCheckBoxes = new SerieTypesComboBoxesWidget(this);
    ui->verticalLayout_rangeComboBoxes->addWidget(serieTypesCheckBoxes);
    hillTypesCheckBoxes = new HillTypesCheckBoxesWidget(this);
    ui->verticalLayout_hillTypesCheckBoxes->addWidget(hillTypesCheckBoxes);

    //Averages models
    averagePositionModel = new SimulationRatingTableModel(this);
    ui->tableView_avgPosition->setModel(averagePositionModel);
    averageJudgesModel = new SimulationRatingTableModel(this);
    ui->tableView_avgJudges->setModel(averagePositionModel);
    averageWindModel = new SimulationRatingTableModel(this);
    ui->tableView_avgWind->setModel(averagePositionModel);
    averageTakeoffRatingModel = new SimulationRatingTableModel(this);
    ui->tableView_avgTakeoffRating->setModel(averagePositionModel);
    averageFlightRatingModel = new SimulationRatingTableModel(this);
    ui->tableView_avgFlightRating->setModel(averagePositionModel);
    averageDistanceModel = new SimulationRatingTableModel(this);
    ui->tableView_avgDistance->setModel(averagePositionModel);
    //Records models
    bestDistanceModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_bestDistance->setModel(bestDistanceModel);
    worstDistanceModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_worstDistance->setModel(worstDistanceModel);
    bestPointsModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_bestPoints->setModel(bestPointsModel);
    worstPointsModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_worstPoints->setModel(worstPointsModel);
    bestJudgesModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_bestJudges->setModel(bestJudgesModel);
    worstJudgesModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_worstJudges->setModel(worstJudgesModel);
    bestWindModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_bestWind->setModel(bestWindModel);
    worstWindModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_worstWind->setModel(worstWindModel);
    bestTakeoffModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_bestTakeoff->setModel(bestTakeoffModel);
    worstTakeoffModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_worstTakeoff->setModel(worstTakeoffModel);
    bestFlightModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_bestFlight->setModel(bestFlightModel);
    worstFlightModel = new SimulationRecordsTableModel(save, this);
    ui->tableView_worstFlight->setModel(worstFlightModel);

    generalClassificationModel = new GeneralClassificationTableModel(this);
    ui->tableView_generalClassification->setModel(generalClassificationModel);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, [this](int index){
        if(index == 0)
        {
            ui->spinBox_recordsCount->hide();
            ui->label_recordsCount->hide();
        }
        else if(index == 1)
        {
            ui->spinBox_recordsCount->show();
            ui->label_recordsCount->show();
        }
    });
    emit ui->tabWidget->currentChanged(0);
    connect(ui->spinBox_recordsCount, &QSpinBox::editingFinished, this, &SimulationRatingsWindow::fillWindow);

    connect(ui->checkBox_showHidden, &QCheckBox::stateChanged, this, [this](int state){
        if(state == 0)
        {
            ui->label_takeoff->hide();
            ui->label_flight->hide();
            ui->label_avgTakeoff->hide();
            ui->label_avgFlight->hide();
            ui->tableView_avgTakeoffRating->hide();
            ui->tableView_avgFlightRating->hide();
            ui->tableView_bestTakeoff->hide();
            ui->tableView_bestFlight->hide();
            ui->tableView_worstTakeoff->hide();
            ui->tableView_worstFlight->hide();
        }
        else if(state == 2)
        {
            ui->label_takeoff->show();
            ui->label_flight->show();
            ui->label_avgTakeoff->show();
            ui->label_avgFlight->show();
            ui->tableView_avgTakeoffRating->show();
            ui->tableView_avgFlightRating->show();
            ui->tableView_bestTakeoff->show();
            ui->tableView_bestFlight->show();
            ui->tableView_worstTakeoff->show();
            ui->tableView_worstFlight->show();
        }
    });
    emit ui->checkBox_showHidden->stateChanged(0);

    int i=1;
    for(auto & preset : GlobalDatabase::get()->getEditableGlobalPointsForPlacesPresets()){
        ui->comboBox_preset->addItem(QString::number(i) + ". " + preset.getName());
        i++;
    }

    connect(ui->comboBox_preset, &QComboBox::currentIndexChanged, this, &SimulationRatingsWindow::fillWindow);
}

SimulationRatingsWindow::~SimulationRatingsWindow()
{
    delete ui;
}

bool rankingCompForPositions(const QPair<Jumper *, double> p1, const QPair<Jumper *, double> p2)
{
    if(p1.second == 0)
        return false;
    else if(p2.second == 0)
        return true;
    if(p1.second == p2.second)
        return p1.first->getID() < p2.first->getID();
    return p1.second < p2.second;
}

bool rankingComp(const QPair<Jumper *, double> p1, const QPair<Jumper *, double> p2)
{
    if(p1.second == p2.second)
        return p1.first->getID() > p2.first->getID();
    return p1.second > p2.second;
}

bool descendingCompForRecords(const QPair<JumpData *, double> p1, const QPair<JumpData *, double> p2)
{
    if(p1.second == p2.second)
        return p1.first->getID() > p2.first->getID();
    return p1.second > p2.second;
}
bool ascendingCompForRecords(const QPair<JumpData *, double> p1, const QPair<JumpData *, double> p2)
{
    if(p1.second == p2.second)
        return p1.first->getID() < p2.first->getID();
    return p1.second < p2.second;
}

void SimulationRatingsWindow::fillWindow()
{
    QVector<CompetitionInfo *> competitions = CompetitionInfo::getCompetitionsByStartAndEnd(CompetitionInfo::mergeSeasonsCompetitions(rangeComboBoxes->getSeasonsList()),
                                                                                            rangeComboBoxes->getCompetition(1), rangeComboBoxes->getCompetition(2));
    jumpersSingleResults = CompetitionSingleResult::getJumpersFilteredSingleResults(filteredJumpers, competitions,
                                                                                    serieTypesCheckBoxes->getSerieTypes(), hillTypesCheckBoxes->getHillTypesSet(), classificationsCheckBoxes->getClassifications(), classificationsCheckBoxes->allUnchecked());

    //Średnia pozycja
    QVector<QPair<Jumper *, double>> avgPositionsRanking;
    for(auto & jumper : jumpersSingleResults.keys())
    {
        double avgPosition = 0;
        for(auto & result : jumpersSingleResults.value(jumper))
        {
            avgPosition += result->getPosition();
        }
        avgPosition /= jumpersSingleResults.value(jumper).count();
        avgPosition = roundDoubleToTwoPlaces(avgPosition);
        if(jumpersSingleResults.value(jumper).count() == 0)
            avgPosition = 0;
        avgPositionsRanking.push_back(qMakePair(jumper, avgPosition));
    }
    std::sort(avgPositionsRanking.begin(), avgPositionsRanking.end(), &rankingCompForPositions);
    averagePositionModel->setRanking(avgPositionsRanking);
    ui->tableView_avgPosition->setModel(nullptr);
    ui->tableView_avgPosition->setModel(averagePositionModel);
    ui->tableView_avgPosition->resizeColumnsToContents();

    //Średnie noty za styl
    QVector<QPair<Jumper *, double>> avgJudgesRanking;
    for(auto & jumper : jumpersSingleResults.keys())
    {
        int count = 0;
        double avgJudges = 0;
        for(auto & result : jumpersSingleResults.value(jumper))
        {
            for(auto & jump : result->getJumpsReference())
            {
                if(jump.getDSQ() == false && result->getCompetition()->getRulesPointer()->getHasJudgesPoints())
                {
                    for(auto & jg : jump.getJudges())
                    {
                        avgJudges += jg;
                        count++;
                    }
                }
            }
        }
        avgJudges /= count;
        avgJudges = roundDoubleToTwoPlaces(avgJudges);
        if(jumpersSingleResults.value(jumper).count() == 0)
            avgJudges = 0;
        avgJudgesRanking.push_back(qMakePair(jumper, avgJudges));
    }
    std::sort(avgJudgesRanking.begin(), avgJudgesRanking.end(), &rankingComp);
    averageJudgesModel->setRanking(avgJudgesRanking);
    ui->tableView_avgJudges->setModel(nullptr);
    ui->tableView_avgJudges->setModel(averageJudgesModel);
    ui->tableView_avgJudges->resizeColumnsToContents();

    //Średni wiatr
    QVector<QPair<Jumper *, double>> avgWindRanking;
    for(auto & jumper : jumpersSingleResults.keys())
    {
        int count = 0;
        double avgWind = 0;
        for(auto & result : jumpersSingleResults.value(jumper))
        {
            for(auto & jump : result->getJumpsReference())
            {
                if(jump.getDSQ() == false && result->getCompetition()->getRulesPointer()->getHasWindCompensations())
                {
                    avgWind += jump.getAveragedWind();
                    count++;
                }
            }
        }
        avgWind /= count;
        avgWind = roundDoubleToTwoPlaces(avgWind);
        if(jumpersSingleResults.value(jumper).count() == 0)
            avgWind = 0;
        avgWindRanking.push_back(qMakePair(jumper, avgWind));
    }
    std::sort(avgWindRanking.begin(), avgWindRanking.end(), &rankingComp);
    averageWindModel->setRanking(avgWindRanking);
    ui->tableView_avgWind->setModel(nullptr);
    ui->tableView_avgWind->setModel(averageWindModel);
    ui->tableView_avgWind->resizeColumnsToContents();

    //Ocena wybicia
    QVector<QPair<Jumper *, double>> avgTakeoffRatingRanking;
    for(auto & jumper : jumpersSingleResults.keys())
    {
        int count = 0;
        double avgRating = 0;
        for(auto & result : jumpersSingleResults.value(jumper))
        {
            for(auto & jump : result->getJumpsReference())
            {
                if(jump.getDSQ() == false)
                {
                    avgRating += jump.getSimulationDataReference().getTakeoffRating();
                    count++;
                }
            }
        }
        avgRating /= count;
        avgRating = roundDoubleToTwoPlaces(avgRating);
        if(jumpersSingleResults.value(jumper).count() == 0)
            avgRating = 0;
        avgTakeoffRatingRanking.push_back(qMakePair(jumper, avgRating));
    }
    std::sort(avgTakeoffRatingRanking.begin(), avgTakeoffRatingRanking.end(), &rankingComp);
    averageTakeoffRatingModel->setRanking(avgTakeoffRatingRanking);
    ui->tableView_avgTakeoffRating->setModel(nullptr);
    ui->tableView_avgTakeoffRating->setModel(averageTakeoffRatingModel);
    ui->tableView_avgTakeoffRating->resizeColumnsToContents();

    //Ocena lotu
    QVector<QPair<Jumper *, double>> avgFlightRatingRanking;
    for(auto & jumper : jumpersSingleResults.keys())
    {
        int count = 0;
        double avgRating = 0;
        for(auto & result : jumpersSingleResults.value(jumper))
        {
            for(auto & jump : result->getJumpsReference())
            {
                if(jump.getDSQ() == false)
                {
                    avgRating += jump.getSimulationDataReference().getFlightRating();
                    count++;
                }
            }
        }
        avgRating /= count;
        avgRating = roundDoubleToTwoPlaces(avgRating);
        if(jumpersSingleResults.value(jumper).count() == 0)
            avgRating = 0;
        avgFlightRatingRanking.push_back(qMakePair(jumper, avgRating));
    }
    std::sort(avgFlightRatingRanking.begin(), avgFlightRatingRanking.end(), &rankingComp);
    averageFlightRatingModel->setRanking(avgFlightRatingRanking);
    ui->tableView_avgFlightRating->setModel(nullptr);
    ui->tableView_avgFlightRating->setModel(averageFlightRatingModel);
    ui->tableView_avgFlightRating->resizeColumnsToContents();

    //Odległosść
    QVector<QPair<Jumper *, double>> avgDistanceRanking;
    for(auto & jumper : jumpersSingleResults.keys())
    {
        int count = 0;
        double avgDistance = 0;
        for(auto & result : jumpersSingleResults.value(jumper))
        {
            for(auto & jump : result->getJumpsReference())
            {
                if(jump.getDSQ() == false)
                {
                    avgDistance += jump.getDistance();
                    count++;
                }
            }
        }
        avgDistance /= count;
        avgDistance = roundDoubleToTwoPlaces(avgDistance);
        if(jumpersSingleResults.value(jumper).count() == 0)
            avgDistance = 0;
        avgDistanceRanking.push_back(qMakePair(jumper, avgDistance));
    }
    std::sort(avgDistanceRanking.begin(), avgDistanceRanking.end(), &rankingComp);
    averageDistanceModel->setRanking(avgDistanceRanking);
    ui->tableView_avgDistance->setModel(nullptr);
    ui->tableView_avgDistance->setModel(averageDistanceModel);
    ui->tableView_avgDistance->resizeColumnsToContents();

    //-------------------REKORDY---------------------
    //Odległość
    //NAJLEPSZA
    int recordsMaxSize = ui->spinBox_recordsCount->value();
    QVector<QPair<JumpData *, double>> bestDistanceRecords;
    QVector<QPair<JumpData *, double>> worstDistanceRecords;
    QVector<QPair<JumpData *, double>> bestPointsRecords;
    QVector<QPair<JumpData *, double>> worstPointsRecords;
    QVector<QPair<JumpData *, double>> bestJudgesRecords;
    QVector<QPair<JumpData *, double>> worstJudgesRecords;
    QVector<QPair<JumpData *, double>> bestWindRecords;
    QVector<QPair<JumpData *, double>> worstWindRecords;
    QVector<QPair<JumpData *, double>> bestTakeoffRecords;
    QVector<QPair<JumpData *, double>> worstTakeoffRecords;
    QVector<QPair<JumpData *, double>> bestFlightRecords;
    QVector<QPair<JumpData *, double>> worstFlightRecords;

    for(auto & jumper : jumpersSingleResults.keys())
        for(auto & result : jumpersSingleResults.value(jumper))
            for(auto & jump : result->getJumpsReference())
                if(jump.getDSQ() == false)
                {
                    //----------ODLEGŁOŚĆ----------
                    //najlepsze
                    double val = jump.getDistance();
                    if(bestDistanceRecords.size() < recordsMaxSize)
                        bestDistanceRecords.push_back(qMakePair(&jump, val));
                    if(val > bestDistanceRecords.last().second)
                        bestDistanceRecords[bestDistanceRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(bestDistanceRecords.begin(), bestDistanceRecords.end(), &descendingCompForRecords);
                    //najgorsze
                    if(worstDistanceRecords.size() < recordsMaxSize)
                        worstDistanceRecords.push_back(qMakePair(&jump, val));
                    if(val < worstDistanceRecords.last().second)
                        worstDistanceRecords[worstDistanceRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(worstDistanceRecords.begin(), worstDistanceRecords.end(), &ascendingCompForRecords);

                    //----------PUNKTY----------
                    //najlepsze
                    val = jump.getPoints();
                    if(bestPointsRecords.size() < recordsMaxSize)
                        bestPointsRecords.push_back(qMakePair(&jump, val));
                    if(val > bestPointsRecords.last().second)
                        bestPointsRecords[bestPointsRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(bestPointsRecords.begin(), bestPointsRecords.end(), &descendingCompForRecords);
                    //najgorsze
                    if(worstPointsRecords.size() < recordsMaxSize)
                        worstPointsRecords.push_back(qMakePair(&jump, val));
                    if(val < worstPointsRecords.last().second)
                        worstPointsRecords[worstPointsRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(worstPointsRecords.begin(), worstPointsRecords.end(), &ascendingCompForRecords);


                    //----------PUNKTY OD SĘDZIÓW----------
                    //najlepsze
                    if(jump.getCompetition()->getRulesPointer()->getHasJudgesPoints()){
                        val = jump.getJudgesPoints();
                        if(bestJudgesRecords.size() < recordsMaxSize)
                            bestJudgesRecords.push_back(qMakePair(&jump, val));
                        if(val > bestJudgesRecords.last().second)
                            bestJudgesRecords[bestJudgesRecords.count() - 1] = qMakePair(&jump, val);
                        std::sort(bestJudgesRecords.begin(), bestJudgesRecords.end(), &descendingCompForRecords);
                        //najgorsze
                        if(worstJudgesRecords.size() < recordsMaxSize)
                            worstJudgesRecords.push_back(qMakePair(&jump, val));
                        if(val < worstJudgesRecords.last().second)
                            worstJudgesRecords[worstJudgesRecords.count() - 1] = qMakePair(&jump, val);
                        std::sort(worstJudgesRecords.begin(), worstJudgesRecords.end(), &ascendingCompForRecords);
                    }

                    //----------UŚREDNIONY WIATR----------
                    //najlepsze
                    val = jump.getAveragedWind();
                    if(bestWindRecords.size() < recordsMaxSize)
                        bestWindRecords.push_back(qMakePair(&jump, val));
                    if(val > bestWindRecords.last().second)
                        bestWindRecords[bestWindRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(bestWindRecords.begin(), bestWindRecords.end(), &descendingCompForRecords);
                    //najgorsze
                    if(worstWindRecords.size() < recordsMaxSize)
                        worstWindRecords.push_back(qMakePair(&jump, val));
                    if(val < worstWindRecords.last().second)
                        worstWindRecords[worstWindRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(worstWindRecords.begin(), worstWindRecords.end(), &ascendingCompForRecords);

                    //----------OCENA WYBICIA----------
                    //najlepsze
                    val = jump.getSimulationDataReference().getTakeoffRating();
                    if(bestTakeoffRecords.size() < recordsMaxSize)
                        bestTakeoffRecords.push_back(qMakePair(&jump, val));
                    if(val > bestTakeoffRecords.last().second)
                        bestTakeoffRecords[bestTakeoffRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(bestTakeoffRecords.begin(), bestTakeoffRecords.end(), &descendingCompForRecords);
                    //najgorsze
                    if(worstTakeoffRecords.size() < recordsMaxSize)
                        worstTakeoffRecords.push_back(qMakePair(&jump, val));
                    if(val < worstTakeoffRecords.last().second)
                        worstTakeoffRecords[worstTakeoffRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(worstTakeoffRecords.begin(), worstTakeoffRecords.end(), &ascendingCompForRecords);

                    //----------OCENA LOTU----------
                    //najlepsze
                    val = jump.getSimulationDataReference().getFlightRating();
                    if(bestFlightRecords.size() < recordsMaxSize)
                        bestFlightRecords.push_back(qMakePair(&jump, val));
                    if(val > bestFlightRecords.last().second)
                        bestFlightRecords[bestFlightRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(bestFlightRecords.begin(), bestFlightRecords.end(), &descendingCompForRecords);
                    //najgorsze
                    if(worstFlightRecords.size() < recordsMaxSize)
                        worstFlightRecords.push_back(qMakePair(&jump, val));
                    if(val < worstFlightRecords.last().second)
                        worstFlightRecords[worstFlightRecords.count() - 1] = qMakePair(&jump, val);
                    std::sort(worstFlightRecords.begin(), worstFlightRecords.end(), &ascendingCompForRecords);
                }


    bestDistanceModel->setRecords(bestDistanceRecords);
    ui->tableView_bestDistance->setModel(nullptr);
    ui->tableView_bestDistance->setModel(bestDistanceModel);
    ui->tableView_bestDistance->resizeColumnsToContents();
    worstDistanceModel->setRecords(worstDistanceRecords);
    ui->tableView_worstDistance->setModel(nullptr);
    ui->tableView_worstDistance->setModel(worstDistanceModel);
    ui->tableView_worstDistance->resizeColumnsToContents();

    bestPointsModel->setRecords(bestPointsRecords);
    ui->tableView_bestPoints->setModel(nullptr);
    ui->tableView_bestPoints->setModel(bestPointsModel);
    ui->tableView_bestPoints->resizeColumnsToContents();
    worstPointsModel->setRecords(worstPointsRecords);
    ui->tableView_worstPoints->setModel(nullptr);
    ui->tableView_worstPoints->setModel(worstPointsModel);
    ui->tableView_worstPoints->resizeColumnsToContents();

    bestJudgesModel->setRecords(bestJudgesRecords);
    ui->tableView_bestJudges->setModel(nullptr);
    ui->tableView_bestJudges->setModel(bestJudgesModel);
    ui->tableView_bestJudges->resizeColumnsToContents();
    worstJudgesModel->setRecords(worstJudgesRecords);
    ui->tableView_worstJudges->setModel(nullptr);
    ui->tableView_worstJudges->setModel(worstJudgesModel);
    ui->tableView_worstJudges->resizeColumnsToContents();

    bestWindModel->setRecords(bestWindRecords);
    ui->tableView_bestWind->setModel(nullptr);
    ui->tableView_bestWind->setModel(bestWindModel);
    ui->tableView_bestWind->resizeColumnsToContents();
    worstWindModel->setRecords(worstWindRecords);
    ui->tableView_worstWind->setModel(nullptr);
    ui->tableView_worstWind->setModel(worstWindModel);
    ui->tableView_worstWind->resizeColumnsToContents();

    bestTakeoffModel->setRecords(bestTakeoffRecords);
    ui->tableView_bestTakeoff->setModel(nullptr);
    ui->tableView_bestTakeoff->setModel(bestTakeoffModel);
    ui->tableView_bestTakeoff->resizeColumnsToContents();
    worstTakeoffModel->setRecords(worstTakeoffRecords);
    ui->tableView_worstTakeoff->setModel(nullptr);
    ui->tableView_worstTakeoff->setModel(worstTakeoffModel);
    ui->tableView_worstTakeoff->resizeColumnsToContents();

    bestFlightModel->setRecords(bestFlightRecords);
    ui->tableView_bestFlight->setModel(nullptr);
    ui->tableView_bestFlight->setModel(bestFlightModel);
    ui->tableView_bestFlight->resizeColumnsToContents();
    worstFlightModel->setRecords(worstFlightRecords);
    ui->tableView_worstFlight->setModel(nullptr);
    ui->tableView_worstFlight->setModel(worstFlightModel);
    ui->tableView_worstFlight->resizeColumnsToContents();

    //-------Fakty-------//
    //Rozegrane konkursy
    int jumpsCount = 0;
    int jumpsK = 0;
    int jumpsHS = 0;
    double averageDistance = 0;
    QHash<Jumper *, int> winsStat;
    QHash<Jumper *, int> podiumsStat;
    QSet<CompetitionInfo *> comps;
    int telemarks = 0;
    int bothLegs = 0;
    int support = 0;
    int fall = 0;
    CompetitionInfo * mostDifferenceBetweenWinnerCompetition = nullptr;
    CompetitionInfo * worstDifferenceBetweenWinnerCompetition = nullptr;
    double mostDifferenceBetweenWinner = 0;
    double worstDifferenceBetweenWinner = 100000000000;
    CompetitionInfo * mostDifferenceBetween10Competition = nullptr;
    CompetitionInfo * worstDifferenceBetween10Competition = nullptr;
    double mostDifferenceBetween10 = 0;
    double worstDifferenceBetween10 = 100000000000;
    double averageJudges = 0;
    int judgesCount = 0;
    double averageWind = 0;
    for(auto & key : jumpersSingleResults.keys())
    {
        for(auto & res : jumpersSingleResults.value(key))
        {
            if(res->getPosition() == 1)
                winsStat[res->getJumper()] += 1;
            if(res->getPosition() <= 3)
                podiumsStat[res->getJumper()] += 1;
            for(auto & jump : res->getJumpsReference())
            {
                if(jump.getDSQ() == false)
                {
                    if(jump.getDistance() >= jump.getHill()->getKPoint())
                        jumpsK += 1;
                    if(jump.getDistance() >= jump.getHill()->getHSPoint())
                        jumpsHS += 1;
                    jumpsCount += 1;
                    averageDistance += jump.getDistance();
                    if(res->getCompetition()->getRulesPointer()->getHasJudgesPoints())
                    {
                        for(auto & jg : jump.getJudges())
                        {
                            averageJudges += jg;
                            judgesCount += 1;
                        }
                    }
                    averageWind += jump.getAveragedWind();
                    switch(jump.getLanding().getType())
                    {
                    case Landing::TelemarkLanding:
                        telemarks++;
                        break;
                    case Landing::BothLegsLanding:
                        bothLegs++;
                        break;
                    case Landing::SupportLanding:
                        support++;
                        break;
                    case Landing::Fall:
                        fall++;
                        break;
                    }
                }
            }
            if(res->getCompetition()->getCancelled() == false)
                comps.insert(res->getCompetition());
        }
    }
    averageDistance /= jumpsCount;
    averageJudges /= judgesCount;
    averageWind /= jumpsCount;
    QString text = QString::number(comps.size()) + " (";
    int seriesCount = 0;
    for(auto & comp : comps)
    {
        CompetitionResults * results = &comp->getResultsReference();
        if(results->getResultsReference().count() >= 2)
        {
            double diff = results->getResultsReference().first().getPointsSum() - results->getResultsReference()[1].getPointsSum();
            if(diff > mostDifferenceBetweenWinner){
                mostDifferenceBetweenWinner = diff;
                mostDifferenceBetweenWinnerCompetition = comp;
            }
            if(diff < worstDifferenceBetweenWinner){
                worstDifferenceBetweenWinner = diff;
                worstDifferenceBetweenWinnerCompetition = comp;
            }
        }
        if(results->getResultsReference().count() >= 10)
        {
            double diff = results->getResultsReference().first().getPointsSum() - results->getResultsReference()[9].getPointsSum();
            if(diff > mostDifferenceBetween10){
                mostDifferenceBetween10 = diff;
                mostDifferenceBetween10Competition = comp;
            }
            if(diff < worstDifferenceBetween10){
                worstDifferenceBetween10 = diff;
                worstDifferenceBetween10Competition = comp;
            }
        }
        seriesCount += comp->getRulesPointer()->getRoundsReference().count();
    }
    text += QString::number(seriesCount) + tr(" serii)");
    ui->label_playedCompetitions->setText(text);

    ui->label_jumpsCount->setText(QString::number(jumpsCount));
    ui->label_kpointjumps->setText(QString::number(jumpsK) + " (" + QString::number(double(jumpsK) / double(jumpsCount) * 100)+ "%)");
    ui->label_hsjumps->setText(QString::number(jumpsHS) + " (" + QString::number(double(jumpsHS) / double(jumpsCount) * 100) + "%)");
    ui->label_saveavgdist->setText(QString::number(averageDistance) + "m");

    QList wins = winsStat.values();
    std::sort(wins.begin(), wins.end(), std::greater<int>());
    ui->label_wins->setText(QString::number(wins.first()));
ui->label_winsJumper->setText(winsStat.key(wins.first())->getNameAndSurname() + " (" + winsStat.key(wins.first())->getCountryCode() + ")");

    QList podiums = podiumsStat.values();
    std::sort(podiums.begin(), podiums.end(), std::greater<int>());
    ui->label_podiums->setText(QString::number(podiums.first()));
ui->label_podiumsJumper->setText(podiumsStat.key(podiums.first())->getNameAndSurname() + " (" + podiumsStat.key(podiums.first())->getCountryCode() + ")");

    ui->label_telemarks->setText(QString::number(telemarks) + " (" + QString::number(double(telemarks) / double(jumpsCount) * 100) + "%)");
    ui->label_bothLegs->setText(QString::number(bothLegs) + " (" + QString::number(double(bothLegs) / double(jumpsCount) * 100) + "%)");
    ui->label_support->setText(QString::number(support) + " (" + QString::number(double(support) / double(jumpsCount) * 100) + "%)");
    ui->label_fall->setText(QString::number(fall) + " (" + QString::number(double(fall) / double(jumpsCount) * 100) + "%)");

    ui->label_biggerWin->setText(QString::number(mostDifferenceBetweenWinner) + tr("pkt"));
    for(auto & season : save->getSeasonsReference())
    {
        Hill * hill = mostDifferenceBetweenWinnerCompetition->getHill();
        ui->label_biggerWinJumper->setText("   " + QString::number(season.getSeasonNumber()) + "/" + QString::number(season.getCalendarReference().getCompetitionsReference().indexOf(mostDifferenceBetweenWinnerCompetition) + 1) + ": " + hill->getName() + " HS" + QString::number(hill->getHSPoint()));
    }
    ui->label_smallestWin->setText(QString::number(worstDifferenceBetweenWinner) + tr("pkt"));
    for(auto & season : save->getSeasonsReference())
    {
        Hill * hill = worstDifferenceBetweenWinnerCompetition->getHill();
        ui->label_smallestWinJumper->setText("   " + QString::number(season.getSeasonNumber()) + "/" + QString::number(season.getCalendarReference().getCompetitionsReference().indexOf(worstDifferenceBetweenWinnerCompetition) + 1) + ": " + hill->getName() + " HS" + QString::number(hill->getHSPoint()));
    }

    ui->label_10Largestdifference->setText(QString::number(mostDifferenceBetween10) + tr("pkt"));
    for(auto & season : save->getSeasonsReference())
    {
        Hill * hill = mostDifferenceBetween10Competition->getHill();
        ui->label_10Largestdifference->setText(QString::number(mostDifferenceBetween10) + tr("pkt   ") + QString::number(season.getSeasonNumber()) + "/" + QString::number(season.getCalendarReference().getCompetitionsReference().indexOf(mostDifferenceBetween10Competition) + 1) + ": " + hill->getName() + " HS" + QString::number(hill->getHSPoint()));
    }
    ui->label_10smallestDifference->setText(QString::number(worstDifferenceBetween10) + tr("pkt"));
    for(auto & season : save->getSeasonsReference())
    {
        Hill * hill = worstDifferenceBetween10Competition->getHill();
        ui->label_10smallestDifference->setText(QString::number(worstDifferenceBetween10) + tr("pkt   ") + QString::number(season.getSeasonNumber()) + "/" + QString::number(season.getCalendarReference().getCompetitionsReference().indexOf(worstDifferenceBetween10Competition) + 1) + ": " + hill->getName() + " HS" + QString::number(hill->getHSPoint()));
    }
    ui->label_saveAvgJudges->setText(QString::number(averageJudges) + tr("pkt"));
    ui->label_saveAvgWind->setText(QString::number(averageWind) + tr("m/s"));

    QHash<Jumper *, double> generalClassification;
    QMap<int, double> pointsForPlaces = GlobalDatabase::get()->getEditableGlobalPointsForPlacesPresets()[ui->comboBox_preset->currentIndex()].getPointsForPlacesReference();

    for(auto & jumper : jumpersSingleResults.keys()){
        for(auto & result : jumpersSingleResults.value(jumper)){
            generalClassification[result->getJumper()] = generalClassification.value(result->getJumper()) + pointsForPlaces.value(result->getPosition());
        }
    }
    QVector<QPair<Jumper *, double>> vector;
    /*QHashIterator<Jumper *, double> it(generalClassification);
    while (it.hasNext())
    {
        qDebug()<<it.key()<<" k";
        vector.append(qMakePair(it.key(), it.value()));
    }*/
    for (auto [key, value] : generalClassification.asKeyValueRange()) {
        // Wypisujemy klucz i wartość na konsolę
        qDebug() << key << value;
        vector.append(qMakePair(key, value));
    }
    std::sort(vector.begin(), vector.end(), rankingComp);
    generalClassificationModel->setResults(vector);
    ui->tableView_generalClassification->setModel(nullptr);
    ui->tableView_generalClassification->setModel(generalClassificationModel);
}

void SimulationRatingsWindow::setupConnections()
{
    connect(rangeComboBoxes, &CompetitionsRangeComboBoxesWidget::changed, this, &SimulationRatingsWindow::fillWindow);
    connect(serieTypesCheckBoxes, &SerieTypesComboBoxesWidget::changed, this, &SimulationRatingsWindow::fillWindow);
    connect(classificationsCheckBoxes, &ClassificationsCheckBoxesWidget::changed, this, &SimulationRatingsWindow::fillWindow);
    connect(hillTypesCheckBoxes, &HillTypesCheckBoxesWidget::changed, this, &SimulationRatingsWindow::fillWindow);
}

QCheckBox *SimulationRatingsWindow::getShowFormCheckBox()
{
    return ui->checkBox_showHidden;
}

QVector<Jumper *> SimulationRatingsWindow::getFilteredJumpers() const
{
    return filteredJumpers;
}

void SimulationRatingsWindow::setFilteredJumpers(const QVector<Jumper *> &newFilteredJumpers)
{
    filteredJumpers = newFilteredJumpers;
}

void SimulationRatingsWindow::saveRankingCsv(QString fileName, short rankingType)
{
    QFile file(fileName);
    file.open(QFile::ReadWrite | QFile::Text);
    file.resize(0);
    QString stream;
    stream = tr("Pozycja") + ";" + tr("Zawodnik") + ";" + tr("Wartosc") + ";" + "\n";
    QVector<QPair<Jumper *, double>> * ranking;
    switch(rankingType)
    {
    case AveragePosition:
        ranking = &averagePositionModel->getRankingReference();
        break;
    case AverageJudges:
        ranking = &averageJudgesModel->getRankingReference();
        break;
    case AverageWind:
        ranking = &averageWindModel->getRankingReference();
        break;
    case AverageTakeoffRating:
        ranking = &averageTakeoffRatingModel->getRankingReference();
        break;
    case AverageTakeoffFlight:
        ranking = &averageFlightRatingModel->getRankingReference();
        break;
    case AverageDistance:
        ranking = &averageDistanceModel->getRankingReference();
        break;
    }
    int pos = 1;
    for(auto & pair : *ranking)
    {
        stream += QString::number(pos) + ";" + pair.first->getNameAndSurname() + " (" + pair.first->getCountryCode() + ")" + ";" + QString::number(pair.second) + "\n";
        pos++;
    }
    QString string = stream.replace(".", ",");
    file.resize(0);
    file.write(string.toUtf8());
    file.close();
}

void SimulationRatingsWindow::saveRecordsCsv(QString fileName, short recordType, bool best)
{
    QFile file(fileName);
    file.open(QFile::ReadWrite | QFile::Text);
    file.resize(0);
    QString stream;
    stream = tr("Pozycja") + ";" + tr("Zawodnik") + ";" + tr("Wartosc") + ";" + tr("Konkurs") + "\n";
    QVector<QPair<JumpData *, double>> * records;
    switch(recordType)
    {
    case Distance:
        if(best) records = &bestDistanceModel->getRecordsReference();
        else records = &worstDistanceModel->getRecordsReference();
        break;
    case Points:
        if(best) records = &bestPointsModel->getRecordsReference();
        else records = &worstPointsModel->getRecordsReference();
        break;
    case Judges:
        if(best) records = &bestJudgesModel->getRecordsReference();
        else records = &worstJudgesModel->getRecordsReference();
        break;
    case Wind:
        if(best) records = &bestWindModel->getRecordsReference();
        else records = &worstWindModel->getRecordsReference();
        break;
    case TakeoffRating:
        if(best) records = &bestTakeoffModel->getRecordsReference();
        else records = &worstTakeoffModel->getRecordsReference();
        break;
    case FlightRating:
        if(best) records = &bestFlightModel->getRecordsReference();
        else records = &worstFlightModel->getRecordsReference();
        break;
    }
    int pos = 1;
    for(auto & pair : *records)
    {
        CompetitionInfo * comp = pair.first->getCompetition();
        Season * season = nullptr;
        for(auto & s : save->getSeasonsReference())
            if(s.getCalendarReference().getCompetitionsReference().contains(comp))
                season = &s;
        QString string = QString::number(season->getSeasonNumber()) + "/" + QString::number(season->getCalendarReference().getCompetitionsReference().indexOf(comp) + 1)
                         + " " + comp->getHill()->getName() + " HS" + QString::number(comp->getHill()->getHSPoint()) + tr(" (Runda ")
                         + QString::number(MyFunctions::getIndexOfItemInVector(pair.first->getSingleResult()->getJumpsReference(), pair.first) + 1) + ") " + " (" + comp->getShortSerieTypeText() + ")";
        stream += QString::number(pos) + ";" + pair.first->getJumper()->getNameAndSurname() + " (" + pair.first->getJumper()->getCountryCode() + ")" +";" + QString::number(pair.second) + ";" + string + "\n";
    }
    QString string = stream.replace(".", ",");
    file.resize(0);
    file.write(string.toUtf8());
    file.close();
}

void SimulationRatingsWindow::saveGeneralClassificationCsv(QString fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadWrite | QFile::Text);
    file.resize(0);
    QString stream;
    stream = tr("Pozycja") + ";" + tr("Zawodnik") + ";" + tr("Punkty") + "\n";
    QVector<QPair<Jumper *, double>> * results = &generalClassificationModel->getResultsReference();
    int pos = 1;
    for(auto & pair : *results)
    {
        stream += QString::number(pos) + ";" + pair.first->getNameAndSurname() + " (" + pair.first->getCountryCode() + ")" +";" + QString::number(pair.second) + "\n";
    }
    QString string = stream.replace(".", ",");
    file.resize(0);
    file.write(string.toUtf8());
    file.close();
}

GeneralClassificationTableModel *SimulationRatingsWindow::getGeneralClassificationModel() const
{
    return generalClassificationModel;
}

void SimulationRatingsWindow::setGeneralClassificationModel(GeneralClassificationTableModel *newGeneralClassificationModel)
{
    generalClassificationModel = newGeneralClassificationModel;
}

HillTypesCheckBoxesWidget *SimulationRatingsWindow::getHillTypesCheckBoxes() const
{
    return hillTypesCheckBoxes;
}

SerieTypesComboBoxesWidget *SimulationRatingsWindow::getSerieTypesCheckBoxes() const
{
    return serieTypesCheckBoxes;
}

CompetitionsRangeComboBoxesWidget *SimulationRatingsWindow::getRangeComboBoxes() const
{
    return rangeComboBoxes;
}

ClassificationsCheckBoxesWidget *SimulationRatingsWindow::getClassificationsCheckBoxes() const
{
    return classificationsCheckBoxes;
}

void SimulationRatingsWindow::on_pushButton_jumpersLists_clicked()
{
    EditStartListWithJumpersListsWindow * window = new EditStartListWithJumpersListsWindow(this);
    window->setLists(&save->getJumpersListsReference());
    window->setupWidgets();
    if(window->exec() == QDialog::Accepted)
    {
        filteredJumpers = save->getJumpers();
        for(auto & status : window->constructJumpersListsStatuses())
        {
            if(status.second == JumpersListsListItemWidget::Select)
            {
                for(auto & jumper : status.first->getJumpersReference())
                {
                    if(filteredJumpers.contains(jumper) == false)
                    {
                        filteredJumpers.push_back(jumper);
                    }
                }
            }
            else if(status.second == JumpersListsListItemWidget::Unselect)
            {
                for(auto & jumper : status.first->getJumpersReference())
                {
                    if(filteredJumpers.contains(jumper) == true)
                    {
                        MyFunctions::removeFromVector(filteredJumpers, jumper);
                    }
                }
            }
        }
        fillWindow();
    }
}


void SimulationRatingsWindow::on_pushButton_csvExport_clicked()
{
    bool ok;
    QString folderName = QInputDialog::getText(this, tr("Nazwa folderu"), tr("Podaj nazwę jaką ma mieć folder z zapisanymi statystykami zapisu"), QLineEdit::Normal, "", &ok);
    if(ok == true)
    {
        QDir d("stats/simulation-saves-stats/" + folderName + "/rankings");
        if(!d.exists())
            d.mkpath(".");
        QString text = "stats/simulation-saves-stats/" + folderName + "/rankings/";
        saveRankingCsv(text + "average-position.csv", AveragePosition);
        saveRankingCsv(text + "average-judges.csv", AverageJudges);
        saveRankingCsv(text + "average-wind.csv", AverageWind);
        saveRankingCsv(text + "average-takeoff-rating.csv", AverageTakeoffRating);
        saveRankingCsv(text + "average-flight-rating.csv", AverageTakeoffFlight);
        saveRankingCsv(text + "average-distance.csv", AverageDistance);

        d = QDir("stats/simulation-saves-stats/" + folderName + "/records");
        if(!d.exists())
            d.mkpath(".");
        text = "stats/simulation-saves-stats/" + folderName + "/records/";
        saveRecordsCsv(text + "best-distance.csv", Distance, true);
        saveRecordsCsv(text + "worst-distance.csv", Distance, false);
        saveRecordsCsv(text + "best-points.csv", Points, true);
        saveRecordsCsv(text + "worst-points.csv", Points, false);
        saveRecordsCsv(text + "best-judges.csv", Judges, true);
        saveRecordsCsv(text + "worst-judges.csv", Judges, false);
        saveRecordsCsv(text + "best-wind.csv", Wind, true);
        saveRecordsCsv(text + "worst-wind.csv", Wind, false);
        saveRecordsCsv(text + "best-takeoff-rating.csv", TakeoffRating, true);
        saveRecordsCsv(text + "worst-takeoff-rating.csv", TakeoffRating, false);
        saveRecordsCsv(text + "best-flight-rating.csv", FlightRating, true);
        saveRecordsCsv(text + "worst-flight-rating.csv", FlightRating, false);

        d = QDir("stats/simulation-saves-stats/" + folderName + "/others");
        if(!d.exists())
            d.mkpath(".");
        text = "stats/simulation-saves-stats/" + folderName + "/others/";
        saveGeneralClassificationCsv(text + "general-classification.csv");
    }
}

