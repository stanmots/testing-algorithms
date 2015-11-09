#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QHash>
#include <cmath>
#include <QDebug>

#include "MainQmlQuick1Loader.h"

MainQmlQuick1Loader::MainQmlQuick1Loader(QWidget *parent) : QDeclarativeView(parent),
    pm_menuBar(new QMenuBar(this))
{
    pm_localizationMenu = pm_menuBar->addMenu(tr("Language"));

    readSettings();
    retranslate();
}

MainQmlQuick1Loader::~MainQmlQuick1Loader(){}

void MainQmlQuick1Loader::setMainTranslator(TranslationChanger *translator) {
    pm_mainTranslationChanger = translator;
}

TranslationChanger *MainQmlQuick1Loader::getMainTranslator() const {
    return pm_mainTranslationChanger;
}

void MainQmlQuick1Loader::setMainQmlFile(const QString &file) {

    setSource(file);
}

void MainQmlQuick1Loader::showQmlScene() {
    if(m_isNeedShowMaximized == true) {
        this->showMaximized();
    } else {
        this->show();
    }
}

void MainQmlQuick1Loader::createLangMenu() {

    if(pm_mainTranslationChanger == NULL) {
        qDebug()<<"Language menu cannot be created, because main translator isn't set...";
    } else {

        if(pm_mainTranslationChanger->getInstalledLanguages().size() == 0) {
            qDebug()<<"Language menu cannot be created, because there are no translation files...";
        } else {

            QActionGroup *langGroup = new QActionGroup(this);
            langGroup->setExclusive(true);

            QHashIterator<QString, QString> i(pm_mainTranslationChanger->getInstalledLanguages());
            while (i.hasNext()) {
                i.next();
                QLocale locale = QLocale(i.key());
                QString lang = locale.nativeLanguageName();
                QAction *action = new QAction(lang, this);
                action->setCheckable(true);
                action->setData(i.key());

                if(i.key() == pm_mainTranslationChanger->getCurrentLang()){
                    action->setChecked(true);
                }
                pm_localizationMenu->addAction(action);
                langGroup->addAction(action);
            }

            connect(langGroup, SIGNAL(triggered(QAction *)), this, SLOT(changeLanguageSlot(QAction *)));
        }
    }
}

void MainQmlQuick1Loader::changeLanguageSlot(QAction *action) {

    if(action != NULL)
    {
        // load the language dependant on the action content
        pm_mainTranslationChanger->loadLanguage(action->data().toString());
    }
}

void MainQmlQuick1Loader::closeEvent(QCloseEvent *event) {

    writeSettings();
    event->accept();
}

void MainQmlQuick1Loader::changeEvent(QEvent *event) {
    if(event != NULL) {
        // this event is send if a translator is loaded
        if(event->type() == QEvent::LanguageChange) {
            retranslate();
        }
    }

    QDeclarativeView::changeEvent(event);
}

void MainQmlQuick1Loader::retranslate() {

    setWindowTitle(TranslationChanger::tr("TestGraphs Application", "The main window title"));
    pm_localizationMenu->setTitle(tr("Language"));
}

void MainQmlQuick1Loader::readSettings() {

    QSettings m_settings;
    QDesktopWidget *mainScreen = qApp->desktop();
    QRect availableRect =  mainScreen->availableGeometry();
    QPoint s_mainWindowPos = m_settings.value("mainWindow/posistion", QPoint(0, 0)).toPoint();
    QSize s_mainWindowSize = m_settings.value("mainWindow/size", QSize(availableRect.width(),
                                                                       availableRect.height())).toSize();
    m_isNeedShowMaximized = m_settings.value("mainWindow/isSizeMaximized", true).toBool();

    this->setMinimumWidth(round(availableRect.width()/2));
    this->setMinimumHeight(round(availableRect.height()/2));

    if(m_isNeedShowMaximized == false) {

        //handle size
        quint32 mainWindowWidth = 0, mainWindowHeight = 0;
        if(s_mainWindowSize.width() < this->minimumWidth()) {
            mainWindowWidth = this->minimumWidth();
        }
        else {
            mainWindowWidth = s_mainWindowSize.width();
        }

        if(s_mainWindowSize.height() < this->minimumHeight()) {
            mainWindowHeight = this->minimumHeight();
        }
        else {
            mainWindowHeight = s_mainWindowSize.height();
        }

        resize(mainWindowWidth, mainWindowHeight);

        quint32 mainWindowXPos = 0, mainWindowYPos = 0,
                availableRightX = availableRect.x() + availableRect.width(),
                availableBottomY = availableRect.y() + availableRect.height();

        //handle position
        if(s_mainWindowPos.x() < availableRect.x()) {
            mainWindowXPos = availableRect.x();
        } else if(s_mainWindowPos.x() + mainWindowWidth > availableRightX) {
            mainWindowXPos = availableRightX - mainWindowWidth;
        }
        else {
            mainWindowXPos = s_mainWindowPos.x();
        }
        if(s_mainWindowPos.y() < availableRect.y()) {
            mainWindowYPos = availableRect.y();
        } else if(s_mainWindowPos.y() + mainWindowHeight > availableBottomY) {
            mainWindowYPos = availableBottomY - mainWindowHeight;
        }
        else {
            mainWindowYPos = s_mainWindowPos.y();
        }

        move(mainWindowXPos, mainWindowYPos);
    }
}

void MainQmlQuick1Loader::writeSettings() {

    QSettings m_settings;

    m_settings.beginGroup("mainWindow");
    if(this->isMaximized() == true) {
        m_settings.setValue("isSizeMaximized", true);
    } else {
        m_settings.setValue("isSizeMaximized", false);
        m_settings.setValue("size", this->size());
        m_settings.setValue("posistion", this->pos());
    }
    m_settings.endGroup();
}
