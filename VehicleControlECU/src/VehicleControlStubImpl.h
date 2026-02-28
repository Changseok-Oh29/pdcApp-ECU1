#ifndef VEHICLECONTROLSTUBIMPL_H
#define VEHICLECONTROLSTUBIMPL_H

#include <CommonAPI/CommonAPI.hpp>
#include <v1/vehiclecontrol/VehicleControlStubDefault.hpp>
#include <QObject>
#include "PiRacerController.h"

using namespace v1::vehiclecontrol;

class VehicleControlStubImpl : public QObject, public VehicleControlStubDefault
{
    Q_OBJECT
    
public:
    VehicleControlStubImpl(PiRacerController* piracerController);
    virtual ~VehicleControlStubImpl();
    
    // Override RPC method from FIDL
    virtual void setGearPosition(const std::shared_ptr<CommonAPI::ClientId> _client,
                                 std::string _gear,
                                 setGearPositionReply_t _reply) override;
    
private slots:
    void onGearDistanceChanged(QString newGear, QString oldGear, uint16_t distance);
    void onVehicleStateChanged(QString gear, uint16_t speed, uint8_t battery);
    
private:
    PiRacerController* m_piracerController;
};

#endif // VEHICLECONTROLSTUBIMPL_H
