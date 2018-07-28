#include "StdAfx.h"
#include "Player.h"
#include "ItemComponent.h"
#include "DefaultComponents/Physics/Vehicles/VehicleComponent.h"

#define MOUSE_DELTA_TRESHOLD 0.0001f
#define PICKUP_RANGE 0.5f

void CPlayerComponent::UpdateMovementRequest(float frameTime) {
	
	if (bIsDriver) {

		Vec3 velocity = ZERO;

		if (CVehicleComponent *pVehicle = pPlayerVehicle->GetComponent<CVehicleComponent>()) {

			float fVehicleSpeed = pVehicle->GetVehicleSpeed();

			if (m_inputFlags & (TInputFlags)EInputFlag::MoveLeft) {
				velocity.x -= fVehicleSpeed * frameTime;
			}
			if (m_inputFlags & (TInputFlags)EInputFlag::MoveRight) {
				velocity.x += fVehicleSpeed * frameTime;
			}
			if (m_inputFlags & (TInputFlags)EInputFlag::MoveForward) {
				velocity.y += fVehicleSpeed * frameTime;
			}
			if (m_inputFlags & (TInputFlags)EInputFlag::MoveBack) {
				velocity.y -= fVehicleSpeed * frameTime;
			}

			if (Cry::DefaultComponents::CVehiclePhysicsComponent *pVehiclePhys = pPlayerVehicle->GetComponent<Cry::DefaultComponents::CVehiclePhysicsComponent>()) {

				pVehiclePhys->SetCurrentGear(1);
				pVehiclePhys->SetVelocity(velocity);

			}

		}

	}
	else if (bIsPassenger) {

	}
	else if (!bIsDriver) {

		if (GroundCheck()) {

			Vec3 velocity = ZERO;

			float moveSpeed = 20.5f;
			float sprintSpeed = 200.f;
			const float mainSpeed = 20.5f;
			const float fSprintCostRatio = 1.f;

			if (m_inputFlags & (TInputFlags)EInputFlag::MoveJump) {
				jumpVel = 100.f;
				velocity.z += jumpVel * frameTime;
			}
			if (m_inputFlags & (TInputFlags)EInputFlag::MoveLeft) {
				velocity.x -= mainSpeed * frameTime;
			}
			if (m_inputFlags & (TInputFlags)EInputFlag::MoveRight) {
				velocity.x += mainSpeed * frameTime;
			}
			if (m_inputFlags & (TInputFlags)EInputFlag::MoveForward) {

				if (m_inputFlags & (TInputFlags)EInputFlag::MoveSprint) {
					//Set move speed to sprint speed if shift is pressed	
					moveSpeed = sprintSpeed;
				}

				velocity.y += moveSpeed * frameTime;
			}
			if (m_inputFlags & (TInputFlags)EInputFlag::MoveBack) {
				velocity.y -= mainSpeed * frameTime;

				if (m_inputFlags & (TInputFlags)EInputFlag::MoveForward) {

					m_inputFlags &= ~(TInputFlags)EInputFlag::MoveSprint;
					velocity.y = 0.f;
				}
			}

			m_pCharacterController->AddVelocity(GetEntity()->GetWorldRotation() * velocity);

		}

	}
}

void CPlayerComponent::UpdateLookDirectionRequest(float frameTime) {
	const float rotationSpeed = 0.002f;
	const float rotationLimitsMinPitch = -0.84f;
	const float rotationLimitsMaxPitch = 1.5f;

	// Apply smoothing filter to the mouse input
	m_mouseDeltaRotation = m_mouseDeltaSmoothingFilter.Push(m_mouseDeltaRotation).Get();

	// Update angular velocity metrics
	m_horizontalAngularVelocity = (m_mouseDeltaRotation.x * rotationSpeed) / frameTime;
	m_averagedHorizontalAngularVelocity.Push(m_horizontalAngularVelocity);

	// Start with updating look orientation from the latest input
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

	// Yaw
	ypr.x += m_mouseDeltaRotation.x * rotationSpeed;

	// Pitch
	// TODO: Perform soft clamp here instead of hard wall, should reduce rot speed in this direction when close to limit.
	ypr.y = CLAMP(ypr.y + m_mouseDeltaRotation.y * rotationSpeed, rotationLimitsMinPitch, rotationLimitsMaxPitch);

	// Roll (skip)
	ypr.z = 0;

	m_lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

	// Reset the mouse delta accumulator every frame
	m_mouseDeltaRotation = ZERO;
}

