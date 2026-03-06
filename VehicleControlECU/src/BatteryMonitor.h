#ifndef BATTERYMONITOR_H
#define BATTERYMONITOR_H

#include <QObject>
#include <memory>
#include "../lib/Adafruit_INA219.hpp"

class BatteryMonitor : public QObject
{
    Q_OBJECT
    
public:
    explicit BatteryMonitor(QObject *parent = nullptr);
    ~BatteryMonitor();
    
    bool initialize();
    
    float getVoltage();
    float getCurrent();

private:
    std::unique_ptr<INA219> m_ina219;
};

#endif // BATTERYMONITOR_H
