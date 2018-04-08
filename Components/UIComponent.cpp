#include <StdAfx.h>

#include "UIComponent.h"
#include "ItemComponent.h"

void CUIComponent::Initialize() {

	//Gets the element
	pMainUI = gEnv->pFlashUI->GetUIElement("MainUI");
	pShowMainUI = gEnv->pFlashUI->GetUIAction("showmainui");
	pManager = gEnv->pFlashUI->GetUIActionManager();

	//Makes sure that the UI is showing at start
	pManager->StartAction(pShowMainUI, "MainUI");

	//Show
	{
		SUIArguments args;
		args.AddArgument<bool>(true);

		pMainUI->CallFunction("Show");
	}

	//Set health text
	SetHealthbar();

	//UpdateAmmoText
	UpdateAmmo();

}

uint64 CUIComponent::GetEventMask() const {
	return uint64();
}

void CUIComponent::ProcessEvent(const SEntityEvent & event) {}

void CUIComponent::ReflectType(Schematyc::CTypeDesc<CUIComponent>& desc) {

	desc.SetGUID("{10E19C50-3758-48B7-9019-68905C39ABEA}"_cry_guid);

}

//Updates the healthbar
void CUIComponent::SetHealthbar() {

	//Gets the health component
	if (auto *pHealth = m_pEntity->GetComponent<CHealthComponent>()) {

		//Gets the health
		SUIArguments args;
		args.AddArgument<int>((int)pHealth->Get());
	
		//Sets the health
		pMainUI->CallFunction("SetHealth", args);

	}


}

//Updates the ammo text
void CUIComponent::UpdateAmmo() {

	//Gets the players inventory
	if (auto *pInventory = m_pEntity->GetComponent<CInventoryComponent>()) {

		//Gets the item currently selected
		if (auto *pWeapon = pInventory->GetSelectedWeapon()) {

			//Gets the weapon component of it
			if (auto *pWeaponComponent = pWeapon->GetEntity()->GetComponent<CWeaponComponent>()) {
				
				//Creates a string from the two ammo counts and a "/"
				string ammoString = ToString(pWeaponComponent->GetCurrAmmo()) + "/" + ToString(pWeaponComponent->GetClipAmmo());

				SUIArguments args;
				args.AddArgument<string>(ammoString);

				//Calls the flash function
				pMainUI->CallFunction("UpdateAmmoText", args);

			}
			else {

				//Creates a string shown when no weapon is selected
				string ammoString = "0/0";

				SUIArguments args;
				args.AddArgument<string>(ammoString);

				//Calls the flash function
				pMainUI->CallFunction("UpdateAmmoText", args);

			}
		}
		else {

			//Creates a string shown when no weapon is selected
			string ammoString = "0/0";

			SUIArguments args;
			args.AddArgument<string>(ammoString);

			//Calls the flash function
			pMainUI->CallFunction("UpdateAmmoText", args);

		}

	}

}

void CUIComponent::UpdateAlive(int alive) {

	SUIArguments args;

	string sAlive = ToString(alive);

	args.AddArgument<string>(sAlive);

	pMainUI->CallFunction("UpdateAliveText", args);

}