void CPlayerComponent::UpdateAnimation(float frameTime) {
	const float angularVelocityTurningThreshold = 0.174; // [rad/s]

														 // Update tags and motion parameters used for turning
	const bool isTurning = std::abs(m_averagedHorizontalAngularVelocity.Get()) > angularVelocityTurningThreshold;
	m_pAnimationComponent->SetTagWithId(m_rotateTagId, isTurning);
	if (isTurning) {
		// TODO: This is a very rough predictive estimation of eMotionParamID_TurnAngle that could easily be replaced with accurate reactive motion 
		// if we introduced IK look/aim setup to the character's model and decoupled entity's orientation from the look direction derived from mouse input.

		const float turnDuration = 1.0f; // Expect the turning motion to take approximately one second.
		m_pAnimationComponent->SetMotionParameter(eMotionParamID_TurnAngle, m_horizontalAngularVelocity * turnDuration);
	}

	// Update active fragment
	const auto& desiredFragmentId = m_pCharacterController->IsWalking() ? m_walkFragmentId : m_idleFragmentId;
	if (m_activeFragmentId != desiredFragmentId) {
		m_activeFragmentId = desiredFragmentId;
		m_pAnimationComponent->QueueFragmentWithId(m_activeFragmentId);
	}

	// Update entity rotation as the player turns
	// We only want to affect Z-axis rotation, zero pitch and roll
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));
	ypr.y = 0;
	ypr.z = 0;
	const Quat correctedOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

	// Send updated transform to the entity, only orientation changes
	GetEntity()->SetPosRotScale(GetEntity()->GetWorldPos(), correctedOrientation, Vec3(1, 1, 1));
}

void CPlayerComponent::UpdateCamera(float frameTime) {
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

	// Ignore z-axis rotation, that's set by CPlayerAnimations
	ypr.x = 0;

	// Start with changing view rotation to the requested mouse look orientation
	Matrix34 localTransform = IDENTITY;
	localTransform.SetRotation33(CCamera::CreateOrientationYPR(ypr));

	// Offset the player along the forward axis (normally back)
	// Also offset upwards
	const float viewOffsetForward = -1.5f;
	const float viewOffsetUp = 2.f;
	localTransform.SetTranslation(Vec3(0, viewOffsetForward, viewOffsetUp));

	m_pCameraComponent->SetTransformMatrix(localTransform);
}

void CPlayerComponent::UpdateFPCamera(float frameTime) {

	// Start with updating look orientation from the latest input
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

	ypr.x += m_mouseDeltaRotation.x * m_rotationSpeed;

	const float rotationLimitsMinPitch = -0.84f;
	const float rotationLimitsMaxPitch = 1.5f;

	// TODO: Perform soft clamp here instead of hard wall, should reduce rot speed in this direction when close to limit.
	ypr.y = CLAMP(ypr.y + m_mouseDeltaRotation.y * m_rotationSpeed, rotationLimitsMinPitch, rotationLimitsMaxPitch);
	// Skip roll
	ypr.z = 0;

	m_lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));

	// Reset every frame
	m_mouseDeltaRotation = ZERO;

	// Ignore z-axis rotation, that's set by CPlayerAnimations
	ypr.x = 0;

	// Start with changing view rotation to the requested mouse look orientation
	Matrix34 localTransform = IDENTITY;
	localTransform.SetRotation33(CCamera::CreateOrientationYPR(ypr));

	const float viewOffsetForward = 0.2f;
	const float viewOffsetUp = fLeanDown;
	const float viewOffsetSide = fLeanSide;

	if (ICharacterInstance *pCharacter = m_pAnimationComponent->GetCharacter()) {
		// Get the local space orientation of the camera joint
		const QuatT &cameraOrientation = pCharacter->GetISkeletonPose()->GetAbsJointByID(m_cameraJointId);
		// Apply the offset to the camera
		localTransform.SetTranslation(cameraOrientation.t + Vec3(viewOffsetSide, viewOffsetForward, viewOffsetUp));
	}

	m_pCameraComponent->SetTransformMatrix(localTransform);

}

