#include "StdAfx.h"

#include "BaseFlowNodes.h"
#include "CryFlowGraph\IFlowBaseNode.h"

class CTestNode : public CFlowBaseNode<eNCT_Instanced> {

public:
	enum EInputs {

		IN_TRIGGER = 0,
		IN_INT_1,
		IN_INT_2,
		IN_INT_3

	};

	enum EOutputs {

		OUT_SUM

	};

	CTestNode(SActivationInfo* pActInfo) {}

	virtual IFlowNodePtr Clone(SActivationInfo *pActInfo) { return new CTestNode(pActInfo); }
	virtual void GetMemoryUsage(ICrySizer* s) const { s->Add(*this); }

	virtual void GetConfiguration(SFlowNodeConfig& config) {

		static const SInputPortConfig in_config[] = {
			InputPortConfig<SFlowSystemVoid>("Trigger", _HELP("Activates the node")),
			InputPortConfig<int>("A", _HELP("Operator A")),
			InputPortConfig<int>("B", _HELP("Operator B")),
			InputPortConfig<int>("C", _HELP("Operator C"))
		};

		static const SOutputPortConfig out_config[] = {
			OutputPortConfig<int>("Sum", _HELP("Sum of the three input integers"))
		};

		config.sDescription = "Sum of three integers when activated";
		config.pInputPorts = in_config;
		config.pOutputPorts = out_config;
		config.SetCategory(EFLN_APPROVED);

	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo) {

		if (event == eFE_Activate) {

			if (IsPortActive(pActInfo, IN_TRIGGER)) {

				const int a = (pActInfo, IN_INT_1);
				const int b = (pActInfo, IN_INT_2);
				const int c = (pActInfo, IN_INT_3);

				ActivateOutput(pActInfo, OUT_SUM, a + b + c);

			}

		}

	}

};

REGISTER_FLOW_NODE("Time:Test", CTestNode);