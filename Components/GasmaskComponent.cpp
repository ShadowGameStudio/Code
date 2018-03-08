#include "StdAfx.h"
#include "GasmaskComponent.h"

void CGasmaskComponent::InitializeClass() {}

void CGasmaskComponent::ProcessEventClass(SEntityEvent & event) {}

void CGasmaskComponent::ReflectType(Schematyc::CTypeDesc<CGasmaskComponent>& desc) {

	desc.SetGUID("{AD3B9B4E-7084-4637-B2F9-EE79C4979217}"_cry_guid);
	desc.AddBase<SItemComponent>();
	desc.SetEditorCategory("Items");
	desc.SetLabel("Gasmask Component");
	desc.SetDescription("Handels all the gasmask functionality");

}
	