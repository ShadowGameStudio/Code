/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels the vehicle stuff

-------------------------------------------------------------------------------*/

#pragma once

#include "VehicleProperties.h"
#include <CryEntitySystem/IEntityComponent.h>
#include "CryNetwork\Rmi.h"

class CVehicleComponent : public IEntityComponent {

	struct SEnterParams {

		EntityId PassId;
		EntityId VecId;
		uint16 channelId;

		void SerializeWith(TSerialize ser) {
			ser.Value("PassId", PassId, 'eid');
			ser.Value("VecId", VecId, 'eid');
			ser.Value("channelId", channelId, 'ui16');
		}

	};

	struct SLeaveParams {
		
		EntityId PassId;
		EntityId VecId;
		int channelId;
		
		void SerializeWith(TSerialize ser) {
			ser.Value("PassId", PassId, 'eid');
			ser.Value("VecId", VecId, 'eid');
		}
	};

public:
  CVehicleComponent() = default;

  virtual void Initialize() override;
  virtual uint64 GetEventMask() const override;
  virtual void ProcessEvent(const SEntityEvent &event) override;
  static void ReflectType(Schematyc::CTypeDesc<CVehicleComponent> &desc);

  //Network
  //Client

  //Server
  bool SvEnterVehicle(SEnterParams && p, INetChannel *pNetChannel);
  bool SvLeaveVehicle(SLeaveParams && p, INetChannel *pNetChannel);

  void DelegateAuthorityToClient(const EntityId controlledEntity, const uint16 channelId);
  //Network

  void RequestEnter(IEntity *pNewPassenger, IEntity *pVehicle);
  void RequestLeave(IEntity *pPassenger, IEntity *pVehicle);

  SVehicleProperties *GetProperties() { return &sVehicleProperties; }
  string GetVehicleName() { return GetProperties()->sVehicleName; }
  float GetVehicleSpeed() { return GetProperties()->fVehicleSpeed; }
  EVehicleType GetVehicleType() { return GetProperties()->eVehicleType; }

  int GetMaxPassengers() { return GetProperties()->iMaxPassengers; }

  void CreateVehicleName();

  const EEntityAspects kDriveAspect = eEA_GameClientC;

  virtual bool NetSerialize(TSerialize ser, EEntityAspects aspect, uint8 profile, int flags) override;
  virtual NetworkAspectType GetNetSerializeAspectMask() const override { return kDriveAspect | eEA_Physics; };

protected:
  SVehicleProperties sVehicleProperties, sPrevVehicleProperties;

  IEntity *pDriver = nullptr;

private:

	bool bHasDriver = false;
	int iCurrentPassengers = 0;

};