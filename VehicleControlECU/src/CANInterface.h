#ifndef CANINTERFACE_H
#define CANINTERFACE_H

#include <QObject>
#include <QMutex>
#include <QTimer>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

// CAN ID from Arduino
#define ARDUINO_SPEED_ID 0x0F6

class CANInterface : public QObject
{
    Q_OBJECT

public:
    explicit CANInterface(QObject *parent = nullptr);
    ~CANInterface();

    bool initialize(const QString &interface = "can0");
    void shutdown();
    bool isConnected() const { return m_isConnected; }

    // Get current speed in km/h (converted from cm/s)
    float getCurrentSpeedKmh() const;
    
    // Get raw speed in cm/s
    float getCurrentSpeedCms() const;

    // Get filtered distance in cm
    float getCurrentDistanceCm() const;

signals:
    void speedDataReceived(float speedCms);
    void distanceDataReceived(float distanceCm);  // NEW: Distance data signal
    void canError(const QString &error);

private slots:
    void receiveCANMessages();

private:
    bool setupCANInterface(const QString &interface);
    void processCANFrame(const struct can_frame &frame);
    float parseSpeedData(const uint8_t *data);
    float parseDistanceData(const uint8_t *data);

    int m_canSocket;
    bool m_isConnected;
    QString m_interfaceName;
    QTimer *m_receiveTimer;

    float m_currentSpeedCms;
    float m_currentDistanceCm;
    mutable QMutex m_dataMutex;
};

#endif // CANINTERFACE_H
