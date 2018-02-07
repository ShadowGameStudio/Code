#include "DomeComponent.h"

static void RegisterDome(Schematyc::IEnvRegistrar& registrar) {
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(SCHEMATYC_MAKE_ENV_COMPONENT(CDomeComponent));
		// Functions
		{
		}
	}
}
CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterDome)

void CDomeComponent::InitializeClass() {

	m_pDomeComponent = m_pEntity->GetOrCreateComponentClass<CDomeComponent>();

//SpawnDome();

}

void CDomeComponent::ProcessEventClass(SEntityEvent & event) {

	switch (event.event) {
	case ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED:

		if (sDomeProperties != sPrevDomeProperties) {

			sPrevDomeProperties = sDomeProperties;

			LoadGeometry();

		}

		break;
	}

}

void CDomeComponent::ReflectType(Schematyc::CTypeDesc<CDomeComponent>& desc) {

	desc.SetGUID("{9EFD9954-4E27-464E-9E04-5BC62E4847D5}"_cry_guid);
	desc.AddBase<SItemComponent>();
	desc.SetEditorCategory("Gameplay");
	desc.SetLabel("Dome Component");
	desc.SetDescription("Dome Component");
	desc.AddMember(&CDomeComponent::sItemProperties, 'itep', "ItemProperties", "Item Settings", "All item properties", SItemProperties());

}

void CDomeComponent::PickUp(IEntity * pNewOwner) {}

void CDomeComponent::Drop() {}

bool CDomeComponent::IsPickable() {
	return false;
}

void CDomeComponent::Physicalize() {

	SEntityPhysicalizeParams physParams;
	physParams.type = PHYS_GEOM_TYPE_NO_COLLIDE;

}

void CDomeComponent::LoadGeometry() {

	string sGeomPath = GetProperties()->sRenderProperties.sGeomPath.value.c_str();
	string defaultGeomPath = "Objects/default/primitive_box.cgf";

	if (sGeomPath.empty())
		m_pEntity->LoadGeometry(GEOMETRY_SLOT, defaultGeomPath);
	else
		m_pEntity->LoadGeometry(GEOMETRY_SLOT, sGeomPath);

}

void CDomeComponent::SpawnDome() {


}
