#include <StdAfx.h>

#include "ZiplineComponent.h"

void CZiplineComponent::Initialize() {}

uint64 CZiplineComponent::GetEventMask() const {
	return uint64();
}

void CZiplineComponent::ProcessEvent(const SEntityEvent & event) {}

void CZiplineComponent::ReflectType(Schematyc::CTypeDesc<CZiplineComponent>& desc) {

	desc.SetGUID("{35720179-A3EA-4B36-B624-9AE49D3A3DDE}"_cry_guid);

}
