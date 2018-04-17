#include <StdAfx.h>

#include "ZiplineComponent.h"
#include "Player.h"

void CZiplineComponent::InitializeClass() {}

void CZiplineComponent::ProcessEventClass(const SEntityEvent & event) {}

void CZiplineComponent::ReflectType(Schematyc::CTypeDesc<CZiplineComponent>& desc) {

	desc.SetGUID("{35720179-A3EA-4B36-B624-9AE49D3A3DDE}"_cry_guid);

}

void CZiplineComponent::OnZiplineAttached() {}

void CZiplineComponent::OnZiplineDetached() {}

void CZiplineComponent::StartRaycast() {

	//Get half of the games window width and height
	const float fHalfRenderWidth = static_cast<float>(gEnv->pRenderer->GetWidth()) * 0.5f;
	const float fHalfRenderHeight = static_cast<float>(gEnv->pRenderer->GetHeight()) * 0.5f;

	//Set the range for the ray to travel
	const float fRange = 50.f;

	//Get the positions
	Vec3 cameraCenterNear, cameraCenterFar;
	gEnv->pRenderer->UnProjectFromScreen(fHalfRenderWidth, fHalfRenderHeight, 0, &cameraCenterNear.x, &cameraCenterNear.y, &cameraCenterNear.z);
	gEnv->pRenderer->UnProjectFromScreen(fHalfRenderWidth, fHalfRenderHeight, 1, &cameraCenterFar.x, &cameraCenterFar.y, &cameraCenterFar.z);

	//Set the direction for the ray to travel
	const Vec3 searchDirecton = (cameraCenterFar - cameraCenterNear).GetNormalized() * fRange;

	//Create the array of hits
	std::array<ray_hit, 1> hits;

	//Set the query flags
	const uint32 queryFlags = ent_all;

	//Set the flags to use for the ray
	const uint32 rayFlags = rwi_stop_at_pierceable;

	//Get the hits
	const int numHits = gEnv->pPhysicalWorld->RayWorldIntersection(cameraCenterNear, searchDirecton, queryFlags, rayFlags, hits.data(), hits.max_size());

	//If it hits, continue
	if (numHits > 0) {

		CryLogAlways("Test");

	}

}
