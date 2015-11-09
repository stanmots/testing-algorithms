TEMPLATE = app

QT *= declarative

SOURCES += src/main.cpp \
    src/GraphsAlgorithmsSolver.cpp \
    src/TranslationChanger.cpp \
    src/MainQmlQuick1Loader.cpp \
    src/GraphsOperationsHelper.cpp \
    src/ConnectedTreeMethod.cpp \
    src/GuaranteedPredictionMethod.cpp \
    src/UnguaranteedPredictionMethod.cpp \
    src/PredictionStorage.cpp \
    src/GuaranteedPredictionV20Method.cpp \
    src/FunctionPropertiesHolder.cpp \
    src/AlgorithmPropertiesSingleton.cpp \
    src/ShortestPathMethod.cpp

RESOURCES += TestingAlgorithmsResources.qrc

lupdate_only{
SOURCES = ui/*.qml \
        ui/Components/*.qml \
}

TRANSLATIONS = ts/TranslationFile_ru.ts \

OTHER_FILES += ui/main.qml \
    ui/Components/MainTabView.qml \
    ui/Components/SingleGraphPage.qml \
    ui/Components/PanelsSplitter.qml \
    ui/Components/GraphPropertiesPanel.qml \
    ui/Components/AlgorithmTestPanel.qml \
    ui/Components/CreateGraphTabView.qml \
    ui/Components/GraphsListVisualModel.qml \
    ui/Components/AdjacencyMatrixView.qml \
    ui/Components/AdjacencyMatrixToolBar.qml \
    ui/Components/AdjacencyMatrixWindow.qml \  
    ui/Components/CloseButton.qml \
    ui/Components/TabWidget.qml \
    ui/Components/CustomButton.qml \
    ui/Components/MatrixView.qml \
    ui/Components/DropDownComboBox.qml \
    ui/Components/DeepTestingPage.qml

HEADERS += \
    src/GraphsAlgorithmsSolver.h \
    src/TranslationChanger.h \
    src/MainQmlQuick1Loader.h \
    src/GraphsTypesHelper.h \
    src/GraphsOperationsHelper.h \
    src/ConnectedTreeMethod.h \
    src/GuaranteedPredictionMethod.h \
    src/UnguaranteedPredictionMethod.h \
    src/PredictionStorage.h \
    src/GuaranteedPredictionV20Method.h \
    src/FunctionPropertiesHolder.h \
    src/AlgorithmPropertiesSingleton.h \
    src/ShortestPathMethod.h

INCLUDEPATH += "lib"