void CPlayerComponent::Update(float frameTime) {

	//Pick up message
	Vec3 pickupOrigin = m_pEntity->GetWorldPos() + (m_pEntity->GetForwardDir() * PICKUP_RANGE);
	IPhysicalEntity **pEntityList = NULL;
	int num = gEnv->pEntitySystem->GetPhysicalEntitiesInBox(pickupOrigin, PICKUP_RANGE, pEntityList);
	float lastDist = 10.f;
	float curDist = 0.f;
	SItemComponent *pNewItem = nullptr;
	CVehicleComponent *pNewVehicle = nullptr;

	for (int i = 0; i < num; i++) {

		if (IPhysicalEntity *pPhys = pEntityList[i]) {
			if (IEntity *pInteractingEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pPhys)) {
				if (CVehicleComponent *pVehicle = pInteractingEntity->GetComponent<CVehicleComponent>()) {

					const Vec3 vehiclePos = pInteractingEntity->GetWorldPos();
					const Vec3 diff = vehiclePos - m_pEntity->GetWorldPos();
					curDist = sqrt(powf(diff.x, 2.f) + powf(diff.y, 2.f));

					if (curDist < lastDist) {

						lastDist = curDist;
						pNewVehicle = pVehicle;

					}

				}
				else if (SItemComponent *pPickupItem = pInteractingEntity->GetComponent<SItemComponent>()) {
						if (pPickupItem->IsPickable()) {

						const Vec3 newItemPos = pInteractingEntity->GetWorldPos();
						const Vec3 diff = newItemPos - m_pEntity->GetWorldPos();
						curDist = sqrt(powf(diff.x, 2.f) + powf(diff.y, 2.f));

						if (curDist < lastDist) {

							lastDist = curDist;
							pNewItem = pPickupItem;

						}
					}
				}
			}
		}
	}

	if (pNewVehicle) {
		pTargetVehicle = pNewVehicle;
	}
	else if (pNewItem) {
		pTargetItem = pNewItem;
	}

	if (pTargetVehicle) {
		ShowMessage(pTargetVehicle->GetVehicleName());
	}
	else if (pTargetItem) {
		ShowMessage(pTargetItem->GetItemName());
	}
}

void CPlayerComponent::ShowMessage(string name) {

	Vec2 screenMid = { gEnv->pRenderer->GetWidth() / 2.f, gEnv->pRenderer->GetHeight() / 2.f };
	ColorF pfWhite = { 1.f, 1.f, 1.f, 1.f };
	string sPickupMessage = "Pick up " + name;
	string sVehicleMessage = "Enter " + name;

	//If it's a vehicle, continue
	if (pTargetVehicle) {
		gEnv->pRenderer->GetIRenderAuxGeom()->Draw2dLabel(screenMid.x, screenMid.y, 1.5f, pfWhite, true, sVehicleMessage);
	}
	//else, continue
	else {
		gEnv->pRenderer->GetIRenderAuxGeom()->Draw2dLabel(screenMid.x, screenMid.y, 1.5f, pfWhite, true, sPickupMessage);
	}
}
