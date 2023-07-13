QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
#CONFIG += console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    source/UI/Competition/CompetitionManagerWindow.cpp \
    source/UI/Competition/JumperCompetitionResultsWidget.cpp \
    source/UI/Competition/Results/ResultsTableModel.cpp \
    source/UI/Competition/Results/TeamResultsTreeModel.cpp \
    source/UI/Competition/StartList/StartListModel.cpp \
    source/UI/Competition/TeamsSquadsTreeModel.cpp \
    source/UI/Competition/TeamsSquadsTreeView.cpp \
    source/UI/DatabaseEditor/DatabaseItemsListView.cpp \
    source/UI/DatabaseEditor/ListModels/ClassificationsListModel.cpp \
    source/UI/DatabaseEditor/ListModels/CompetitionRulesListModel.cpp \
    source/UI/DatabaseEditor/ListModels/HillsListModel.cpp \
    source/UI/DatabaseEditor/ListModels/JumpersListModel.cpp \
    source/UI/DatabaseEditor/ListModels/PointsForPlacesPresetsListModel.cpp \
    source/UI/EditorWidgets/ClassificationEditorWidget.cpp \
    source/UI/EditorWidgets/CompetitionRulesEditorWidget.cpp \
    source/UI/EditorWidgets/InrunSnowGeneratorSettingsEditorWidget.cpp \
    source/UI/EditorWidgets/PointsForPlacesPresetEditorWidget.cpp \
    source/UI/EditorWidgets/RoundInfoEditorWidget.cpp \
    source/UI/EditorWidgets/TeamsEditing/Models/PointsForPlacesTableModel.cpp \
    source/UI/EditorWidgets/TeamsEditing/Models/TeamJumpersListModel.cpp \
    source/UI/EditorWidgets/TeamsEditing/TeamEditorWidget.cpp \
    source/UI/EditorWidgets/WindsGeneratorSettingsEditorWidget.cpp \
    source/UI/EditorWidgets/WindsGeneratorSettingsWidgetInputItem.cpp \
    source/UI/JumpManipulation/JumpManipulatorConfigWindow.cpp \
    source/UI/ResultsShowing/JumpDataDetailedInfoWindow.cpp \
    source/UI/Competition/CompetitionConfigWindow.cpp \
    source/UI/ResultsShowing/JumpWindInfoWidget.cpp \
    source/UI/ResultsShowing/JumpWindInfoWidgetItem.cpp \
    source/UI/Seasons/CalendarEditor/CalendarEditorTableModel.cpp \
    source/UI/Seasons/CalendarEditor/CalendarEditorWidget.cpp \
    source/UI/Seasons/CalendarEditor/MultipleTrainingsEditDialog.cpp \
    source/UI/Seasons/CalendarEditor/MultipleTrialRoundsEditDialog.cpp \
    source/UI/Seasons/CompetitionInfoEditorWidget.cpp \
    source/UI/Seasons/ItemModels/SimulationSavesListModel.cpp \
    source/UI/Seasons/NewSeasonConfiguratorWindow.cpp \
    source/UI/Seasons/NewSimulationSaveConfigurationWindow.cpp \
    source/UI/Seasons/SimulationSaveInfoWidget.cpp \
    source/UI/Seasons/SimulationSavesWindow.cpp \
    source/UI/SingleJumps/SingleJumpsConfigWindow.cpp \
    source/UI/AppSettings/AppSettingsWindow.cpp \
    source/UI/DatabaseEditor/DatabaseEditorWindow.cpp \
    source/UI/EditorWidgets/CharacteristicsEditor.cpp \
    source/UI/EditorWidgets/Dialogs/CharacteristicInputDialog.cpp \
    source/UI/EditorWidgets/HillEditorWidget.cpp \
    source/UI/EditorWidgets/JumperEditorWidget.cpp \
    source/UI/SingleJumps/SingleJumpsResultsTableModel.cpp \
    source/UI/SingleJumps/SingleJumpsResultsWindow.cpp \
    source/competitions/AbstractCompetitionManager.cpp \
    source/competitions/CompetitionInfo.cpp \
    source/competitions/CompetitionManagers/TeamCompetitionManager.cpp \
    source/competitions/CompetitionResults.cpp \
    source/competitions/CompetitionRules.cpp \
    source/competitions/CompetitionSingleResult.cpp \
    source/competitions/CompetitionManagers/IndividualCompetitionManager.cpp \
    source/competitions/RoundInfo.cpp \
    source/competitions/StartListCompetitorStatus.cpp \
    source/global/CountryFlagsManager.cpp \
    source/global/GlobalAppSettings.cpp \
    source/global/GlobalDatabase.cpp \
    source/global/GlobalSimulationSettings.cpp \
    source/global/GlobalTranslators.cpp \
    source/global/PointsForPlacesPreset.cpp \
    source/global/SeasonDatabaseObjectsManager.cpp \
    source/global/TreeItem.cpp \
    source/main.cpp \
    source/UI/mainwindow.cpp \
    source/seasons/Season.cpp \
    source/seasons/SeasonCalendar.cpp \
    source/seasons/SeasonSettings.cpp \
    source/seasons/SimulationSave.cpp \
    source/seasons/Classification.cpp \
    source/simulator/Characteristic.cpp \
    source/simulator/Hill.cpp \
    source/simulator/InrunSnowGenerator.cpp \
    source/simulator/JumpData.cpp \
    source/simulator/JumpManipulator.cpp \
    source/simulator/JumpSimulationData.cpp \
    source/simulator/JumpSimulator.cpp \
    source/simulator/Jumper.cpp \
    source/simulator/JumperSkills.cpp \
    source/simulator/Landing.cpp \
    source/simulator/Team.cpp \
    source/simulator/Wind.cpp \
    source/simulator/WindsCalculator.cpp \
    source/simulator/wind-generation/WindGenerationSettings.cpp \
    source/simulator/wind-generation/WindsGenerator.cpp \
    source/single-jumps/SingleJumpsManager.cpp \
    source/utilities/ClassWithCharacteristics.cpp \
    source/utilities/ClassWithID.cpp \
    source/global/IDGenerator.cpp \
    source/global/MyRandom.cpp \
    source/utilities/functions.cpp

