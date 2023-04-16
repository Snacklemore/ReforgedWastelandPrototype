class WST_MoneyConfigComponentClass : ScriptComponentClass
{


};



class WST_MoneyConfigComponent : ScriptComponent
{
	[Attribute("50", UIWidgets.EditBox, "Initial value of wallet")]
	int initalValue;
	
	bool alreadySet = false;
	int GetValue()
	{
		return initalValue;
	}
	
};
