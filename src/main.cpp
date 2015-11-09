#include <QApplication>
#include <QtDeclarative>
#include <QProgressDialog>

#include "MainQmlQuick1Loader.h"
#include "GraphsAlgorithmsSolver.h"
#include "TranslationChanger.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //application settings
    app.setOrganizationName(QLatin1String("TestSoftware"));
    app.setOrganizationDomain(QLatin1String("testsoft.com"));
    app.setApplicationName(QLatin1String("TestGraphs"));
    app.setApplicationVersion(QLatin1String("1.0"));

    //qml loading
    MainQmlQuick1Loader view;
    TranslationChanger translationChanger;
    view.setMainTranslator(&translationChanger);
    view.createLangMenu();

    QProgressDialog *mainProgressDialog = new QProgressDialog(&view, Qt::FramelessWindowHint);
    mainProgressDialog->setWindowModality(Qt::NonModal);
    mainProgressDialog->setCancelButton(NULL);

    qmlRegisterType<GraphsAlgorithmsSolver>("com.testsoft.graphs", 1, 0, "GraphSolver");
    view.rootContext()->setContextProperty(QLatin1String("mainTranslator"), (QObject *) &translationChanger);
    view.rootContext()->setContextProperty(QLatin1String("mainProgressDialog"), (QObject *) mainProgressDialog);

    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view.setMainQmlFile(QLatin1String("qrc:////MainFilePath/main.qml"));

    view.showQmlScene();

    return app.exec();
}
