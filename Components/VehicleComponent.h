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

		void SerializeWith(TSerialize ser) {
			ser.Value("PassId", PassId, 'eid');
			ser.Value("VecId", VecId, 'eid');
		}

	};

	struct SLeaveParams {
		
		EntityId PassId;
		EntityId VecId;
		
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

  void LoadGeometry();
  void Physicalize();

  //Network
  //Client
  bool ClEnterVehicle(SEnterParams && p, INetChannel *pNetChannel);
  bool ClLeaveVehicle(SLeaveParams && p, INetChannel *pNetChannel);

  //Server
  bool SvEnterVehicle(SEnterParams && p, INetChannel *pNetChannel);
  bool SvLeaveVehicle(SLeaveParams && p, INetChannel *pNetChannel);
  //Network

  void RequestEnter(IEntity *pNewPassenger, IEntity *pVehicle);
  void RequestLeave(IEntity *pPassenger, IEntity *pVehicle);

  SVehicleProperties *GetProperties() { return &sVehicleProperties; }
  string GetItemName() { return GetProperties()->sItemName; }
  EVehicleType GetVehicleType() { return GetProperties()->eVehicleType; }

  int GetMaxPassengers() { return GetProperties()->iMaxPassengers; }


protected:
  SVehicleProperties sVehicleProperties, sPrevVehicleProperties;

  IEntity *pDriver = nullptr;

private:

	bool bHasDriver = false;
	int iCurrentPassengers = 0;

};