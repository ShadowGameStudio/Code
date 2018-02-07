#pragma once

#include "StdAfx.h"

struct SDomeProperties {

	inline bool operator==(const SDomeProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SDomeProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }

	float fMoveTime;
	float fTime;

	int circleAmount;
};

static void ReflectType(Schematyc::CTypeDesc<SDomeProperties>& desc) {

	desc.SetGUID("{6975F376-32D2-4A01-9DF1-90C6BDDCEFB1}"_cry_guid);
	desc.SetLabel("Dome Properties");
	desc.SetDescription("Dome Properties");
	desc.AddMember(&SDomeProperties::fMoveTime, 'fmte', "DecreaseTime", "Decrease Time", "Sets the decreate", 0.0f);
	desc.AddMember(&SDomeProperties::fTime, 'ftme', "PersistanceTime", "Persistance Time", "Sets the time that it takes before the dome decreases", 0.0f);
	desc.AddMember(&SDomeProperties::circleAmount, 'ciam', "CircleAmount", "Circle Amount", "Sets the amount of circles", 0);

}
