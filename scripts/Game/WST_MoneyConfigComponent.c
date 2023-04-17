class WST_MoneyConfigComponentClass : ScriptComponentClass
{


};



class WST_MoneyConfigComponent : ScriptComponent
{
	[Attribute("50", UIWidgets.EditBox, "Initial value of wallet")]
	int initalValue;
	
	bool valueChanged;
	
	[RplProp(onRplName: "OnBroadcastValueUpdated", condition: RplCondition.Custom, customConditionName: "RpcConditionMethod")]
	bool alreadySet = false;
	
	
	void WST_MoneyConfigComponent()
	{
		valueChanged = false;
	
	}
	void OnBroadcastValueUpdated()
	{
		// this method will only run on proxies if authority's RpcConditionMethod returns true
		Print("MoneyComponent::OnBroadcastValueUpdated::: " );

	}
	
	bool RpcConditionMethod()
	{
		if (valueChanged)
		{
			valueChanged = false;
			return true;
		
		}
		return false;
	}
	int GetValue()
	{
		return initalValue;
	}
	
};