HEADERS += \
    source/UI/Competition/CompetitionManagerWindow.h \
    source/UI/Competition/JumperCompetitionResultsWidget.h \
    source/UI/Competition/Results/ResultsTableModel.h \
    source/UI/Competition/Results/TeamResultsTreeModel.h \
    source/UI/Competition/StartList/StartListModel.h \
    source/UI/Competition/TeamsSquadsTreeModel.h \
    source/UI/Competition/TeamsSquadsTreeView.h \
    source/UI/DatabaseEditor/DatabaseItemsListView.h \
    source/UI/DatabaseEditor/ListModels/ClassificationsListModel.h \
    source/UI/DatabaseEditor/ListModels/CompetitionRulesListModel.h \
    source/UI/DatabaseEditor/ListModels/HillsListModel.h \
    source/UI/DatabaseEditor/ListModels/JumpersListModel.h \
    source/UI/DatabaseEditor/ListModels/PointsForPlacesPresetsListModel.h \
    source/UI/EditorWidgets/ClassificationEditorWidget.h \
    source/UI/EditorWidgets/CompetitionRulesEditorWidget.h \
    source/UI/EditorWidgets/InrunSnowGeneratorSettingsEditorWidget.h \
    source/UI/EditorWidgets/PointsForPlacesPresetEditorWidget.h \
    source/UI/EditorWidgets/RoundInfoEditorWidget.h \
    source/UI/EditorWidgets/TeamsEditing/Models/PointsForPlacesTableModel.h \
    source/UI/EditorWidgets/TeamsEditing/Models/TeamJumpersListModel.h \
    source/UI/EditorWidgets/TeamsEditing/TeamEditorWidget.h \
    source/UI/EditorWidgets/WindsGeneratorSettingsEditorWidget.h \
    source/UI/EditorWidgets/WindsGeneratorSettingsWidgetInputItem.h \
    source/UI/JumpManipulation/JumpManipulatorConfigWindow.h \
    source/UI/ResultsShowing/JumpDataDetailedInfoWindow.h \
    source/UI/Competition/CompetitionConfigWindow.h \
    source/UI/ResultsShowing/JumpWindInfoWidget.h \
    source/UI/ResultsShowing/JumpWindInfoWidgetItem.h \
    source/UI/Seasons/CalendarEditor/CalendarEditorTableModel.h \
    source/UI/Seasons/CalendarEditor/CalendarEditorWidget.h \
    source/UI/Seasons/CalendarEditor/MultipleTrainingsEditDialog.h \
    source/UI/Seasons/CalendarEditor/MultipleTrialRoundsEditDialog.h \
    source/UI/Seasons/CompetitionInfoEditorWidget.h \
    source/UI/Seasons/ItemModels/SimulationSavesListModel.h \
    source/UI/Seasons/NewSeasonConfiguratorWindow.h \
    source/UI/Seasons/NewSimulationSaveConfigurationWindow.h \
    source/UI/Seasons/SimulationSaveInfoWidget.h \
    source/UI/Seasons/SimulationSavesWindow.h \
    source/UI/SingleJumps/SingleJumpsConfigWindow.h \
    source/UI/AppSettings/AppSettingsWindow.h \
    source/UI/DatabaseEditor/DatabaseEditorWindow.h \
    source/UI/EditorWidgets/CharacteristicsEditor.h \
    source/UI/EditorWidgets/Dialogs/CharacteristicInputDialog.h \
    source/UI/EditorWidgets/HillEditorWidget.h \
    source/UI/EditorWidgets/JumperEditorWidget.h \
    source/UI/SingleJumps/SingleJumpsResultsTableModel.h \
    source/UI/SingleJumps/SingleJumpsResultsWindow.h \
    source/UI/mainwindow.h \
    source/competitions/AbstractCompetitionManager.h \
    source/competitions/CompetitionInfo.h \
    source/competitions/CompetitionManagers/TeamCompetitionManager.h \
    source/competitions/CompetitionResults.h \
    source/competitions/CompetitionRules.h \
    source/competitions/CompetitionSingleResult.h \
    source/competitions/CompetitionManagers/IndividualCompetitionManager.h \
    source/competitions/RoundInfo.h \
    source/competitions/StartListCompetitorStatus.h \
    source/global/CountryFlagsManager.h \
    source/global/GlobalAppSettings.h \
    source/global/GlobalDatabase.h \
    source/global/GlobalSimulationSettings.h \
    source/global/GlobalTranslators.h \
    source/global/PointsForPlacesPreset.h \
    source/global/SeasonDatabaseObjectsManager.h \
    source/global/TreeItem.h \
    source/seasons/Season.h \
    source/seasons/SeasonCalendar.h \
    source/seasons/SeasonSettings.h \
    source/seasons/SimulationSave.h \
    source/seasons/Classification.h \
    source/simulator/Characteristic.h \
    source/simulator/Hill.h \
    source/simulator/InrunSnowGenerator.h \
    source/simulator/JumpData.h \
    source/simulator/JumpManipulator.h \
    source/simulator/JumpSimulationData.h \
    source/simulator/JumpSimulator.h \
    source/simulator/Jumper.h \
    source/simulator/JumperSkills.h \
    source/simulator/Landing.h \
    source/simulator/Team.h \
    source/simulator/Wind.h \
    source/simulator/WindsCalculator.h \
    source/simulator/wind-generation/WindGenerationSettings.h \
    source/simulator/wind-generation/WindsGenerator.h \
    source/single-jumps/SingleJumpsManager.h \
    source/utilities/ClassWithCharacteristics.h \
    source/utilities/ClassWithID.h \
    source/global/IDGenerator.h \
    source/global/MyRandom.h \
    source/utilities/functions.h

