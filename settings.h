#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

#include <QObject>
#include <cstring>
#include <QDebug>
#include <QDir>

#define SETTINGS_PATH QDir::currentPath()





/*!
 * \brief The Settings class
 */
class Settings : public QObject
{
    Q_OBJECT
public:

    static Settings* getInstance()
    {
        static Settings instance;
        return &instance;
    }
 static void loadMainSettings();
 static void SaveSetting(QString settings_file = "settings.ini", QString block = "", QString name = "", QString value = "");
 static QVariant get_setting(QString settings_file = "settings.ini", QString block = "", QString name = "");
 static QVariant get_main_setting(const QString &block, const QString &name);

private:
 Settings() {}
 Q_DISABLE_COPY(Settings)
};

#endif // SETTINGS_H
