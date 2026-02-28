#include "BatteryMonitor.h"
#include <QDebug>
#include <algorithm>

BatteryMonitor::BatteryMonitor(QObject *parent)
    : QObject(parent)
{
}

BatteryMonitor::~BatteryMonitor()
{
}

bool BatteryMonitor::initialize()
{
    try {
        m_ina219 = std::make_unique<INA219>(1, 0x41);  // I2C bus 1, address 0x41
        qDebug() << "✅ BatteryMonitor initialized (INA219)";
        return true;
    } catch (const std::exception& e) {
        qCritical() << "❌ Failed to initialize BatteryMonitor:" << e.what();
        return false;
    }
}

float BatteryMonitor::getVoltage()
{
    if (!m_ina219) return 0.0f;
    
    try {
        float busVoltage = m_ina219->getBusVoltage();
        float shuntVoltage = m_ina219->getShuntVoltage();
        return busVoltage + shuntVoltage;
    } catch (...) {
        qWarning() << "Failed to read voltage";
        return 0.0f;
    }
}

float BatteryMonitor::getCurrent()
{
    if (!m_ina219) return 0.0f;
    
    try {
        return m_ina219->getCurrent();
    } catch (...) {
        qWarning() << "Failed to read current";
        return 0.0f;
    }
}

uint8_t BatteryMonitor::getPercentage()
{
    float voltage = getVoltage();
    return static_cast<uint8_t>(voltageToPercentage(voltage));
}

float BatteryMonitor::voltageToPercentage(float voltage)
{
    float percentage = ((voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE)) * 100.0f;
    return std::clamp(percentage, 0.0f, 100.0f);
}
