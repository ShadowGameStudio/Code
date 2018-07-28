/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : The base play area component

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "Player.h"

class CPlayAreaComponent final : public IEntityComponent {

	struct SDamageParams{

		EntityId Id;

		void SerializeWith(TSerialize ser) {
			ser.Value("Id", Id, 'eid');
		}

	};

	enum TIMER_EVENT {
		Timer_Decrease = 0,
		Timer_DecreaseAfter = 1,
		Timer_Damage = 2
	};

public:

	CPlayAreaComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CPlayAreaComponent>& desc);

	void DamagePlayer(IEntity* pPlayer);

	//Network
//	virtual bool NetSerialize(TSerialize ser, EEntityAspects aspect, uint8 profile, int flags) override;
	virtual NetworkAspectType GetNetSerializeAspectMask() const { return kDecreaseAspect; }

	bool ClDamagePlayer(SDamageParams&& p, INetChannel* pNetChannel);

	//Network

	void DecreasePlayArea();
	void Update(float frameTime);

	IEntity* pPlayerToDamage = nullptr;

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
	bool bTimerDamageSet = false;

private:

	const EEntityAspects kDecreaseAspect = eEA_GameServerA;

};