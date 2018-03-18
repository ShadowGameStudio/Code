#pragma once

#include "StdAfx.h"

#include <CrySchematyc\ResourceTypes.h>
#include <CrySchematyc\Reflection\TypeDesc.h>

#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/IObject.h>
#include <CrySchematyc/Utils/SharedString.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>

#include <CryGame/IGameFramework.h>
#include <IActionMapManager.h>

enum ItemGeometrySlot {
	GEOMETRY_SLOT = 0
};

//Render

struct SRenderProperties {
	
	inline bool operator==(const SRenderProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SRenderProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

	Schematyc::GeomFileName sGeomPath;
	Schematyc::MaterialFileName sMaterial;

};

static void ReflectType(Schematyc::CTypeDesc<SRenderProperties>& desc) {

	desc.SetGUID("{91A636AC-A57A-4CDC-96D0-2F658508BE0A}"_cry_guid);
	desc.SetLabel("Render Properties");
	desc.SetDescription("Render Properties");
	desc.AddMember(&SRenderProperties::sGeomPath, 'geom', "GeometryPath", "Geometry path", "Geometry path of this item", "");
	desc.AddMember(&SRenderProperties::sMaterial, 'smat', "MaterialPath", "Material path", "Material path of this item", "");
}

//Physics

struct SPhysicsProperties {

	inline bool operator==(const SPhysicsProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SPhysicsProperties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

	float fMass;

};

static void ReflectType(Schematyc::CTypeDesc<SPhysicsProperties>& desc) {

	desc.SetGUID("{F5784ABE-1C84-4018-B761-984990FDE573}"_cry_guid);
	desc.SetLabel("Physics Properties");
	desc.SetDescription("Physics Properties");
	desc.AddMember(&SPhysicsProperties::fMass, 'fmas', "Mass", "Mass", "Mass of the item", 0.f);
}

enum EItemType {

	Normal,
	WeaponEquipment,
	Weapon,
	Backpack,
	MeeleWeapon,
	Healthpack,
	Gasmask

};

static void ReflectType(Schematyc::CTypeDesc<EItemType>& desc) {

	desc.SetGUID("{18935D73-EBBE-4E5C-9D24-239B6D0071A2}"_cry_guid);
	desc.SetLabel("Item type");
	desc.SetDescription("Sets the items type");
	desc.SetFlags(Schematyc::ETypeFlags::Switchable);
	desc.SetDefaultValue(EItemType::Normal);
	desc.AddConstant(EItemType::Normal, "Normal", "Normal");
	desc.AddConstant(EItemType::WeaponEquipment, "WeaponEquipment", "Weapon Equipment");
	desc.AddConstant(EItemType::Weapon, "Weapon", "Weapon");
	desc.AddConstant(EItemType::Backpack, "Backpack", "Backpack");
	desc.AddConstant(EItemType::MeeleWeapon, "MeeleWeapon", "Meele Weapon");
	desc.AddConstant(EItemType::Healthpack, "Healthpack", "Healthpack");
	desc.AddConstant(EItemType::Gasmask, "Gasmask", "Gasmask");

}

//Main Prop. Structure
struct SItemProperties {

	inline bool operator==(const SItemProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
	inline bool operator!=(const SItemProperties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }

	SRenderProperties sRenderProperties, sPrevRenderProperties;
	SPhysicsProperties sPhysicsProperties;
	EItemType eItemType;
	//Item specific

	int itemLevel;
	float fItemWeight;

	//Non-editor
	string sItemName;
};

static void ReflectType(Schematyc::CTypeDesc<SItemProperties>& desc) {

	desc.SetGUID("{0E44FE1E-A4EF-440D-B4D1-C95046005B09}"_cry_guid);
	desc.SetLabel("Item Properties");
	desc.SetDescription("Item Properties");
	desc.AddMember(&SItemProperties::sRenderProperties, 'renp', "RenderProperties", "Render Properties", "All the render settings for current item", SRenderProperties());
	desc.AddMember(&SItemProperties::sPhysicsProperties, 'phyp', "PhysicsProperties", "Physics Properties", "All the physics settings for current item", SPhysicsProperties());
	desc.AddMember(&SItemProperties::eItemType, 'eit', "ItemType", "Item Type", "Sets the items type", EItemType());
	desc.AddMember(&SItemProperties::fItemWeight, 'iw', "ItemWeight", "Item Weight", "Sets the item weight in kilos", 0.f);
	desc.AddMember(&SItemProperties::itemLevel, 'ir', "ItemRarity", "Item Rarity", "Sets the item rarity", 0);

}