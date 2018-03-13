/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels all the basic AI functionality

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include <DefaultComponents\Physics\CharacterControllerComponent.h>
#include <DefaultComponents\Geometry\AdvancedAnimationComponent.h>
#include <ICryMannequin.h>
#include "Player.h"
#include "AIControllerComponent.h"
#include <array>

#define PERCEPTION_RANGE 20.f
#define CHASE_RANGE 15.f
#define ATTACK_RANGE 0.5f

enum Timer {
	Timer_Speed
};

class CAIComponent final : public IEntityComponent {

	enum EAIMode {

		eAIM_None,
		eAIM_Idle,
		eAIM_Chasing,
		eAIM_Attacking

	};

public:
	CAIComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CAIComponent>& desc);

	EAIMode *GetAIMode() { return &m_AIMode; }

	template<typename T, size_t SAMPLES_COUNT>
	class MovingAverage {
		static_assert(SAMPLES_COUNT > 0, "SAMPLES_COUNT shall be larger than zero!");

	public:

		MovingAverage()
			: m_values()
			, m_cursor(SAMPLES_COUNT)
			, m_accumulator() {}

		MovingAverage& Push(const T& value) {
			if (m_cursor == SAMPLES_COUNT) {
				m_values.fill(value);
				m_cursor = 0;
				m_accumulator = std::accumulate(m_values.begin(), m_values.end(), T(0));
			}
			else {
				m_accumulator -= m_values[m_cursor];
				m_values[m_cursor] = value;
				m_accumulator += m_values[m_cursor];
				m_cursor = (m_cursor + 1) % SAMPLES_COUNT;
			}

			return *this;
		}

		T Get() const {
			return m_accumulator / T(SAMPLES_COUNT);
		}

		void Reset() {
			m_cursor = SAMPLES_COUNT;
		}

	private:

		std::array<T, SAMPLES_COUNT> m_values;
		size_t m_cursor;

		T m_accumulator;
	};

protected:

	void UpdateMovementRequest(float frameTime);
	void UpdateAnimation(float frameTime);
	void UpdateVicinity(float frameTime);
	void UpdateLookOrientation(float frameTime);
	void UpdateMode(float frameTime);


protected:

	CAIControllerComponent *m_pAIController = nullptr;
	Cry::DefaultComponents::CAdvancedAnimationComponent *m_pAnimationComponent = nullptr;
	
	CPlayerComponent *pPlayerWithinVec = nullptr;

	float m_horizontalAngularVelocity;
	float m_fMoveSpeed;
	float m_fDistanceFromPlayer;
	bool m_bGameStarted = false;
	bool m_bTimerSet = false;

	MovingAverage<float, 10> m_averagedHorizontalAngularVelocity;
	FragmentID m_idleFragmentId;
	FragmentID m_walkFragmentId;
	FragmentID m_activeFragmentId;
	TagID m_rotateTagId;

	Quat m_lookOrientation;
	EAIMode m_AIMode;
};