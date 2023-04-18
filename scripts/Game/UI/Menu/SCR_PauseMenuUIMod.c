//------------------------------------------------------------------------------------------------
modded class PauseMenuUI
{
	InputManager m_InputManager;

	protected Widget m_wEditorOpen;
	protected Widget m_wEditorClose;
	protected TextWidget m_wVersion;
	protected Widget m_wRoot;
	protected Widget m_wFade;
	protected Widget m_wSystemTime;
	protected bool m_bFocused = true;

	//Editor and Photo mode Specific
	protected SCR_ButtonTextComponent m_EditorUnlimitedOpenButton;
	protected SCR_ButtonTextComponent m_EditorUnlimitedCloseButton;
	protected SCR_ButtonTextComponent m_EditorPhotoOpenButton;
	protected SCR_ButtonTextComponent m_EditorPhotoCloseButton;
	
	protected SCR_SaveLoadComponent m_SaveLoadComponent;
	
	const string EXIT_SAVE = "#AR-PauseMenu_ReturnSaveTitle";
	const string EXIT_NO_SAVE = "#AR-PauseMenu_ReturnTitle";
	
	const ResourceName ACTIONS_IMAGESET = "{2EFEA2AF1F38E7F0}UI/Textures/Icons/icons_wrapperUI-64.imageset";
	const string EXIT_MESSAGE = "#AR-PauseMenu_ReturnText";
	const string EXIT_TITLE = "#AR-PauseMenu_ReturnTitle";
	const string EXIT_IMAGE = "exit";
	
	const string RESTART_MESSAGE = "#AR-PauseMenu_RestartText";
	const string RESTART_TITLE = "#AR-PauseMenu_Restart";
	const string RESTART_IMAGE = "restart";

	const string LOAD_MESSAGE = "#AR-PauseMenu_LoadText";
	const string LOAD_TITLE = "#AR-PauseMenu_Load";
	const string LOAD_IMAGE = "up";
		
	static ref ScriptInvoker m_OnPauseMenuOpened = new ScriptInvoker();
	static ref ScriptInvoker m_OnPauseMenuClosed = new ScriptInvoker();

	//------------------------------------------------------------------------------------------------
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		
		SCR_ButtonTextComponent comp;

		
		
		// Save
		comp = SCR_ButtonTextComponent.GetButtonText("Save", m_wRoot);
		if (comp)
		{
			comp.m_OnClicked.Insert(SaveGear);
			comp.GetRootWidget().SetVisible(true);
			comp.SetEnabled(true);
		}

		
	}

	void SaveGear()
	{
		SCR_PlayerController pc = GetGame().GetPlayerController();
		IEntity player = pc.GetControlledEntity();
		WST_GearPersistenceComponent gpc = WST_GearPersistenceComponent.Cast(player.FindComponent(WST_GearPersistenceComponent));
		gpc.SaveGear();
	
	}
};



