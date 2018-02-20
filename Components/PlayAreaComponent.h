/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : The base play area component

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "Player.h"

class CPlayAreaComponent final : public IEntityComponent {

	enum TIMER_EVENT {
		Timer_Decrease = 0,
		Timer_DecreaseAfter = 1
	};

public:

	CPlayAreaComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CPlayAreaComponent>& desc);

	//Network
//	virtual bool NetSerialize(TSerialize ser, EEntityAspects aspect, uint8 profile, int flags) override;
	virtual NetworkAspectType GetNetSerializeAspectMask() const { return kDecreaseAspect; }

	void SpawnPlayArea();
	void DecreasePlayArea();
	void Update(float frameTime);

	int iTimesDecreased = 0;
	int iDecreaseTime = 12000;
	float fScaleX = 20.f;
	float fScaleY = 20.f;
	float fScaleZ = 20.f;
	float fScaleTo = 15.f;
	float fDecreaseAmount = 0.01f;
	bool bIsSpawned = false;
	bool bIsDecreased = true;
	bool bTimerSet = false;

private:

	const EEntityAspects kDecreaseAspect = eEA_GameServerA;

};