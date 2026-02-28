#include "GamepadHandler.h"
#include <QDebug>

GamepadHandler::GamepadHandler(QObject *parent)
    : QObject(parent)
    , m_currentGear("P")
{
    m_pollTimer = new QTimer(this);
    m_pollTimer->setInterval(20);  // 50Hz polling
    connect(m_pollTimer, &QTimer::timeout, this, &GamepadHandler::pollGamepad);
}

GamepadHandler::~GamepadHandler()
{
    stop();
}

bool GamepadHandler::initialize()
{
    try {
        m_gamepad = std::make_unique<ShanWanGamepad>("/dev/input/js0");
        
        if (!m_gamepad->init()) {
            qCritical() << "❌ Failed to initialize gamepad";
            return false;
        }
        
        qDebug() << "✅ GamepadHandler initialized";
        return true;
    } catch (const std::exception& e) {
        qCritical() << "❌ Gamepad initialization error:" << e.what();
        return false;
    }
}

void GamepadHandler::start()
{
    if (m_gamepad && m_gamepad->isConnected()) {
        m_pollTimer->start();
        qDebug() << "🎮 Gamepad polling started";
    }
}

void GamepadHandler::stop()
{
    m_pollTimer->stop();
}

void GamepadHandler::pollGamepad()
{
    if (!m_gamepad) return;
    
    ShanWanGamepadInput input = m_gamepad->readData();
    
    // Handle gear change buttons
    handleGearButtons(input);
    
    // Emit steering and throttle changes
    float steering = -input.analog_stick_left.x;  // Inverted steering direction
    float throttle = input.analog_stick_right.y * 0.5f;  // Limit to 50% for safety

    emit steeringChanged(steering);
    emit throttleChanged(throttle);
}

void GamepadHandler::handleGearButtons(const ShanWanGamepadInput& input)
{
    QString newGear = m_currentGear;
    
    // Detect button press (rising edge)
    if (input.button_a && !m_buttonAPressed) {
        newGear = "D";  // Drive
    } else if (input.button_b && !m_buttonBPressed) {
        newGear = "N";  // Neutral
    } else if (input.button_x && !m_buttonXPressed) {
        newGear = "P";  // Park
    } else if (input.button_y && !m_buttonYPressed) {
        newGear = "R";  // Reverse
    }
    
    // Update button states
    m_buttonAPressed = input.button_a;
    m_buttonBPressed = input.button_b;
    m_buttonXPressed = input.button_x;
    m_buttonYPressed = input.button_y;
    
    // Emit signal if gear changed
    if (newGear != m_currentGear) {
        m_currentGear = newGear;
        qDebug() << "🎮 Gear change requested:" << newGear;
        emit gearChangeRequested(newGear);
    }
}
