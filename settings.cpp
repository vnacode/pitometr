#include "settings.h"
#include <QSettings>
#include <QString>

void Settings::loadMainSettings()
{

}

void Settings::SaveSetting(QString settings_file, QString block, QString name, QString value)
{
    QSettings settings(SETTINGS_PATH + settings_file,QSettings::IniFormat);
    settings.beginGroup(block);
    settings.setValue(name,value);
    settings.endGroup();
    settings.sync();
}

QVariant Settings::get_setting(QString settings_file, QString block, QString name)
{
    QSettings settings(SETTINGS_PATH + settings_file,QSettings::IniFormat);
    settings.beginGroup(block);
    return settings.value(name);
}


//////////////
/// \class Settings::get_main_setting
/// \brief Settings::get_main_setting возвращает значение от main блока
//
QVariant Settings::get_main_setting(const QString &block,const QString &name)
{
    QSettings settings(SETTINGS_PATH + "settings.ini",QSettings::IniFormat);
    settings.beginGroup(block);
    if (!settings.value(name).isValid())
        qDebug() << "ERROR with getting settings " << block << name;
    return settings.value(name);
}
