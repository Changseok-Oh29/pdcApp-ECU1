#ifndef PIRACERCONTROLLER_H
#define PIRACERCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <memory>
#include "../lib/Adafruit_PCA9685.hpp"
#include "BatteryMonitor.h"
#include "CANInterface.h"

class PiRacerController : public QObject
{
    Q_OBJECT
    
public:
    explicit PiRacerController(QObject *parent = nullptr);
    ~PiRacerController();
    
    bool initialize();
    
    // Vehicle control
    void setGearPosition(const QString& gear);
    void setSteeringPercent(float percent);  // -1.0 to 1.0
    void setThrottlePercent(float percent);  // -1.0 to 1.0
    
    // Vehicle state
    QString getCurrentGear() const { return m_currentGear; }
    uint16_t getCurrentSpeed() const { return m_currentSpeed; }
    uint16_t getCurrentDistance() const { return m_currentDistance; }
    uint16_t getBatteryVoltage() const;  // Returns voltage in mV
    int16_t getBatteryCurrent() const;   // Returns current in mA

signals:
    void gearDistanceChanged(QString newGear, QString oldGear, uint16_t distance);
    void vehicleStateChanged(QString gear, uint16_t speed, uint16_t voltage, int16_t current);

private slots:
    void onSpeedDataReceived(float speedCms);
    void onDistanceDataReceived(float distanceCm);
    
private:
    // Hardware
    std::unique_ptr<PCA9685> m_steeringController;
    std::unique_ptr<PCA9685> m_throttleController;
    std::unique_ptr<BatteryMonitor> m_batteryMonitor;
    std::unique_ptr<CANInterface> m_canInterface;
    
    // PWM Configuration
    const int PWM_RESOLUTION = 12;
    const int PWM_MAX_RAW_VALUE = 4095;  // 2^12 - 1
    const float PWM_FREQ_50HZ = 50.0f;
    const float PWM_WAVELENGTH_50HZ = 0.02f;  // 1/50Hz
    
    // PWM Channels
    const int PWM_STEERING_CHANNEL = 0;
    const int PWM_THROTTLE_CHANNEL_LEFT_MOTOR_IN_1 = 5;
    const int PWM_THROTTLE_CHANNEL_LEFT_MOTOR_IN_2 = 6;
    const int PWM_THROTTLE_CHANNEL_LEFT_MOTOR_IN_PWM = 7;
    const int PWM_THROTTLE_CHANNEL_RIGHT_MOTOR_IN_1 = 1;
    const int PWM_THROTTLE_CHANNEL_RIGHT_MOTOR_IN_2 = 2;
    const int PWM_THROTTLE_CHANNEL_RIGHT_MOTOR_IN_PWM = 0;
    
    // State
    QString m_currentGear;
    uint16_t m_currentSpeed;     // Real speed from CAN in cm/s
    uint16_t m_currentDistance;  // Distance from ultrasonic sensor in cm
    float m_currentThrottle;

    // Periodic state broadcast timer (10Hz)
    QTimer* m_stateTimer;

    // Helper functions
    float get50HzDutyCycleFromPercent(float value);
    void warmUp();
};

#endif // PIRACERCONTROLLER_H
