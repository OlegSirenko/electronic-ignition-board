#ifndef CONFIG_PLUGIN_H
#define CONFIG_PLUGIN_H

#include "widget.h"
#include "App/PluginInterface.h"

class ConfigPlugin : public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.uavos.gcs.PluginArson/1.0.1")
    Q_INTERFACES(PluginInterface)
public:
    QObject *createControl() { return new class Widget(); }
    int flags() { return Widget; }
    QString title() { return tr("Plugin Arson (ver 1.0.1)"); }
    QString descr() { return tr("Test Plugin Arson"); }
    QString icon() { return "serial-port"; }
};

#endif // CONFIG_PLUGIN_H