FORMS += \
    source/UI/Competition/CompetitionManagerWindow.ui \
    source/UI/Competition/JumperCompetitionResultsWidget.ui \
    source/UI/Competition/TeamsSquadsTreeView.ui \
    source/UI/DatabaseEditor/DatabaseItemsListView.ui \
    source/UI/EditorWidgets/ClassificationEditorWidget.ui \
    source/UI/EditorWidgets/CompetitionRulesEditorWidget.ui \
    source/UI/EditorWidgets/InrunSnowGeneratorSettingsEditorWidget.ui \
    source/UI/EditorWidgets/PointsForPlacesPresetEditorWidget.ui \
    source/UI/EditorWidgets/RoundInfoEditorWidget.ui \
    source/UI/EditorWidgets/TeamsEditing/TeamEditorWidget.ui \
    source/UI/EditorWidgets/WindsGeneratorSettingsEditorWidget.ui \
    source/UI/EditorWidgets/WindsGeneratorSettingsWidgetInputItem.ui \
    source/UI/JumpManipulation/JumpManipulatorConfigWindow.ui \
    source/UI/ResultsShowing/JumpDataDetailedInfoWindow.ui \
    source/UI/Competition/CompetitionConfigWindow.ui \
    source/UI/ResultsShowing/JumpWindInfoWidget.ui \
    source/UI/ResultsShowing/JumpWindInfoWidgetItem.ui \
    source/UI/Seasons/CalendarEditor/CalendarEditorWidget.ui \
    source/UI/Seasons/CalendarEditor/MultipleTrainingsEditDialog.ui \
    source/UI/Seasons/CalendarEditor/MultipleTrialRoundsEditDialog.ui \
    source/UI/Seasons/CompetitionInfoEditorWidget.ui \
    source/UI/Seasons/NewSeasonConfiguratorWindow.ui \
    source/UI/Seasons/NewSimulationSaveConfigurationWindow.ui \
    source/UI/Seasons/SimulationSaveInfoWidget.ui \
    source/UI/Seasons/SimulationSavesWindow.ui \
    source/UI/SingleJumps/SingleJumpsConfigWindow.ui \
    source/UI/AppSettings/AppSettingsWindow.ui \
    source/UI/DatabaseEditor/DatabaseEditorWindow.ui \
    source/UI/EditorWidgets/CharacteristicsEditor.ui \
    source/UI/EditorWidgets/Dialogs/CharacteristicInputDialog.ui \
    source/UI/EditorWidgets/HillEditorWidget.ui \
    source/UI/EditorWidgets/JumperEditorWidget.ui \
    source/UI/SingleJumps/SingleJumpsResultsWindow.ui \
    source/UI/mainwindow.ui

TRANSLATIONS += \
    translations/translation_en.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#copydata.commands = $(COPY_DIR) $$PWD/translations $$PWD/flags $$OUT_PWD
#copydata.commands = $(COPY_DIR) $$PWD/userData $$PWD/translations $$PWD/flags $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

DISTFILES += \
    flaticon_authors \
    flags_author \
    userData/GlobalDatabase/globalCompetitionsRules.json \
    userData/GlobalDatabase/globalHills.json \
    userData/GlobalDatabase/globalJumpers.json \
    userData/Settings/appSettings.json \
    README.md

RESOURCES += \
assets.qrc
