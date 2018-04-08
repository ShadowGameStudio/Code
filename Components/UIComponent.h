/*-------------------------------------------------------------------------------

Author : Ivar Jönsson
Project : BR
Purpose : Handels all of the MainUI functionality

-------------------------------------------------------------------------------*/

#pragma once

#include <CryEntitySystem/IEntityComponent.h>
#include "FlashUI\FlashUI.h"

class CUIComponent : public IEntityComponent {
public:
	CUIComponent() = default;

	virtual void Initialize() override;
	virtual uint64 GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	static void ReflectType(Schematyc::CTypeDesc<CUIComponent>& desc);

	void SetHealthbar();
	void UpdateAmmo();
	void UpdateAlive(int alive);

private:

	IUIElement *pMainUI;
	IUIAction *pShowMainUI;
	IUIActionManager *pManager;

};