#ifndef GAMEPADHANDLER_H
#define GAMEPADHANDLER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <memory>
#include "../lib/ShanwanGamepad.hpp"

class GamepadHandler : public QObject
{
    Q_OBJECT
    
public:
    explicit GamepadHandler(QObject *parent = nullptr);
    ~GamepadHandler();
    
    bool initialize();
    void start();
    void stop();
    
signals:
    void gearChangeRequested(QString newGear);
    void steeringChanged(float value);    // -1.0 to 1.0
    void throttleChanged(float value);    // -1.0 to 1.0
    
private slots:
    void pollGamepad();
    
private:
    std::unique_ptr<ShanWanGamepad> m_gamepad;
    QTimer* m_pollTimer;
    
    QString m_currentGear;
    bool m_buttonAPressed = false;
    bool m_buttonBPressed = false;
    bool m_buttonXPressed = false;
    bool m_buttonYPressed = false;
    
    void handleGearButtons(const ShanWanGamepadInput& input);
};

#endif // GAMEPADHANDLER_H
