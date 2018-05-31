#pragma once

#include "StdAfx.h"

#include <CrySchematyc\Reflection\TypeDesc.h>
#include <CrySchematyc\ResourceTypes.h>

#include <CrySchematyc/IObject.h>
#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>

struct SVehicleRenderProperties {

    inline bool operator==(const SVehicleRenderProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SVehicleRenderProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

	Schematyc::GeomFileName sGeomPath;
	Schematyc::MaterialFileName sMaterial;

};

static void ReflectType(Schematyc::CTypeDesc<SVehicleRenderProperties> &desc) {

    desc.SetGUID("{5567A27E-895B-45EB-821B-659B032C3014}"_cry_guid);
    desc.SetLabel("Render Properties");
    desc.SetDescription("Render Properties");
    desc.AddMember(&SVehicleRenderProperties::sGeomPath, 'geo', "GeometryPath", "Geometry path", "Geometry path of this item", "");
    desc.AddMember(&SVehicleRenderProperties::sMaterial, 'mat', "MaterialPath", "Material path", "Material path of this item", "");
}

struct SVehiclePhysicsProperties {

	inline bool operator==(const SVehiclePhysicsProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SVehiclePhysicsProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

	float fMass;

};

static void ReflectType(Schematyc::CTypeDesc<SVehiclePhysicsProperties>& desc) {

	desc.SetGUID("{049D69D8-50B2-41FB-AAA6-DA6A3A4E33D2}"_cry_guid);
	desc.SetLabel("Physics Properties");
	desc.SetDescription("Physics Properties");
	desc.AddMember(&SVehiclePhysicsProperties::fMass, 'fma', "Mass", "Mass", "Mass of the vehicle", 0.f);
}

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

	SVehicleRenderProperties sRenderProperties, sPrevRenderProperties;
	SVehiclePhysicsProperties sPhysicsProperties;
	EVehicleType eVehicleType;

	int iMaxPassengers;
	float fMaxVelocity;
	Vec3 vDriversSeat;

	//Non-editor
	string sVehicleName;
};

static void ReflectType(Schematyc::CTypeDesc<SVehicleProperties>& desc) {

	desc.SetGUID("{F2E9DEFA-371D-41C9-B427-13BBD33CB207}"_cry_guid);
	desc.SetLabel("Item Properties");
	desc.SetDescription("Item Properties");
	desc.AddMember(&SVehicleProperties::sRenderProperties, 'ren', "RenderProperties", "Render Properties", "All the render settings for current vehicle", SVehicleRenderProperties());
	desc.AddMember(&SVehicleProperties::sPhysicsProperties, 'phy', "PhysicsProperties", "Physics Properties", "All the physics settings for current vehicle", SVehiclePhysicsProperties());
	desc.AddMember(&SVehicleProperties::eVehicleType, 'evt', "VehicleType", "Vehicle Type", "Sets the vehicles type", EVehicleType());
	desc.AddMember(&SVehicleProperties::iMaxPassengers, 'mps', "MaxPassengers", "Max Passengers", "Sets the max amount of passengers the vehicle can carry", 0);
	desc.AddMember(&SVehicleProperties::fMaxVelocity, 'fmv', "MaxVelocity", "Max Velocity", "Sets the max velocity of the vehicle", 0.f);
	desc.AddMember(&SVehicleProperties::vDriversSeat, 'vds', "DriversSeat", "Drivers Seat", "Sets location of the drivers seat", 0, 0, 0);

}