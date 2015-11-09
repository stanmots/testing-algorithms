#ifndef MAINQMLQUICK1LOADER_H
#define MAINQMLQUICK1LOADER_H

#include <QDeclarativeView>
#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>

#include "TranslationChanger.h"

class MainQmlQuick1Loader : public QDeclarativeView
{
    Q_OBJECT

public:
    explicit MainQmlQuick1Loader(QWidget *parent = 0);
    virtual ~MainQmlQuick1Loader();

    void setMainQmlFile (const QString &file);
    void retranslate();
    void showQmlScene();
    void readSettings();
    void writeSettings();
    void createLangMenu();

    void setMainTranslator(TranslationChanger *translator);

    TranslationChanger *getMainTranslator() const;

public slots:
    void changeLanguageSlot(QAction *action);

protected:
    virtual void closeEvent (QCloseEvent *event);
    void changeEvent(QEvent*);

private:
    bool m_isNeedShowMaximized;

    TranslationChanger *pm_mainTranslationChanger;

    QMenu *pm_localizationMenu;
    QMenuBar *pm_menuBar;
};

#endif // MAINQMLQUICK1LOADER_H
