[ComponentEditorProps(category: "", description: "")]
class DBG_TestActionClass: ScriptComponentClass {};
 
class DBG_TestAction: ScriptComponent
{

	override void EOnInit(IEntity owner)
	{
		Print("DBG_TESTACTION: E");
		
		GetGame().GetInputManager().AddActionListener("TestAction", EActionTrigger.DOWN, DBG_TestActionPrintToConsole);
		
        owner.SetFlags(EntityFlags.ACTIVE, true);
		SetEventMask(owner, EntityEvent.FRAME);
	}
 
	// We need the OnPostInit and EOnInit for the EOnFrame to be called.
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT | EntityEvent.FRAME);
	}
	
	void DBG_TestActionPrintToConsole()
	{
		Print("DBG_TESTACTIONCLASS: ACTION FIRED!");
		GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.WST_TransferWindow);
	}


};