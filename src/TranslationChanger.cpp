#include <QLocale>
#include <QDir>
#include <QSettings>
#include <QDebug>

#include "TranslationChanger.h"

TranslationChanger::TranslationChanger(QObject *parent) :
    QObject(parent), pm_translator(new QTranslator(this)),
    m_installedLanguages(QHash<QString, QString>())
{
    //get all available translations
    QString langPath = QApplication::applicationDirPath();
    langPath.append("/translations");
    QDir dir(langPath);
    QStringList fileNames = dir.entryList(QStringList("TranslationFile_*.qm"));

    if(fileNames.size() == 0) {
        qDebug()<< "Cannot find any translation file!";
    } else {
        for (int i = 0; i < fileNames.size(); ++i)
        {
            // get locale extracted by filename
            QString locale;

            //"TranslationFile_ru.qm"
            QString translationFile = locale = fileNames[i];
            //"TranslationFile_ru"
            locale.truncate(locale.lastIndexOf('.'));
            //and now local is "ru"
            locale.remove(0, locale.indexOf('_') + 1);

            m_installedLanguages[locale] = langPath + "/" + translationFile;
        }
        m_installedLanguages["en"] = "defaultLanguage";

        //try to restore saved language setting
        QString systemLocale = QLocale::system().name();
        systemLocale.truncate(systemLocale.lastIndexOf('_'));
        QSettings settings;
        QString savedLangSetting = settings.value("mainWindow/language", systemLocale).toString();
        loadLanguage(savedLangSetting);
    }
}

void TranslationChanger::loadLanguage(const QString &language) {

    if(m_installedLanguages.contains(language) == false) {
        qDebug()<<"Translation file for the specified language is not exist!";
    }
    else if(m_currentLang != language) {
        m_currentLang = language;

        if(pm_translator != NULL && pm_translator->isEmpty() == false) {
            //remove old translator
            qApp->removeTranslator(pm_translator);
        }

        QSettings settings;
        settings.setValue("mainWindow/language", language);

        //english is the default language
        if(language != QLatin1String("en")){
            if(pm_translator->load(m_installedLanguages[language]) == true) {
                qApp->installTranslator(pm_translator);
            } else {
                qDebug()<<"Error! Can't load translation file...";
            }
        }

        emit languageChanged();
    }
}

const QHash<QString, QString> &TranslationChanger::getInstalledLanguages() const {

    return m_installedLanguages;
}

const QString &TranslationChanger::getCurrentLang() const {
    return m_currentLang;
}
