#include "StdAfx.h"
#include "WeaponsComponent.h"

void CWeaponsComponent::InitializeClass() {



}

uint64 CWeaponsComponent::GetEventMask() const {
	return uint64();
}

void CWeaponsComponent::ProcessEvent(SEntityEvent & event) {}

void CWeaponsComponent::ReflectType(Schematyc::CTypeDesc<CWeaponsComponent>& desc) {}
