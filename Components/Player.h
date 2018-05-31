#pragma once

#include <array>
#include <numeric>

#include <CryEntitySystem/IEntityComponent.h>
#include <CryMath/Cry_Camera.h>

#include <ICryMannequin.h>

#include <DefaultComponents/Cameras/CameraComponent.h>
#include <DefaultComponents/Physics/CharacterControllerComponent.h>
#include <DefaultComponents/Geometry/AdvancedAnimationComponent.h>
#include <DefaultComponents/Input/InputComponent.h>
#include "InventoryComponent.h"
#include "HealthpackComponent.h"
#include "HealthComponent.h"
#include "UIComponent.h"
#include "VehicleComponent.h"

////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CPlayerComponent final : public IEntityComponent
{

	enum Timer_Event {
		Timer_jumpTime = 0
	};

	enum class EInputFlagType
	{
		Hold = 0,
		Toggle
	};

	typedef uint8 TInputFlags;

	enum class EInputFlag
		: TInputFlags {
		MoveLeft = 1 << 0,
		MoveRight = 1 << 1,
		MoveForward = 1 << 2,
		MoveBack = 1 << 3,
		MoveSprint = 1 << 4,
		MoveJump = 1 << 5
	};

	template<typename T, size_t SAMPLES_COUNT>
	class MovingAverage
	{
		static_assert(SAMPLES_COUNT > 0, "SAMPLES_COUNT shall be larger than zero!");

	public:

		MovingAverage()
			: m_values()
			, m_cursor(SAMPLES_COUNT)
			, m_accumulator()
		{
		}

		MovingAverage& Push(const T& value)
		{
			if (m_cursor == SAMPLES_COUNT)
			{
				m_values.fill(value);
				m_cursor = 0;
				m_accumulator = std::accumulate(m_values.begin(), m_values.end(), T(0));
			}
			else
			{
				m_accumulator -= m_values[m_cursor];
				m_values[m_cursor] = value;
				m_accumulator += m_values[m_cursor];
				m_cursor = (m_cursor + 1) % SAMPLES_COUNT;
			}

			return *this;
		}

		T Get() const
		{
			return m_accumulator / T(SAMPLES_COUNT);
		}

		void Reset()
		{
			m_cursor = SAMPLES_COUNT;
		}

	private:

		std::array<T, SAMPLES_COUNT> m_values;
		size_t m_cursor;

		T m_accumulator;
	};

	const EEntityAspects kInputAspect = eEA_GameClientD;

public:
	CPlayerComponent() = default;
	virtual ~CPlayerComponent() {}

	// IEntityComponent
	virtual void Initialize() override;
	void InitializeInput();
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	// ~IEntityComponent

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CPlayerComponent>& desc);

	void Revive();

	//Getting components
	CInventoryComponent *GetInventory() { return m_pInventoryComponent; }
	Cry::DefaultComponents::CAdvancedAnimationComponent* GetAnimations() { return m_pAnimationComponent; }
	//

	void FreezePlayer(bool freeze) { bFreezePlayer = freeze; }
	void AttachToBack(SItemComponent *pWeaponToAttach, int slotId);

	//Network functions, vars
	void LocalPlayerInitialize();
	virtual bool NetSerialize(TSerialize ser, EEntityAspects aspect, uint8 profile, int flags) override;
	virtual NetworkAspectType GetNetSerializeAspectMask() const { return kInputAspect; }
	//RMI
	//Vars
	//

	bool bPlayerIsWearingGasmask = false;
	bool bCanTakeInfectedDamage = true;
	bool bPlayerHasBackpack = false;

	Vec3 GetViewWorldDirection();

protected:
	void UpdateMovementRequest(float frameTime);
	void UpdateLookDirectionRequest(float frameTime);
	void UpdateAnimation(float frameTime);
	void UpdateCamera(float frameTime);
	void UpdateFPCamera(float frameTime);
	void Update(float frameTime);
	void ShowMessage(string itemName);

	void SpawnAtSpawnPoint();

	void HandleInputFlagChange(TInputFlags flags, int activationMode, EInputFlagType type = EInputFlagType::Hold);


	//Player actions
	void Action_Use(int activationMode);
	void Action_InventoryToggle(int activationMode);
	void Action_DI(int activationMode);
	void Action_SelectSlot(int activationMode, int slotId);
	void Action_TP(int activationMode);
	void Action_LeanLeft(int activationMode);
	void Action_LeanRight(int activationMode);
	void Action_Attack(int activationMode);
	void Action_Heal(int activationMode);
	void Action_SpawnDome(int activationMode);
	//

	//Player specific methods
	void PickUp(SItemComponent *pNewItem);
	void EnterVehicle(CVehicleComponent *pVehicle);
	void SetPlayerParams();
	//

protected:
	Cry::DefaultComponents::CCameraComponent* m_pCameraComponent = nullptr;
	Cry::DefaultComponents::CCharacterControllerComponent* m_pCharacterController = nullptr;
	Cry::DefaultComponents::CAdvancedAnimationComponent* m_pAnimationComponent = nullptr;
	Cry::DefaultComponents::CInputComponent* m_pInputComponent = nullptr;
	CInventoryComponent *m_pInventoryComponent = nullptr;
	CHealthComponent *m_pHealthComponent = nullptr;
	CUIComponent *m_pUIComponent = nullptr;

	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
	TagID m_rotateTagId;

	TInputFlags m_inputFlags;
	Vec2 m_mouseDeltaRotation;
	MovingAverage<Vec2, 10> m_mouseDeltaSmoothingFilter;

	FragmentID m_activeFragmentId;

	Quat m_lookOrientation; //!< Should translate to head orientation in the future
	float m_horizontalAngularVelocity;
	MovingAverage<float, 10> m_averagedHorizontalAngularVelocity;

	SItemComponent *pTargetItem = nullptr;
	CVehicleComponent *pTargetVehicle = nullptr;

	bool bFreezePlayer;
	bool bIsTP = false;

	const float m_rotationSpeed = 0.002f;
	int m_cameraJointId = 5;
	float m_playerStamina;

	bool bIsInitialized = false;
	bool bIsLeaned = false;
	bool bIsLeanedRight = false;
	bool bIsLeanedLeft = false;

	float fLeanDown = 0.5f;
	float fLeanSide = 0.0f;
	float jumpVel = 0.f;

};
