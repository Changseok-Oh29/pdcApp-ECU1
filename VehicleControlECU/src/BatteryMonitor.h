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
    uint8_t getPercentage();  // 0-100
    
private:
    std::unique_ptr<INA219> m_ina219;
    
    // 3S LiPo battery (11.1V nominal)
    const float MIN_VOLTAGE = 9.0f;   // 3.0V per cell
    const float MAX_VOLTAGE = 12.6f;  // 4.2V per cell
    
    float voltageToPercentage(float voltage);
};

#endif // BATTERYMONITOR_H
