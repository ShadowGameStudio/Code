#pragma once

#include "StdAfx.h"

#include <CrySchematyc\Reflection\TypeDesc.h>
#include <CrySchematyc\ResourceTypes.h>

#include <CrySchematyc/IObject.h>
#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>

enum EVehicleType {

	Car,
	Excavator,
	Crane,
	Truck

};

static void ReflectType(Schematyc::CTypeDesc<EVehicleType>& desc) {

	desc.SetGUID("{43911FEF-339B-41CA-9915-509E8FC9DCC8}"_cry_guid);
	desc.SetLabel("Vehicle type");
	desc.SetDescription("Sets the items type");
	desc.SetFlags(Schematyc::ETypeFlags::Switchable);
	desc.SetDefaultValue(EVehicleType::Car);
	desc.AddConstant(EVehicleType::Car, "Car", "Car");
	desc.AddConstant(EVehicleType::Excavator, "Excavator", "Excavator");
    desc.AddConstant(EVehicleType::Crane, "Crane", "Crane");
    desc.AddConstant(EVehicleType::Truck, "Truck", "Truck");                                                                                                                                     

}

struct SVehicleProperties {

	inline bool operator==(const SVehicleProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SVehicleProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

	EVehicleType eVehicleType;

	float fVehicleSpeed;

	int iMaxPassengers;
	Vec3 vDriversSeat;

	//Non-editor
	string sVehicleName;
};

static void ReflectType(Schematyc::CTypeDesc<SVehicleProperties>& desc) {

	desc.SetGUID("{F2E9DEFA-371D-41C9-B427-13BBD33CB207}"_cry_guid);
	desc.SetLabel("Item Properties");
	desc.SetDescription("Item Properties");
	desc.AddMember(&SVehicleProperties::eVehicleType, 'evt', "VehicleType", "Vehicle Type", "Sets the vehicles type", EVehicleType());
	desc.AddMember(&SVehicleProperties::iMaxPassengers, 'mps', "MaxPassengers", "Max Passengers", "Sets the max amount of passengers the vehicle can carry", 0);
	desc.AddMember(&SVehicleProperties::fVehicleSpeed, 'vs', "VehicleSpeed", "Vehicle Speed", "Sets the speed of the vehicle", 0.0f);
//	desc.AddMember(&SVehicleProperties::vDriversSeat, 'vds', "DriversSeat", "Drivers Seat", "Sets location of the drivers seat", (0, 0, 0));

}