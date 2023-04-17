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
		m_SaveLoadComponent = SCR_SaveLoadComponent.GetInstance();

		m_wRoot = GetRootWidget();
		m_wFade = m_wRoot.FindAnyWidget("BackgroundFade");
		m_wSystemTime = m_wRoot.FindAnyWidget("SystemTime");
		SCR_EditorManagerEntity editorManager = SCR_EditorManagerEntity.GetInstance();
		SCR_ButtonTextComponent comp;

		// Continue
		comp = SCR_ButtonTextComponent.GetButtonText("Continue", m_wRoot);
		if (comp)
		{
			GetGame().GetWorkspace().SetFocusedWidget(comp.GetRootWidget());
			comp.m_OnClicked.Insert(Close);
		}

		// Restart
		comp = SCR_ButtonTextComponent.GetButtonText("Restart", m_wRoot);
		if (comp)
		{
			bool enabledRestart = !Replication.IsRunning();
			comp.GetRootWidget().SetVisible(enabledRestart);
			comp.m_OnClicked.Insert(OnRestart);
		}

		// Respawn
		comp = SCR_ButtonTextComponent.GetButtonText("Respawn", m_wRoot);
		if (comp)
		{
			bool canRespawn;
			BaseGameMode gameMode = GetGame().GetGameMode();
			if (gameMode)
			{
				RespawnSystemComponent respawn = RespawnSystemComponent.Cast(gameMode.FindComponent(RespawnSystemComponent));
				canRespawn = (respawn != null);
			}

			comp.GetRootWidget().SetVisible(canRespawn);
			comp.m_OnClicked.Insert(OnRespawn);
		}

		// Exit
		comp = SCR_ButtonTextComponent.GetButtonText("Exit", m_wRoot);
		if (comp)
		{
			comp.m_OnClicked.Insert(OnExit);
			if (IsSavingOnExit())
				comp.SetText(EXIT_SAVE);
			else
				comp.SetText(EXIT_NO_SAVE);
		}
		
		// Save
		comp = SCR_ButtonTextComponent.GetButtonText("Save", m_wRoot);
		if (comp)
		{
			comp.m_OnClicked.Insert(SaveGear);
			comp.GetRootWidget().SetVisible(true);
			comp.SetEnabled(true);
		}

		// Load
		comp = SCR_ButtonTextComponent.GetButtonText("Load", m_wRoot);
		if (comp)
		{
			comp.m_OnClicked.Insert(OnLoad);
			comp.GetRootWidget().SetVisible(CanLoad());
			comp.SetEnabled(IsLoadAvailable());
		}
		
		// Camera
		comp = SCR_ButtonTextComponent.GetButtonText("Camera", m_wRoot);
		if (comp)
		{
			comp.m_OnClicked.Insert(OnCamera);
			comp.GetRootWidget().SetEnabled(editorManager && !editorManager.IsOpened());
			comp.GetRootWidget().SetVisible(Game.IsDev());
		}

		// Settings
		comp = SCR_ButtonTextComponent.GetButtonText("Settings", m_wRoot);
		if (comp)
			comp.m_OnClicked.Insert(OnSettings);

		// Field Manual
		comp = SCR_ButtonTextComponent.GetButtonText("FieldManual", m_wRoot);
		if (comp)
		{
			comp.m_OnClicked.Insert(OnFieldManual);
		}

		// Players
		comp = SCR_ButtonTextComponent.GetButtonText("Players", m_wRoot);
		if (comp)
			comp.m_OnClicked.Insert(OnPlayers);

		// Version
		m_wVersion = TextWidget.Cast(m_wRoot.FindAnyWidget("Version"));
		if (m_wVersion)
			m_wVersion.SetText(GetGame().GetBuildVersion());

		// Unlimited editor (Game Master)
		m_EditorUnlimitedOpenButton = SCR_ButtonTextComponent.GetButtonText("EditorUnlimitedOpen",m_wRoot);
		if (m_EditorUnlimitedOpenButton)		
			m_EditorUnlimitedOpenButton.m_OnClicked.Insert(OnEditorUnlimited);

		m_EditorUnlimitedCloseButton = SCR_ButtonTextComponent.GetButtonText("EditorUnlimitedClose",m_wRoot);
		if (m_EditorUnlimitedCloseButton)		
			m_EditorUnlimitedCloseButton.m_OnClicked.Insert(OnEditorUnlimited);
		
		//--- Photo mode
		m_EditorPhotoOpenButton = SCR_ButtonTextComponent.GetButtonText("EditorPhotoOpen",m_wRoot);
		if (m_EditorPhotoOpenButton)
			m_EditorPhotoOpenButton.m_OnClicked.Insert(OnEditorPhoto);
		m_EditorPhotoCloseButton = SCR_ButtonTextComponent.GetButtonText("EditorPhotoClose",m_wRoot);
		if (m_EditorPhotoCloseButton)
			m_EditorPhotoCloseButton.m_OnClicked.Insert(OnEditorPhoto);
		
		SetEditorUnlimitedButton(editorManager);
		SetEditorPhotoButton(editorManager);
		
		if (editorManager)
		{
			editorManager.GetOnModeAdd().Insert(OnEditorModeChanged);
			editorManager.GetOnModeRemove().Insert(OnEditorModeChanged);
		}

		comp = SCR_ButtonTextComponent.GetButtonText("Feedback", m_wRoot);
		if (comp)
		{
			comp.m_OnClicked.Insert(OnFeedback);
		}

		m_InputManager = GetGame().GetInputManager();
		
		m_OnPauseMenuOpened.Invoke();
		
		SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_FE_HUD_PAUSE_MENU_OPEN);
	}

	void SaveGear()
	{
		SCR_PlayerController pc = GetGame().GetPlayerController();
		IEntity player = pc.GetControlledEntity();
		WST_GearPersistenceComponent gpc = WST_GearPersistenceComponent.Cast(player.FindComponent(WST_GearPersistenceComponent));
		gpc.SaveGear();
	
	}
};



