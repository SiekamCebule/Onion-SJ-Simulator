#include "MultipleTrialRoundsEditDialog.h"
#include "ui_MultipleTrialRoundsEditDialog.h"
#include <QComboBox>
#include <QLabel>

MultipleTrialRoundsEditDialog::MultipleTrialRoundsEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultipleTrialRoundsEditDialog)
{
    ui->setupUi(this);
}

MultipleTrialRoundsEditDialog::~MultipleTrialRoundsEditDialog()
{
    delete ui;
}

bool MultipleTrialRoundsEditDialog::getTrialRoundChecked()
{
    return ui->checkBox_trialRound->isChecked();
}

CompetitionRules MultipleTrialRoundsEditDialog::getTrialRoundRules()
{
    return trialRoundRules;
}

void MultipleTrialRoundsEditDialog::on_pushButton_trainingsRules_clicked()
{
    QDialog * dialog = new QDialog(this);
    dialog->setWindowFlags(Qt::Window);
    dialog->setWindowTitle(tr("Zasady serii próbnej"));
    dialog->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    dialog->setLayout(new QVBoxLayout(this));
    dialog->setStyleSheet("background-color: white; color: black;");

    CompetitionRulesEditorWidget * editor = new CompetitionRulesEditorWidget(this);
    editor->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    editor->setCompetitionRules(&trialRoundRules);
    editor->fillCompetitionRulesInputs();

    QWidget * widget = new QWidget(dialog);
    QHBoxLayout * layout = new QHBoxLayout(dialog);
    QLabel * label = new QLabel(dialog);
    label->setText("Zasady: ");
    QFont font("Ubuntu", 10);
    font.setBold(true);
    label->setFont(font);
    QComboBox * comboBox = new QComboBox(dialog);
    comboBox->addItem(tr("BRAK"));
    for(auto & rules : *rulesList){
        comboBox->addItem(rules.getName());
    }
    layout->addWidget(label);
    layout->addWidget(comboBox);
    widget->setLayout(layout);
    connect(comboBox, &QComboBox::currentIndexChanged, this, [this, editor, comboBox](){
        if(comboBox->currentIndex() > 0){
            CompetitionRules * rules = const_cast<CompetitionRules *>(&rulesList->at(comboBox->currentIndex() - 1));
            editor->setCompetitionRules(rules);
            editor->fillCompetitionRulesInputs();
        }
        else
            editor->resetInputs();
    });

    dialog->layout()->addWidget(widget);
    dialog->layout()->addWidget(editor);
    connect(editor, &CompetitionRulesEditorWidget::submitted, dialog, &QDialog::accept);
    if(dialog->exec() == QDialog::Accepted){
        trialRoundRules = editor->getCompetitionRulesFromWidgetInputs();
    }
}

void MultipleTrialRoundsEditDialog::on_pushButton_submit_clicked()
{
    emit submitted();
}

void MultipleTrialRoundsEditDialog::setTrialRoundRules(const CompetitionRules &newTrialRoundRules)
{
    trialRoundRules = newTrialRoundRules;
}

void MultipleTrialRoundsEditDialog::setTrialRound(bool ok)
{
    ui->checkBox_trialRound->setChecked(ok);
}

QVector<CompetitionRules> *MultipleTrialRoundsEditDialog::getRulesList() const
{
    return rulesList;
}

void MultipleTrialRoundsEditDialog::setRulesList(QVector<CompetitionRules> *newRulesList)
{
    rulesList = newRulesList;
}

