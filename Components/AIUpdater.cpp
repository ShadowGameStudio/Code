#include "StdAfx.h"
#include "AIComponent.h"

void CAIComponent::UpdateMovementRequest(float frameTime) {

	if (m_AIMode == eAIM_Idle || eAIM_None || eAIM_Attacking) {
		//Sets the move speed of the AI to zero
		m_fMoveSpeed = 0;

	}
	else if (m_AIMode == eAIM_Chasing) {

		//Create a Vec3
		Vec3 velocity = ZERO;

		if (m_bTimerSet == false) {
			m_pEntity->SetTimer(Timer_Speed, 5000);
			m_bTimerSet = true;
		}

		//Adds these two together
		velocity.y += m_fMoveSpeed * frameTime;
		//Pushes the movement
		m_pAIController->AddVelocity(GetEntity()->GetWorldRotation() * velocity);
	}
	else {
		return;
	}


}

void CAIComponent::UpdateAnimation(float frameTime) {

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
	const auto& desiredFragmentId = m_pAIController->IsWalking() ? m_walkFragmentId : m_idleFragmentId;
	if (m_activeFragmentId != desiredFragmentId) {
		m_activeFragmentId = desiredFragmentId;
		m_pAnimationComponent->QueueFragmentWithId(m_activeFragmentId);
	}

}

void CAIComponent::UpdateVicinity(float frameTime) {

	Vec3 AIPos = m_pEntity->GetWorldPos() + (m_pEntity->GetForwardDir() * PERCEPTION_RANGE);
	IPhysicalEntity **pEntityList = NULL;
	int num = gEnv->pEntitySystem->GetPhysicalEntitiesInBox(AIPos, PERCEPTION_RANGE, pEntityList);

	float fLastDist = 100.f;
	float fCurrDist = 0.f;
	CPlayerComponent *pPlayerWithinVicinity = nullptr;

	//Check every entity in the vicinity
	for (int i = 0; i < num; i++) {
		//If it can set pPhys to the entity, continue
		if (IPhysicalEntity *pPhys = pEntityList[i]) {
			//If it can get the entity itself, continue
			if (IEntity *pInteractingEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pPhys)) {
				//If it can get the player from the entity, continue
				if (CPlayerComponent *pPlayer = pInteractingEntity->GetComponent<CPlayerComponent>()) {

					//Gets the players position
					const Vec3 playerPos = pInteractingEntity->GetWorldPos();
					//Gets the difference between the two
					const Vec3 diff = playerPos - m_pEntity->GetWorldPos();
					//Gets the current distance with a square root(expensive)
					fCurrDist = sqrt(powf(diff.x, 2.f) + (diff.y, 2.f));

					//If the current distance is less than the last distance, continue
					if (fCurrDist < fLastDist) {
						
						//Sets the last distance to the current distance
						fLastDist = fCurrDist;
						//Sets the distance from player to the current distance
						m_fDistanceFromPlayer = fCurrDist;

						//Creates a normalized vector that equals to which way the player is
						const Vec3 heading = (pPlayer->GetEntity()->GetWorldPos() - m_pEntity->GetWorldPos()).normalized();
						//Makes the Vec3 above into a direction in Quat, which can be used to look at the player
						const Quat dir = Quat::CreateRotationVDir(heading);
						//Sets the look orientation to the direction that the player is
						m_lookOrientation = dir;

						}

				}
				else {
					m_AIMode = eAIM_None;
				}

			}

		}

	}

	pPlayerWithinVec = pPlayerWithinVicinity;

}

void CAIComponent::UpdateLookOrientation(float frameTime) {

	if (m_AIMode == eAIM_Idle || eAIM_Chasing || eAIM_Attacking) {

		// Update entity rotation as the player turns
		// We only want to affect Z-axis rotation, zero pitch and roll 
		Ang3 ypr = m_pAIController->CreateAnglesYPR(Matrix33(m_lookOrientation));
		ypr.y = 0;
		ypr.z = 0;
		if (m_bGameStarted) {
			const Quat correctedOrientation = Quat(m_pAIController->CreateOrientationYPR(ypr));
			// Send updated transform to the entity, only orientation changes
			GetEntity()->SetPosRotScale(GetEntity()->GetWorldPos(), correctedOrientation, Vec3(1, 1, 1));
		}

	}
	else if (m_AIMode == eAIM_None) {

		return;

	}

}

void CAIComponent::UpdateMode(float frameTime) {

	//If the distance to player is greaer than the chase range, continue
	if (m_fDistanceFromPlayer >= CHASE_RANGE) {
		//Start looking at player
		m_AIMode = eAIM_Idle;
	}
	//Else if the distance to player is less than chase range and greater than attack range, continue
	else if(m_fDistanceFromPlayer <= CHASE_RANGE && m_fDistanceFromPlayer > ATTACK_RANGE){
		//Start chasing player
		m_AIMode = eAIM_Chasing;
	}
	//Else if distance to player is less than attack range, continue
	else if (m_fDistanceFromPlayer <= ATTACK_RANGE) {
		//Start attacking player
		m_AIMode = eAIM_Attacking;
	}

}
