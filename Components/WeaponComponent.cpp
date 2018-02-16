#include "StdAfx.h"
#include "WeaponComponent.h"

void CWeaponComponent::Initialize() {}

uint64 CWeaponComponent::GetEventMask() const {
	return uint64();
}

void CWeaponComponent::ProcessEvent(SEntityEvent & event) {}

void CWeaponComponent::ReflectType(Schematyc::CTypeDesc<CWeaponComponent>& desc) {}
