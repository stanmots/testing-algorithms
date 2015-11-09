#ifndef TRANSLATIONCHANGER_H
#define TRANSLATIONCHANGER_H

#include <QObject>
#include <QTranslator>
#include <QApplication>
#include <QHash>

class TranslationChanger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString emptyString READ getEmptyString NOTIFY languageChanged)

public:
    explicit TranslationChanger(QObject *parent = 0);

    QString getEmptyString() {
     return "";
    }

    Q_INVOKABLE void loadLanguage(const QString &language);

    //read-only
    const QHash<QString, QString> &getInstalledLanguages() const;
    const QString &getCurrentLang() const;

signals:
    void languageChanged();

public slots:

private:
    QString m_currentLang;
    QTranslator *pm_translator;
    QHash<QString, QString> m_installedLanguages;
};

#endif // TRANSLATIONCHANGER_H
