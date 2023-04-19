class SCR_BaseGameModeWastelandClass: SCR_BaseGameModeClass
{
};

class SCR_BaseGameModeWasteland : SCR_BaseGameMode
{

	[Attribute("US", UIWidgets.EditBox, "", "")]
	protected FactionKey m_sBLUFORFactionKey;
	
	[Attribute("USSR", UIWidgets.EditBox, "", "")]
	protected FactionKey m_sOPFORFactionKey;
	
	[Attribute("FIA", UIWidgets.EditBox, "", "")]
	protected FactionKey m_sINDFORFactionKey;
	
	[Attribute(defvalue: "0", uiwidget: UIWidgets.CheckBox, desc: "When enabled, bases will keep their present owning factions regardless of randomization.")]
	protected bool m_bApplyPresetOwners;
	
	[Attribute("2", desc: "How many control points does a faction need to win"), RplProp()]
	protected int m_iControlPointsThreshold;
	
	[Attribute("300", desc: "How long does a faction need to hold the control points to win (seconds)"), RplProp()]
	protected float m_fVictoryTimer;
	
	[Attribute("40", desc: "How much supplies it cost to spawn at base by default?")]
	protected int m_iSpawnCost;
	
	[Attribute("40", UIWidgets.EditBox, "Total maximum of all player and allied AI units.")]
	int m_iTotalPlayersLimit;
	
	[Attribute("1", UIWidgets.CheckBox, "Randomized starting supplies in small bases", "")]
	bool m_bRandomizeSupplies;
	
	[Attribute("2", UIWidgets.EditBox, "How many vehicles should spawn at HQ at start.")]
	protected int m_iStartingHQVehicles;
	
	[Attribute("30", UIWidgets.EditBox, "Time in seconds for how long the vehicle is reserved for a player who request it.")] 
	int m_iSpawnedVehicleTimeProtection;
	
	[Attribute("{F7E8D4834A3AFF2F}UI/Imagesets/Conflict/conflict-icons-bw.imageset")]
	protected ResourceName m_sBuildingIconImageset;
	
	[Attribute("{2294DC90D8E050D2}UI/layouts/Campaign/BuildingHUDIcon.layout")]
	protected ResourceName m_sHUDIcon;
	
	[Attribute("{F1AC26310BAE3788}Prefabs/MP/Campaign/CampaignFactionManager.et")]
	ResourceName m_FactionManagerPrefab;
	
	[Attribute("{35BD6541CBB8AC08}Prefabs/AI/Waypoints/AIWaypoint_Cycle.et")]
	ResourceName m_CycleWaypointPrefab;
	
	[Attribute("{22A875E30470BD4F}Prefabs/AI/Waypoints/AIWaypoint_Patrol.et")]
	ResourceName m_PatrolWaypointPrefab;
	
	[Attribute("{EAAE93F98ED5D218}Prefabs/AI/Waypoints/AIWaypoint_CaptureRelay.et")]
	ResourceName m_RetakeWaypointPrefab;
	
	[Attribute("{B3E7B8DC2BAB8ACC}Prefabs/AI/Waypoints/AIWaypoint_SearchAndDestroy.et")]
	ResourceName m_SeekDestroyWaypointPrefab;
	
	[Attribute("{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et")]
	ResourceName m_DefendWaypointPrefab;
	
	[Attribute("", UIWidgets.ResourceNamePicker, "Remnants group prefab", "et")]
	private ResourceName m_RemnantsGroupPrefab;
	
	[Attribute("{47864BB47AB0B1F4}UI/layouts/HUD/CampaignMP/CampaignMainHUD.layout", "Campaign main HUD")]
	protected ResourceName m_sCampaignHud;
	
	[Attribute("{D74D24696C4F32F0}UI/layouts/HUD/CampaignMP/CampaignMenuScoreUI.layout", "Campaign MAP HUD")]
	protected ResourceName m_sCampaignMapHud;
	
	//*****************************//
	//PUBLIC STATIC SCRIPT INVOKERS//
	//*****************************//
	
	static ref ScriptInvoker s_OnBaseCaptured = new ref ScriptInvoker();
	static ref ScriptInvoker s_OnSignalChanged = new ref ScriptInvoker();
	static ref ScriptInvoker s_OnEnterEnemyBase = new ref ScriptInvoker();
	static ref ScriptInvoker s_OnFactionAssigned = new ref ScriptInvoker();
	static ref ScriptInvoker s_OnFactionAssignedLocalPlayer = new ref ScriptInvoker();
	static ref ScriptInvoker s_OnMobileAssemblyDeployChanged = new ScriptInvoker();
	static ref ScriptInvoker s_OnServiceBuild = new ScriptInvoker();
	protected ref ScriptInvoker Event_OnPlayerEnterBase = new ref ScriptInvoker(); //Gives SCR_CampaignBase
	protected ref ScriptInvoker Event_OnPlayerLeftBase = new ref ScriptInvoker(); //Gives SCR_CampaignBase
		
	//************************//
	//RUNTIME STATIC VARIABLES//
	//************************//
	protected static SCR_RespawnSystemComponent s_RespawnSystemComponent = null;
	protected static SCR_BaseGameModeWasteland s_Instance = null;
	protected static string s_sBackendFilename;
	protected static ref array<SCR_PlayerRadioSpawnPointCampaign> m_aRadioSpawnPoints = {};
	protected static const int XP_INFO_DURATION = 10000; 
	protected static const int BACKEND_WRITE_PERIOD = 60000;
	protected static const int INVALID_BACKEND_ID = 0;
	protected static const int REMNANTS_SPAWN_RADIUS_MIN = Math.Pow(500, 2);
	protected static const int REMNANTS_SPAWN_RADIUS_MAX = Math.Pow(1000, 2);
	protected static const int REMNANTS_DESPAWN_RADIUS_DIFF = Math.Pow(200, 2);
	protected static int STARTING_SUPPLIES_MIN = 100;
	protected static int STARTING_SUPPLIES_MAX = 500;
	static const int SUPPLY_DEPOT_CAPACITY = 4500;
	protected static const int STARTING_SUPPLIES_INTERVAL = 25;
	protected static const int HQ_NO_REMNANTS_RADIUS = 300;
	protected static const int HQ_NO_REMNANTS_PATROL_RADIUS = 600;
	static const int GARAGE_VEHICLE_SPAWN_INTERVAL = 60000;
	static const int REINFORCEMENTS_SUPPLIES = 10;
	static const int REINFORCEMENTS_CHECK_PERIOD = 1000;			//ms: how often we should check for reinforcements arrival
	static const float SUPPLY_TRUCK_UNLOAD_RADIUS = 25;				//m: maximum distance from a supply depot a player can still (un)load their truck
	static FactionKey FACTION_BLUFOR;
	static FactionKey FACTION_OPFOR;
	static FactionKey FACTION_INDFOR;
	static const int AI_GROUPS_PER_FACTION = 3;
	static const int REMNANTS_CNT_MIN = 9;
	static const int REMNANTS_CNT_MAX = 9;
	static const int BASE_CALLSIGNS_COUNT = 40;
	static const int ENDING_TIMEOUT = 15000;
	static const int BUILDING_CONTROLLER_SEARCH_DISTANCE = 7;
	static const int SKILL_LEVEL_MAX = 10;
	static const float SKILL_LEVEL_XP_BONUS = 0.1;
	static const int SKILL_LEVEL_XP_COST = 1000;		// how much XP is needed for new level
	static const int TIME_TO_HIDE_HUD = 15000;
	
	//************************//
	//RUNTIME MEMBER VARIABLES//
	//************************//
	protected ref array<SCR_AIGroup> m_aAIGroups = new ref array<SCR_AIGroup>();
	protected ref array<SCR_CampaignSuppliesComponent> m_aRemnantSupplyDepots = {};
	protected ref array<ref SCR_CampaignClientData> m_aRegisteredClients = new array<ref SCR_CampaignClientData>;
	protected ref array<ref SCR_CampaignRemnantsPresence> m_aRemnantsPresence = new array<ref SCR_CampaignRemnantsPresence>;
	protected ref array<ref SCR_CampaignXPRewardInfo> m_aXPRewardList;
	protected ref array<Vehicle> m_aRequestedVehicles = new array<Vehicle>;
	protected ref map<SCR_SiteSlotEntity, ref SCR_BuildingDedicatedSlotData> m_aDedicatedSlots = new map<SCR_SiteSlotEntity, ref SCR_BuildingDedicatedSlotData>();
	protected ref array<SCR_CampaignBase> m_aBasesWithPlayer = new array<SCR_CampaignBase>;
	protected float m_fRemnantForcesTimer = 0;
	protected int m_iLocationCheckedForPlayerProximity = -1;
	protected bool m_bAllRemnantsSpawned = false;
	protected ref Resource m_rRemnants;
	protected ref Resource m_rAIVehicleWest;
	protected ref Resource m_rAIVehicleEast;
	protected int m_iAIGroupSizeWest;
	protected int m_iAIGroupSizeEast;
	protected bool m_bAllowRemnantsDespawn = true;
	protected bool m_bIsPlayerInRadioRange = true;
	protected Faction m_LastPlayerFaction;
	protected SCR_CampaignBase m_LastVisitedBase;
	protected int m_iBaseSeizingHintsShown;
	protected int m_iSuppliesHintsShown;
	protected SCR_CampaignBase m_FirstBaseWithPlayer;
	protected bool m_bStartupHintsShown = false;
	protected bool m_bRespawnHintShown = false;
	protected bool m_bReinforcementsHintShown = false;
	protected bool m_bTicketsHintShown = false;
	protected bool m_bBaseLostHintShown = false;
	protected AudioHandle m_PlayedRadio = AudioHandle.Invalid;
	protected RplId m_LastDeployedHQIDWest = RplId.Invalid();
	protected RplId m_LastDeployedHQIDEast = RplId.Invalid();
	protected IEntity m_BuildingController;
	protected ref SimplePreload m_Preload;
	protected int m_iRemnantLocationsCnt;
	protected float m_fXpMultiplier = 1;
	protected bool m_bIgnoreMinimumVehicleRank;
	protected ref map<int, Faction> m_aUnprocessedFactionAssignments = new ref map<int, Faction>();
	protected bool m_bCanShowSpawnPosition; // check for showing player spawn location on map
	protected vector m_vFirstSpawnPosition = vector.Zero;
	protected SCR_MapEntity m_MapEntity;
	protected SCR_MapCampaignUI m_MapCampaignUI;
	protected bool m_bWasMapOpened;
	protected ref TimeContainer m_SpawnTime;
	protected bool m_bIsTutorial;
	protected bool m_bIsShowingXPBar;
	protected bool m_bMatchOver;
	protected SCR_CampaignSuppliesComponent m_SupplyDepotWithPlayer;
	protected int m_iHideHud;
	protected int m_iShowWest;
	protected int m_iShowEast;
	TriggerSpawnManager m_TownSpawnManager;
	// UI-related
	protected float m_fHideXPInfo = 0;
	protected Widget m_wGOScreen;
	protected Widget m_wXPInfo;
	protected Widget m_wPlayersList;
	protected Widget m_wPlayersListSlot;
	protected bool m_bNegativeXP = false;
	ref array<Widget> m_aPlayerList = new ref array<Widget>();
	protected Widget m_wInfoOverlay;
	protected Widget m_wCountdownOverlay;
	protected Widget m_wCountdownOverlayMap;
	protected ImageWidget m_wLeftFlag;
	protected ImageWidget m_wRightFlag;
	protected ImageWidget m_wWinScoreSideLeft;
	protected ImageWidget m_wWinScoreSideRight;
	protected RichTextWidget m_wLeftScore;
	protected RichTextWidget m_wRightScore;
	protected RichTextWidget m_wWinScore;
	protected RichTextWidget m_wCountdown;
	protected RichTextWidget m_wFlavour;
	protected ImageWidget m_wLeftFlagMap;
	protected ImageWidget m_wRightFlagMap;
	protected RichTextWidget m_wLeftScoreMap;
	protected RichTextWidget m_wRightScoreMap;
	protected RichTextWidget m_wWinScoreMap;
	protected RichTextWidget m_wCountdownMap;
	protected RichTextWidget m_wFlavourMap;
	protected ImageWidget m_wWinScoreSideLeftMap;
	protected ImageWidget m_wWinScoreSideRightMap;
	protected Widget m_wRoot;
	protected Widget m_wMapRoot;	
	
	SCR_CharacterTriggerEntity triggerTest;
	SCR_MoneyManager m_MoneyManager;
	
	
	//********************************//
	//RUNTIME SYNCHED MEMBER VARIABLES//
	//********************************//
	[RplProp()]
	protected float m_fGameEnd = -1;
	[RplProp(onRplName: "OnMobileAssemblyDeployChangedWest")]
	protected RplId m_DeployedMobileAssemblyIDWest = RplId.Invalid();
	[RplProp(onRplName: "OnMobileAssemblyDeployChangedEast")]
	protected RplId m_DeployedMobileAssemblyIDEast = RplId.Invalid();
	[RplProp()]
	protected int m_iCustomRadioRange = -1;
	[RplProp()]
	protected int m_iMaxRespawnRadios = 5;
	[RplProp()]
	protected int m_iActiveRespawnRadiosCntWest;
	[RplProp()]
	protected int m_iActiveRespawnRadiosCntEast;
	[RplProp()]
	protected float m_fVictoryTimestamp;
	[RplProp()]
	protected float m_fVictoryPauseTimestamp;
	[RplProp()]
	protected int m_iWinningFactionId = -1;
	[RplProp()]
	protected int m_iControlPointsHeldWest;
	[RplProp()]
	protected int m_iControlPointsHeldEast;
	
	//************************//
	//PROTECTED STATIC METHODS//
	//************************//
	
	//*********************//
	//PUBLIC STATIC METHODS//
	//*********************//
	
	//------------------------------------------------------------------------------------------------
	static Resource GetFactionManagerResource()
	{
		SCR_GameModeCampaignMP campaign = GetCampaign();
		if (!campaign)
			return null;
		
		return Resource.Load(campaign.m_FactionManagerPrefab);
	}
	
	//------------------------------------------------------------------------------------------------
	static string GetBackendFilename()
	{
		return s_sBackendFilename;
	}
	
	//------------------------------------------------------------------------------------------------
	static SCR_GameModeCampaignMP GetCampaign()
	{
		return SCR_GameModeCampaignMP.Cast(GetGame().GetGameMode());
	}
	
	//------------------------------------------------------------------------------------------------
	//! Check if we're not actually playing (and halt the code in proper places)
	static bool NotPlaying()
	{
		auto game = GetGame();
		
		if (!game || !game.InPlayMode())
			return true;
		
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns an instance of this game mode.
	static SCR_BaseGameModeWasteland GetInstance()
	{
		return s_Instance;
	}
	
	//*********************//
	//PUBLIC MEMBER METHODS//
	//*********************//
	
	//------------------------------------------------------------------------------------------------
	int GetFactionControlPointsCount(FactionKey factionKey)
	{
		if (factionKey == FACTION_BLUFOR)
			return m_iControlPointsHeldWest;
		
		if (factionKey == FACTION_OPFOR)
			return m_iControlPointsHeldEast;
		
		return 0;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetControlPointTreshold()
	{
		return m_iControlPointsThreshold;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetStartingVehiclesCount()
	{
		return m_iStartingHQVehicles;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetSpawnCost()
	{
		return m_iSpawnCost;
	}
	
	//------------------------------------------------------------------------------------------------
	bool CanShowPlayerSpawn()
	{
		return m_bCanShowSpawnPosition;
	}
	
	//------------------------------------------------------------------------------------------------
	ResourceName GetCycleWaypointPrefab()
	{
		return m_CycleWaypointPrefab;
	}
	
	//------------------------------------------------------------------------------------------------
	ResourceName GetPatrolWaypointPrefab()
	{
		return m_PatrolWaypointPrefab;
	}
	
	//------------------------------------------------------------------------------------------------
	ResourceName GetRetakeWaypointPrefab()
	{
		return m_RetakeWaypointPrefab;
	}
	
	//------------------------------------------------------------------------------------------------
	ResourceName GetSeekDestroyWaypointPrefab()
	{
		return m_SeekDestroyWaypointPrefab;
	}
	
	//------------------------------------------------------------------------------------------------
	ResourceName GetDefendWaypointPrefab()
	{
		return m_DefendWaypointPrefab;
	}
	
	//------------------------------------------------------------------------------------------------
	bool IsMobileAssemblyDeployed(notnull SCR_CampaignFaction faction)
	{
		switch (faction.GetFactionKey())
		{
			case FACTION_BLUFOR: {return m_DeployedMobileAssemblyIDWest.IsValid();};
			case FACTION_OPFOR: {return m_DeployedMobileAssemblyIDEast.IsValid();};
		}
		
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetIsPlayerInRadioRange(bool status)
	{
		m_bIsPlayerInRadioRange = status;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Add XP to given entity
	void AwardXP(IEntity player, CampaignXPRewards rewardID, float multiplier = 1.0, bool volunteer = false)
	{
		if (IsProxy() || !player)
			return;
		
		PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(player));
		AwardXP(playerController, rewardID, multiplier, volunteer);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Add XP to given controller
	void AwardXP(PlayerController controller, CampaignXPRewards rewardID, float multiplier = 1.0, bool volunteer = false)
	{
		if (!controller || IsProxy())
			return;

		SCR_CampaignNetworkComponent campaignNetworkComponent = SCR_CampaignNetworkComponent.Cast(controller.FindComponent(SCR_CampaignNetworkComponent));
		
		if (campaignNetworkComponent)
			campaignNetworkComponent.AddPlayerXP(rewardID, multiplier, volunteer)
	}
	
	//------------------------------------------------------------------------------------------------
	int GetMaxRespawnRadios()
	{
		return m_iMaxRespawnRadios;
	}
	
	//------------------------------------------------------------------------------------------------
	void PlayRadioMsg(SCR_ERadioMsg msg, int baseCallsign, int callerCallsignCompany, int callerCallsignPlatoon, int callerCallsignSquad, int calledCallsignCompany, int calledCallsignPlatoon, int calledCallsignSquad, int param, float seed, float quality)
	{
		if (m_bIsTutorial)
			return;
		
		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		
		if (!pc)
			return;
		
		IEntity player = pc.GetMainEntity();
		
		if (!player)
			return;
		
		SCR_CommunicationSoundComponent soundComp = SCR_CommunicationSoundComponent.Cast(player.FindComponent(SCR_CommunicationSoundComponent));
		
		if (!soundComp)
			return;
		
		SignalsManagerComponent signalComp = SignalsManagerComponent.Cast(player.FindComponent(SignalsManagerComponent));
		
		if (!signalComp)
			return;
		
		int signalBase = signalComp.AddOrFindSignal("Base");
		int signalCompanyCaller = signalComp.AddOrFindSignal("CompanyCaller");
		int signalCompanyCalled = signalComp.AddOrFindSignal("CompanyCalled");
		int signalPlatoonCaller = signalComp.AddOrFindSignal("PlatoonCaller");
		int signalPlatoonCalled = signalComp.AddOrFindSignal("PlatoonCalled");
		int signalSquadCaller = signalComp.AddOrFindSignal("SquadCaller");
		int signalSquadCalled = signalComp.AddOrFindSignal("SquadCalled");
		float signalSeed = signalComp.AddOrFindSignal("Seed");
		float signalQuality = signalComp.AddOrFindSignal("TransmissionQuality");
		
		if (baseCallsign != SCR_CampaignBase.INVALID_BASE_INDEX)
			signalComp.SetSignalValue(signalBase, baseCallsign);
		
		if (callerCallsignCompany != SCR_CampaignBase.INVALID_PLAYER_INDEX)
		{
			signalComp.SetSignalValue(signalCompanyCaller, callerCallsignCompany);
			signalComp.SetSignalValue(signalPlatoonCaller, callerCallsignPlatoon);
			signalComp.SetSignalValue(signalSquadCaller, callerCallsignSquad);
		}
		
		if (calledCallsignCompany != SCR_CampaignBase.INVALID_PLAYER_INDEX)
		{
			signalComp.SetSignalValue(signalCompanyCalled, calledCallsignCompany);
			signalComp.SetSignalValue(signalPlatoonCalled, calledCallsignPlatoon);
			signalComp.SetSignalValue(signalSquadCalled, calledCallsignSquad);
		}
		
		signalComp.SetSignalValue(signalSeed, seed);
		signalComp.SetSignalValue(signalQuality, quality);
		
		string msgName;
		LocalizedString text;
		LocalizedString text2;
		string param1;
		string text2param1;
		string text2param2;
		int duration = SCR_PopUpNotification.DEFAULT_DURATION;
		SCR_ECampaignPopupPriority prio = SCR_ECampaignPopupPriority.DEFAULT;
		SCR_CampaignBase base = SCR_CampaignBaseManager.FindBaseByID(param);
		string sound;
		
		switch (msg)
		{
			case SCR_ERadioMsg.SEIZED_MAIN:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_MOB;
				break;
			}
			
			case SCR_ERadioMsg.SEIZED_MAJOR:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_FOB;
				break;
			}
			
			case SCR_ERadioMsg.SEIZED_SMALL:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_COP;
				break;
			}
			
			case SCR_ERadioMsg.DEMOTION_RENEGADE:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_REN;
				text = "#AR-Campaign_Demotion-UC";
				text2 = "#AR-Rank_Renegade";
				break;
			}
			
			case SCR_ERadioMsg.DEMOTION:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_DEM;
				text = "#AR-Campaign_Demotion-UC";
				SCR_CampaignFaction f = SCR_CampaignFaction.Cast(m_LastPlayerFaction);
				
				if (f)
					text2 = f.GetRankNameUpperCase(param);
				
				break;
			}
			
			case SCR_ERadioMsg.PROMOTION_PRIVATE:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_POP;
				text = "#AR-Campaign_Promotion-UC";
				text2 = "#AR-Rank_WestPrivate";
				break;
			}
			
			case SCR_ERadioMsg.PROMOTION_CORPORAL:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_POC;
				text = "#AR-Campaign_Promotion-UC";
				text2 = "#AR-Rank_WestCorporal";
				break;
			}
			
			case SCR_ERadioMsg.PROMOTION_SERGEANT:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_POS;
				text = "#AR-Campaign_Promotion-UC";
				text2 = "#AR-Rank_WestSergeant";
				break;
			}
			
			case SCR_ERadioMsg.PROMOTION_LIEUTENANT:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_POL;
				text = "#AR-Campaign_Promotion-UC";
				text2 = "#AR-Rank_WestLieutenant";
				break;
			}
			
			case SCR_ERadioMsg.PROMOTION_CAPTAIN:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_PON;
				text = "#AR-Campaign_Promotion-UC";
				text2 = "#AR-Rank_WestCaptain";
				break;
			}
			
			case SCR_ERadioMsg.PROMOTION_MAJOR:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_POM;
				text = "#AR-Campaign_Promotion-UC";
				text2 = "#AR-Rank_WestMajor";
				break;
			}
			
			case SCR_ERadioMsg.REINFORCEMENTS:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_RIF;
				text = "#AR-Campaign_ReinforcementsArrival-UC";
				text2 = "#AR-Campaign_ReinforcementsArrivalDetails";
				param1 = base.GetBaseNameUpperCase();
				GetGame().GetCallqueue().CallLater(ShowHint, 8500, false, SCR_ECampaignHints.REINFORCEMENTS);
				break;
			}
			
			case SCR_ERadioMsg.VICTORY:
			{
				SCR_CampaignFactionManager fManager = SCR_CampaignFactionManager.GetInstance();

				if (!fManager)
					return;
				
				SCR_CampaignFaction f = SCR_CampaignFaction.Cast(fManager.GetFactionByIndex(param));
				
				if (!f || f != SCR_RespawnSystemComponent.GetLocalPlayerFaction())
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_PMV;
				break;
			}
			
			case SCR_ERadioMsg.WINNING:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_PMC;
				break;
			}
			
			case SCR_ERadioMsg.LOSING:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_PML;
				break;
			}
			
			case SCR_ERadioMsg.DEFEAT:
			{
				SCR_CampaignFactionManager fManager = SCR_CampaignFactionManager.GetInstance();
				
				if (!fManager)
					return;
				
				SCR_CampaignFaction f = SCR_CampaignFaction.Cast(fManager.GetFactionByIndex(param));
				
				if (!f || f == SCR_RespawnSystemComponent.GetLocalPlayerFaction())
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_PMD;			
				break;
			}
			
			case SCR_ERadioMsg.RELAY:
			{
				msgName = SCR_SoundEvent.SOUND_SL_RRD;
				break;
			}
			
			case SCR_ERadioMsg.REQUEST_EVAC:
			{
				msgName = SCR_SoundEvent.SOUND_SL_ERT;
				break;
			}
			
			case SCR_ERadioMsg.REQUEST_FUEL:
			{
				msgName = SCR_SoundEvent.SOUND_SL_RRT;
				break;
			}
			
			case SCR_ERadioMsg.SUPPLIES:
			{
				msgName = SCR_SoundEvent.SOUND_SL_SDD;
				break;
			}
			
			case SCR_ERadioMsg.REQUEST_REINFORCEMENTS:
			{
				msgName = SCR_SoundEvent.SOUND_SL_REI;
				break;
			}
			
			case SCR_ERadioMsg.REQUEST_TRANSPORT:
			{
				msgName = SCR_SoundEvent.SOUND_SL_TRT;
				break;
			}
			
			case SCR_ERadioMsg.CONFIRM:
			{
				msgName = SCR_SoundEvent.SOUND_SL_CSR;
				break;
			}
			
			case SCR_ERadioMsg.TASK_ASSIGN_SEIZE:
			{
				msgName = SCR_SoundEvent.SOUND_SL_SRT;
				break;
			}
			
			case SCR_ERadioMsg.TASK_UNASSIGN_REFUEL:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_RRU;
				break;
			}
			
			case SCR_ERadioMsg.TASK_UNASSIGN_TRANSPORT:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_TRU;
				break;
			}
			
			case SCR_ERadioMsg.TASK_UNASSIGN_EVAC:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_ETU;
				break;
			}
			
			case SCR_ERadioMsg.TASK_CANCEL_REQUEST:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_RCR;
				break;
			}
			
			case SCR_ERadioMsg.TASK_ASSIST:
			{
				msgName = SCR_SoundEvent.SOUND_SL_CHR;
				break;
			}
			
			case SCR_ERadioMsg.BASE_LOST:
			{
				msgName = SCR_SoundEvent.SOUND_HQ_BAL;
				break;
			}
			
			case SCR_ERadioMsg.BASE_UNDER_ATTACK:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_BUA;
				text = "#AR-Campaign_BaseUnderAttack-UC";
				
				if (base.GetType() == CampaignBaseType.RELAY)
					text2 = "%1";
				else
					text2 = "%1 (%2)";
				
				text2param1 = base.GetBaseName();
				text2param2 = base.GetCallsignDisplayName();
				
				if (m_FirstBaseWithPlayer == base)
					sound = SCR_SoundEvent.SOUND_SIREN;
				
				prio = SCR_ECampaignPopupPriority.BASE_UNDER_ATTACK;
				duration = 11;
				break;
			}
			
			case SCR_ERadioMsg.BUILT_ARMORY:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_BAA;
				text = "#AR-Campaign_Building_Available-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_Armory-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.BUILT_FUEL:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_BFA;
				text = "#AR-Campaign_Building_Available-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_FuelDepot-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.BUILT_REPAIR:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_BRA;
				text = "#AR-Campaign_Building_Available-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_RepairDepot-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.BUILT_SUPPLY:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_SCB;
				text = "#AR-Campaign_Building_Available-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_SupplyStorage-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.BUILT_VEHICLES_LIGHT:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_LCB;
				text = "#AR-Campaign_Building_Available-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_LightVehicleDepot-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.BUILT_VEHICLES_HEAVY:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_VCB;
				text = "#AR-Campaign_Building_Available-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_HeavyVehicleDepot-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.BUILT_BARRACKS:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_BCB;
				text = "#AR-Campaign_Building_Available-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_LivingQuarters-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.BUILT_ANTENNA:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_ACB;
				text = "#AR-Campaign_Building_Available-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_Antenna-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.BUILT_FIELD_HOSPITAL:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_HCB;
				text = "#AR-Campaign_Building_Available-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_FieldHospital-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.DESTROYED_ARMORY:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_BAD;
				text = "#AR-Campaign_Building_Destroyed-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_Armory-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.DESTROYED_FUEL:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_BFD;
				text = "#AR-Campaign_Building_Destroyed-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_FuelDepot-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.DESTROYED_REPAIR:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_BRD;
				text = "#AR-Campaign_Building_Destroyed-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_RepairDepot-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.REPAIRED_ARMORY:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_BAR;
				text = "#AR-Campaign_Building_Available-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_Armory-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.REPAIRED_FUEL:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_BFR;
				text = "#AR-Campaign_Building_Available-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_FuelDepot-UC";
				duration = 5;
				break;
			}
			
			case SCR_ERadioMsg.REPAIRED_REPAIR:
			{
				if (!base)
					return;
				
				msgName = SCR_SoundEvent.SOUND_HQ_BRR;
				text = "#AR-Campaign_Building_Available-UC";
				text2 = base.GetBaseName();
				param1 = "#AR-Campaign_Building_RepairDepot-UC";
				duration = 5;
				break;
			}
		}
				
		if (!msgName.IsEmpty())
		{
			AudioSystem.TerminateSound(m_PlayedRadio);
			m_PlayedRadio = soundComp.SoundEvent(msgName);
		}
		
		if (!text.IsEmpty() || !text2.IsEmpty())
			SCR_PopUpNotification.GetInstance().PopupMsg(text, duration, text2: text2, prio: prio, param1: param1, sound: sound, text2param1: text2param1, text2param2: text2param2);
	}
	
	//------------------------------------------------------------------------------------------------
	Resource GetRemnantsGroupPrefab()
	{
		return m_rRemnants;
	}
	
	//------------------------------------------------------------------------------------------------
	//An event that is typically called from a SCR_CampaignBase that has been captured
	void OnBaseCaptured(SCR_CampaignBase capturedBase)
	{
#ifdef ENABLE_DIAG
		if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_EXECUTE_GAMEMODE))
			return;
#endif
		
		if (!m_bMatchOver && !IsProxy())
			EvaluateGame();
		
		capturedBase.SendHQMessageBaseCaptured();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Checks if the session is run as client
	bool IsProxy()
	{
		return (m_RplComponent && m_RplComponent.IsProxy());
	}
	//------------------------------------------------------------------------------------------------
	//! Getter for "Rank required" parameter for spawning vehicles.
	// TRUE, if rank requirement is disabled
	bool CanRequestWithoutRank()
	{
		return m_bIgnoreMinimumVehicleRank;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Adds the spawned vehicle to an array handled by garbage collector etc.
	//! \param veh The spawned vehicle
	void RegisterSpawnedVehicle(notnull Vehicle veh)
	{
		m_aRequestedVehicles.Insert(veh);
	}
	
	//------------------------------------------------------------------------------------------------
	void AddBaseWithPlayer(notnull SCR_CampaignBase base, notnull ChimeraCharacter player)
	{
		if (m_aBasesWithPlayer.Find(base) == -1)
		{
			Event_OnPlayerEnterBase.Invoke(base);
			m_aBasesWithPlayer.Insert(base);
			base.ToggleRadioChatter(true);
		}
		
		// Make sure the base closest to player is the first element
		int index;
		int minCheckedDistance = int.MAX;
		int basesCnt = m_aBasesWithPlayer.Count();
		vector playerPos = player.GetOrigin();
		
		for (int i = 0; i < basesCnt; i++)
		{
			int distance = vector.DistanceSq(m_aBasesWithPlayer[i].GetOrigin(), playerPos);
			
			if (distance < minCheckedDistance)
			{
				minCheckedDistance = distance;
				index = i;
			}
		}	
			
		if (basesCnt > 1 && index != 0)
			m_aBasesWithPlayer.SwapItems(0, index);
	}
	
	//------------------------------------------------------------------------------------------------
	void RemoveBaseWithPlayer(notnull SCR_CampaignBase base)
	{
		m_aBasesWithPlayer.RemoveItem(base);
		base.ToggleRadioChatter(false);
		Event_OnPlayerLeftBase.Invoke(base);
	}
	
	ScriptInvoker GetOnPlayerEnterBase()
	{
		return Event_OnPlayerEnterBase;
	}
	
	ScriptInvoker GetOnPlayerLeftBase()
	{
		return Event_OnPlayerLeftBase;
	}
	
	//------------------------------------------------------------------------------------------------
	SCR_CampaignBase GetBasePlayerPresence(out array<SCR_CampaignBase> basesWithPlayer = null)
	{
		if (basesWithPlayer)
			basesWithPlayer = m_aBasesWithPlayer;
		
		if (m_aBasesWithPlayer.Count() == 0)
			return null;
		else
			return m_aBasesWithPlayer[0];
	}
	
	//------------------------------------------------------------------------------------------------
	bool IsPlayerInBase(notnull SCR_CampaignBase base)
	{
		return m_aBasesWithPlayer.Find(base) != -1;
	}
	
	//------------------------------------------------------------------------------------------------
	ResourceName GetBuildingIconsImageset()
	{
		return m_sBuildingIconImageset;
	}
	
	//------------------------------------------------------------------------------------------------
	ResourceName GetHUDIconLayout()
	{
		return m_sHUDIcon;
	}	
	
	//------------------------------------------------------------------------------------------------
	bool GetApplyPresetOwners()
	{
		return m_bApplyPresetOwners;
	}
	
	//------------------------------------------------------------------------------------------------
	void CheckForBasesWithPlayer()
	{
		IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(SCR_PlayerController.GetLocalPlayerId());
		
		if (!player)
		{
			m_FirstBaseWithPlayer = null;
			m_LastVisitedBase = null;
			return;
		}
		
		CharacterControllerComponent comp = CharacterControllerComponent.Cast(player.FindComponent(CharacterControllerComponent));
		
		if (!comp || comp.IsDead())
		{
			m_FirstBaseWithPlayer = null;
			m_LastVisitedBase = null;
			return;
		}
		
		array<SCR_CampaignBase> basesWithPlayer = new array<SCR_CampaignBase>();
		m_FirstBaseWithPlayer = GetBasePlayerPresence(basesWithPlayer);
		
		if (m_FirstBaseWithPlayer)
		{
			foreach (SCR_CampaignBase base: basesWithPlayer)
				if (base.GetCapturingFaction())
					m_FirstBaseWithPlayer = base;
		}
		else
		{
			m_LastVisitedBase = null;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	array<SCR_CampaignBase> GetBasesInRangeOfMobileHQ(notnull IEntity mobileHQ)
	{
		array<SCR_CampaignBase> bases = {};
		vector mobileHQPos = mobileHQ.GetOrigin();
		float mobileHQRadioRangeSq;
		IEntity truck = mobileHQ.GetParent();
		
		BaseRadioComponent comp;
		
		if (truck)
			comp = BaseRadioComponent.Cast(truck.FindComponent(BaseRadioComponent));
		
		if (!comp)
			return bases;
		
		mobileHQRadioRangeSq = Math.Pow(comp.GetRange(), 2);
		
		foreach (SCR_CampaignBase base: SCR_CampaignBaseManager.GetBases())
		{
			if (!base)
				continue;
			
			if (!base.GetIsEnabled())
				continue;
			
			float dist = vector.DistanceSq(mobileHQPos, base.GetOrigin());
			
			if (dist < mobileHQRadioRangeSq)
				bases.Insert(base);
		}
		
		return bases;
	}
	
	//------------------------------------------------------------------------------------------------
	void CheckMobileAssemblies()
	{
		SCR_CampaignFactionManager fManager = SCR_CampaignFactionManager.GetInstance();
		
		if (!fManager)
			return;
		
		array<Faction> factions = new array<Faction>();
		fManager.GetFactionsList(factions);
		
		foreach (Faction f: factions)
		{
			if (!f)
				continue;
			
			SCR_CampaignFaction factionC = SCR_CampaignFaction.Cast(f);
			
			if (!factionC)
				continue;
			
			SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
		
			if (!campaign)
				return;
			
			if (!campaign.IsMobileAssemblyDeployed(factionC))
				continue;
			
			IEntity assembly = factionC.GetDeployedMobileAssembly();
			FactionKey factionCKey = factionC.GetFactionKey();
			
			if (!assembly)
			{
				campaign.SetDeployedMobileAssemblyID(factionCKey, RplId.Invalid());
				continue;
			}
			
			DamageManagerComponent damageComponent = DamageManagerComponent.Cast(assembly.FindComponent(DamageManagerComponent));
			
			if (damageComponent && damageComponent.GetState() == EDamageState.DESTROYED)
			{
				SCR_CampaignMobileAssemblyComponent comp = SCR_CampaignMobileAssemblyComponent.Cast(assembly.FindComponent(SCR_CampaignMobileAssemblyComponent));
				
				if (comp)
					comp.Deploy(false);
				
				Rpc(RpcDo_MobileAssemblyFeedback, ECampaignClientNotificationID.ASSEMBLY_DESTROYED, -1, comp.GetParentFactionID());
				
				if (RplSession.Mode() != RplMode.Dedicated)
					RpcDo_MobileAssemblyFeedback(ECampaignClientNotificationID.ASSEMBLY_DESTROYED, -1, comp.GetParentFactionID());
				
				continue;
			}
			
			IEntity truck = assembly.GetParent();
			Physics physicsComponent;
			vector vel;
			
			if (truck)
			{
				physicsComponent = truck.GetPhysics();
				vel = physicsComponent.GetVelocity();
				vel[1] = 0;
			}
			
			if (physicsComponent && vel.LengthSq() > 0.01)
			{
				SCR_CampaignMobileAssemblyComponent comp = SCR_CampaignMobileAssemblyComponent.Cast(assembly.FindComponent(SCR_CampaignMobileAssemblyComponent));
				
				if (comp)
					comp.Deploy(false);
				
				Rpc(RpcDo_MobileAssemblyFeedback, ECampaignClientNotificationID.ASSEMBLY_DISMANTLED, -1, comp.GetParentFactionID());
				
				if (RplSession.Mode() != RplMode.Dedicated)
					RpcDo_MobileAssemblyFeedback(ECampaignClientNotificationID.ASSEMBLY_DISMANTLED, -1, comp.GetParentFactionID());
				
				continue;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void SetDeployedMobileAssemblyID(FactionKey faction, RplId ID)
	{
		switch (faction)
		{
			case FACTION_BLUFOR: {m_DeployedMobileAssemblyIDWest = ID; OnMobileAssemblyDeployChangedWest(); break;};
			case FACTION_OPFOR: {m_DeployedMobileAssemblyIDEast = ID; OnMobileAssemblyDeployChangedEast(); break;};
		}
		
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	RplId GetDeployedMobileAssemblyID(FactionKey faction)
	{
		switch (faction)
		{
			case FACTION_BLUFOR: {return m_DeployedMobileAssemblyIDWest;};
			case FACTION_OPFOR: {return m_DeployedMobileAssemblyIDEast;};
		}
		
		return RplId.Invalid();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Show info on current XP progress
	void ShowXPInfo(int totalXP, CampaignXPRewards rewardID, int XP, bool volunteer, bool profileUsed, int skillLevel)
	{
		if (rewardID == CampaignXPRewards.UNDEFINED)
			return;
		
		SCR_CampaignFaction faction = SCR_CampaignFaction.Cast(SCR_RespawnSystemComponent.GetLocalPlayerFaction());
			
		if (!faction)
			return;
		
		SCR_CampaignFactionManager campaignFactionManager = SCR_CampaignFactionManager.GetInstance();
		ECharacterRank curRank = campaignFactionManager.GetRankByXP(totalXP);
		ECharacterRank prevRank = campaignFactionManager.GetRankByXP(totalXP - XP);
		string rankText = faction.GetRankName(curRank);
		ResourceName rankIconName = faction.GetRankInsignia(curRank);
		
		if (!m_wXPInfo && GetGame().GetHUDManager())
		{
			m_wXPInfo = GetGame().GetHUDManager().CreateLayout("{E0B82B4FCC95EE05}UI/layouts/HUD/CampaignMP/RankProgress.layout", EHudLayers.MEDIUM, 0);
			m_wXPInfo.SetVisible(false);
		}
		
		if (!m_wXPInfo)
			return;
		
		TextWidget title = TextWidget.Cast(m_wXPInfo.FindWidget("Title"));
		TextWidget rank = TextWidget.Cast(m_wXPInfo.FindWidget("Rank"));
		TextWidget rankNoIcon = TextWidget.Cast(m_wXPInfo.FindWidget("RankNoIcon"));
		ImageWidget rankIcon = ImageWidget.Cast(m_wXPInfo.FindWidget("RankIcon"));
		TextWidget skill = TextWidget.Cast(m_wXPInfo.FindWidget("Skill"));
		
		if (rankIconName.IsEmpty())
		{
			rankNoIcon.SetTextFormat(rankText);
			rankIcon.SetVisible(false);
			rank.SetTextFormat("");
		}
		else
		{
			rank.SetTextFormat(rankText);
			rankIcon.LoadImageTexture(0, rankIconName);
			rankIcon.SetColor(Color.FromRGBA(226,168,79,255));
			rankIcon.SetVisible(true);
			rankNoIcon.SetTextFormat("");
		}
		
		int showXP = XP;
		
		// Show XP progress bar
		ProgressBarWidget progress = ProgressBarWidget.Cast(m_wXPInfo.FindWidget("Progress"));
		ProgressBarWidget progressDiff = ProgressBarWidget.Cast(m_wXPInfo.FindWidget("ProgressDiff"));
		
		if (XP > 0)
		{
			title.SetColor(Color.FromRGBA(27, 194, 98, 255));
			progressDiff.SetColor(Color.FromRGBA(27, 194, 98, 255));
			m_bNegativeXP = false;
		}
		else
		{
			title.SetColor(Color.FromRGBA(255, 72, 74, 255));
			progressDiff.SetColor(Color.FromRGBA(255, 72, 74, 255));
			m_bNegativeXP = true;
		}
		
		if (campaignFactionManager.GetRankNext(curRank) == ECharacterRank.INVALID)	// Player at max level, no gain to show
		{
			progress.SetMin(0);
			progress.SetMax(1);
			progress.SetCurrent(1);
			progressDiff.SetMin(0);
			progressDiff.SetMax(1);
			progressDiff.SetCurrent(0);
		}
		else
		{
			if (campaignFactionManager.GetRankPrev(curRank) == ECharacterRank.INVALID && XP < 0)	// Player is renegade and losing XP, just show red bar
			{
				progress.SetMin(0);
				progress.SetMax(1);
				progress.SetCurrent(0);
				progressDiff.SetMin(0);
				progressDiff.SetMax(1);
				progressDiff.SetCurrent(1);
			}
			else
			{
				int XPCurRank = campaignFactionManager.GetRankXP(curRank);
				int XPNextRank = campaignFactionManager.GetRankXP(campaignFactionManager.GetRankNext(curRank));
					
				if (curRank == prevRank)
				{
					if (campaignFactionManager.GetRankPrev(curRank) != ECharacterRank.INVALID)	// Standard XP change
					{
						progress.SetMin(XPCurRank);
						progress.SetMax(XPNextRank);
						progressDiff.SetMin(XPCurRank);
						progressDiff.SetMax(XPNextRank);
					}
					else
					{
						progress.SetMin(totalXP - XP);	// XP change as renegade, show progress towards a normal rank from current XP
						progress.SetMax(totalXP - XP + 1);
						progressDiff.SetMin(totalXP - XP);
						progressDiff.SetMax(XPNextRank);
					}
					
					if (XP > 0)	// XP gain
					{
						if (rewardID == CampaignXPRewards.ENEMY_KILL || rewardID == CampaignXPRewards.ENEMY_KILL_VEH)
							progress.SetCurrent(totalXP);
						else
						{
							progress.SetCurrent(totalXP - XP);
							progressDiff.SetCurrent(totalXP);
						}
					}
					else	// XP loss
					{
						progress.SetCurrent(totalXP);
						progressDiff.SetCurrent(totalXP - XP);
					}
				}
				else
				{
					if (curRank > prevRank)	// Promotion
					{
						progress.SetMin(0);
						progress.SetMax(1);
						progress.SetCurrent(0);
						progressDiff.SetMin(XPCurRank);
						progressDiff.SetMax(XPNextRank);
						progressDiff.SetCurrent(totalXP);
					}
					else	// Demotion
					{
						progress.SetMin(XPCurRank);
						progress.SetMax(XPNextRank);
						progress.SetCurrent(totalXP);
						progressDiff.SetMin(0);
						progressDiff.SetMax(1);
						progressDiff.SetCurrent(1);
					}
				}
			}
		}
		
		// Show skill info
		if (XP > 0 && profileUsed)
		{
			LocalizedString skillName;
			
			switch (GetXPRewardSkill(rewardID))
			{
				case EProfileSkillID.WEAPON_HANDLER: {skillName = "#AR-Campaign_SkillWeaponSpecialist"; break;};
				case EProfileSkillID.DRIVER: {skillName = "#AR-Campaign_SkillDriver"; break;};
				case EProfileSkillID.SCOUT: {skillName = "#AR-Campaign_SkillScout"; break;};
				case EProfileSkillID.OPERATOR: {skillName = "#AR-Campaign_SkillOperator"; break;};
			}
			
			if (!skillName.IsEmpty())
				skill.SetTextFormat("#AR-Campaign_LevelInfo", skillName, skillLevel);
		}
		
		if (volunteer)
			title.SetTextFormat("#AR-Campaign_RewardBonus_Volunteer", GetXPRewardName(rewardID));
		else
			title.SetTextFormat(GetXPRewardName(rewardID));
		
		if (rewardID != CampaignXPRewards.ENEMY_KILL && rewardID != CampaignXPRewards.ENEMY_KILL_VEH)
		{
			m_wXPInfo.SetVisible(true);
			m_fHideXPInfo = GetWorld().GetWorldTime() + XP_INFO_DURATION;
		}
	}

	//------------------------------------------------------------------------------------------------
	ResourceName GetSlotPresetResourceName(SCR_SiteSlotEntity slot, SCR_CampaignFaction givenFaction = null)
	{
		SCR_BuildingDedicatedSlotData slotData;		
		bool found = m_aDedicatedSlots.Find(slot, slotData);
		SCR_CampaignFaction faction;
		
		if (givenFaction)
			faction = givenFaction;
		else
			faction = SCR_CampaignFaction.Cast(SCR_RespawnSystemComponent.GetLocalPlayerFaction());
		
		if (!faction || !found || !slotData)
			return ResourceName.Empty;
		
		return faction.GetBuildingPrefab(slotData.GetCompositionType());
	}
	
	//------------------------------------------------------------------------------------------------
	SCR_CampaignBase GetSlotPresetBase(SCR_SiteSlotEntity slot)
	{
		SCR_BuildingDedicatedSlotData slotData;
		bool found = m_aDedicatedSlots.Find(slot, slotData);
		if (!found || !slotData)
			return null;
		
		return slotData.GetBase();
	}
	
	//------------------------------------------------------------------------------------------------
	bool GetIsMatchOver()
	{
		return m_bMatchOver;
	}
	
	//------------------------------------------------------------------------------------------------
	void RegisterRemnantSupplyDepot(notnull SCR_CampaignSuppliesComponent comp)
	{
		m_aRemnantSupplyDepots.Insert(comp);
	}
	
	//------------------------------------------------------------------------------------------------
	void CheckPlayerPresenceInSupplyDepots()
	{
		m_SupplyDepotWithPlayer = null;
		
		ChimeraCharacter localPlayer = ChimeraCharacter.Cast(SCR_PlayerController.GetLocalControlledEntity());
		
		if (!localPlayer)
			return;
		
		foreach (SCR_CampaignSuppliesComponent depot : m_aRemnantSupplyDepots)
		{
			if (!depot)
				continue;
			
			float maxDistance = Math.Pow(depot.GetOperationalRadius(), 2);
			
			if (maxDistance > vector.DistanceSqXZ(localPlayer.GetOrigin(), depot.GetOwner().GetOrigin()))
			{
				m_SupplyDepotWithPlayer = depot;
				return;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	SCR_CampaignSuppliesComponent GetSupplyDepotWithPlayer()
	{
		return m_SupplyDepotWithPlayer;
	}
	
	//------------------------------------------------------------------------------------------------
	void RegisterRemnantsPresence(notnull SCR_CampaignRemnantsSpawnPoint spawnpoint)
	{
		SCR_Faction remnantsFaction = SCR_Faction.Cast(GetGame().GetFactionManager().GetFactionByKey(FACTION_BLUFOR));
		
		if (!remnantsFaction)
			return;
		
		SCR_CampaignRemnantsPresence presence;
		
		if (spawnpoint.GetGroupType() == SCR_CampaignRemnantsGroupType.RANDOM)
		{
			// Randomize group type
			Math.Randomize(-1);
			float seed = Math.RandomFloat01();
			array<ResourceName> groups = {};
			
//			remnantsFaction.GetRemnantsByProbability(seed, groups);
			
			if (groups.IsEmpty())
				return;
			
			presence = new SCR_CampaignRemnantsPresence;
			presence.SetGroupPrefab(groups.GetRandomElement());
		}
		else
		{
			//load more prefabs and randomise 
			
			ResourceName prefabFireTeamWaste_MoneySmall = "{7A6C44293D9554E5}Prefabs/Groups/BLUFOR/Group_US_FireTeamWaste_SmallMoneyLoot.et";
			ResourceName prefabFireTeamWaste_MoneyMedium = "{9D23639EA3055572}Prefabs/Groups/BLUFOR/Group_US_FireTeamWaste_MediumMoneyLoot.et";
			ResourceName prefabFireTeamWaste_MoneyBig = "{DD8F91EF241595A0}Prefabs/Groups/BLUFOR/Group_FireTeamWaste_BigMoneyLoot.et";

			ResourceName prefabFireTeamWaste_ChosenPrefab;

			int no = Math.RandomInt(1,10);
			//1-3
			if (no <= 3)
			{
				prefabFireTeamWaste_ChosenPrefab = prefabFireTeamWaste_MoneyMedium;
			}
			//4-8
			if (no > 3 && no <= 8)
			{
				prefabFireTeamWaste_ChosenPrefab = prefabFireTeamWaste_MoneySmall;
			}
			//9-10
			if (no > 8)
			{
				prefabFireTeamWaste_ChosenPrefab = prefabFireTeamWaste_MoneySmall;
			}
			if (prefabFireTeamWaste_ChosenPrefab.IsEmpty())
				return;
			
			
			
			
			
			//5 minutes default respawn
			int respawnPeriodInSeconds = 300;
			//debug prefab {244A2FC361BACED6}Prefabs/Groups/BLUFOR/Group_US_FireTeamWaste_Debug.et
			//AI Presence Creation
			//prefabFireTeamWaste_ChosenPrefab = "{244A2FC361BACED6}Prefabs/Groups/BLUFOR/Group_US_FireTeamWaste_Debug.et";
			presence = new SCR_CampaignRemnantsPresence;
			presence.SetGroupPrefab(prefabFireTeamWaste_ChosenPrefab);
			presence.SetRespawnPeriod(respawnPeriodInSeconds);
			
			//fill the respawn array
			//brief: expects SCR_Positions near the spawn point!
			/////////////////////////////////////
			//////////////IMPORTANT//////////////
			/////////////////////////////////////
			//brief: entity naming convention for SCR_Position objecst:
			//-----> "SP_AI_%location%"
			
			//set MapDescriptor reference in presence 
			SCR_MapDescriptorComponent mdc = SCR_MapDescriptorComponent.Cast(spawnpoint.FindComponent(SCR_MapDescriptorComponent));
			presence.setMapDescriptor(mdc);
			presence.SetSpawnPointSP(spawnpoint);
			
			
			spawnpoint.FillRespawns();
			spawnpoint.FillPresence(presence);
		}
		
		vector locationCenter;
		array<SCR_CampaignRemnantsSpawnPoint> spawnpoints = {};
		array<vector> respawns = {};
		SCR_CampaignRemnantsSpawnPoint initialSpawnpoint;
		
		if (spawnpoint.GetChildren())
		{
			SCR_CampaignRemnantsSpawnPoint childSpawnPoint = SCR_CampaignRemnantsSpawnPoint.Cast(spawnpoint.GetChildren());
			while (childSpawnPoint)
			{
				if (childSpawnPoint.GetIsRespawn())
					presence.AddRespawn(childSpawnPoint.GetOrigin());
				else
					spawnpoints.Insert(childSpawnPoint);
				
				childSpawnPoint = SCR_CampaignRemnantsSpawnPoint.Cast(childSpawnPoint.GetSibling());
			}
			spawnpoints.Insert(spawnpoint);
			
			initialSpawnpoint = spawnpoints.GetRandomElement();
			locationCenter = initialSpawnpoint.GetOrigin();
		}
		else
			locationCenter = spawnpoint.GetOrigin();
		
		presence.SetSpawnpoint(locationCenter);
		//save spawnPoint
		//presence.SetRespawnPeriod(1);
		
		// Spawn waypoints
		array<AIWaypoint> patrolWaypoints = {};
		array<int> waypointIndexes = {};
		int pointsCnt = 1;
		int lowestIndex = int.MAX;
		
		// First waypoint is spawned on the group spawnpoint
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = presence.GetSpawnpoint();
		AIWaypoint firstWP = AIWaypoint.Cast(GetGame().SpawnEntityPrefabLocal(Resource.Load(GetPatrolWaypointPrefab()), null, params));
		
		foreach (SCR_CampaignRemnantsSpawnPoint sp : spawnpoints)
		{
			if (sp == initialSpawnpoint)
				continue;
			
			vector pos = sp.GetOrigin();	
			locationCenter += pos;
			params = EntitySpawnParams();
			params.TransformMode = ETransformMode.WORLD;
			params.Transform[3] = pos;
			AIWaypoint wp = AIWaypoint.Cast(GetGame().SpawnEntityPrefabLocal(Resource.Load(GetPatrolWaypointPrefab()), null, params));
			
			int index = -1;
			// Sort waypoints by indexes
			if (wp)
			{	
				int indexNew = sp.GetWaypointIndex();
				
				for (int i = 0, cnt = waypointIndexes.Count(); i < cnt; i++)
				{
					if (indexNew <= waypointIndexes[i] && indexNew < lowestIndex)
					{
						index = i;
						lowestIndex = indexNew;
					}
				}
				
				if (index == -1)
				{	
					patrolWaypoints.Insert(wp);
					waypointIndexes.Insert(indexNew);
					lowestIndex = indexNew;
				}
				else
				{
					patrolWaypoints.InsertAt(wp, index);
					waypointIndexes.InsertAt(indexNew, index);
				}
			}
			
			pointsCnt++;
		}
		
		params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = presence.GetSpawnpoint();
		
		if (patrolWaypoints.IsEmpty())
		{
			SCR_DefendWaypoint defendWaypoint = SCR_DefendWaypoint.Cast(GetGame().SpawnEntityPrefabLocal(Resource.Load(GetDefendWaypointPrefab()), null, params));
			
			if (defendWaypoint)
				presence.SetWaypoint(defendWaypoint);
		}
		else
		{
			
			if (!patrolWaypoints.IsEmpty())
			{
				if (initialSpawnpoint.GetWaypointIndex() != -1)
				{
					int initialPos = initialSpawnpoint.GetWaypointIndex();
					
					if (initialPos > patrolWaypoints.Count() && initialPos != 0)
						initialPos = patrolWaypoints.Count()-1;
					
					patrolWaypoints.InsertAt(firstWP, initialPos);
					while (patrolWaypoints[0] != firstWP)
					{
						patrolWaypoints.Insert(patrolWaypoints[0]);
						patrolWaypoints.RemoveOrdered(0);
					}
				}
			}
			
			AIWaypointCycle cycleWaypoint = AIWaypointCycle.Cast(GetGame().SpawnEntityPrefabLocal(Resource.Load(GetCycleWaypointPrefab()), null, params));
			
			if (cycleWaypoint)
			{
				cycleWaypoint.SetWaypoints(patrolWaypoints);
				cycleWaypoint.SetRerunCounter(-1);
				presence.SetWaypoint(cycleWaypoint);
			}
		}
		
		// Calculate the central location from which player presence will be checked
		locationCenter[0] = locationCenter[0] / pointsCnt;
		locationCenter[1] = locationCenter[1] / pointsCnt;
		locationCenter[2] = locationCenter[2] / pointsCnt;
		
		presence.SetCenter(locationCenter);
		
		// Locate parent base
		int distLimit = Math.Pow(SCR_CampaignRemnantsPresence.PARENT_BASE_DISTANCE_THRESHOLD, 2);
		float minDistance = float.MAX;
		SCR_CampaignBase nearestBase;
		bool register = true;
		
		foreach (SCR_CampaignBase base: SCR_CampaignBaseManager.GetBases())
		{
			if (base.GetType() == CampaignBaseType.RELAY)
				continue;
			
			if (!base.GetIsEnabled())
				continue;
			
			float dist = vector.DistanceSqXZ(locationCenter, base.GetOrigin());
			
			if (dist > distLimit || dist > minDistance)
				continue;
			
			// Do not register remnants group if its parent base is pre-owned
			if (base.GetType() != CampaignBaseType.MAIN && (!GetApplyPresetOwners() || base.GetStartingBaseOwner().IsEmpty() || base.GetStartingBaseOwner() == FACTION_INDFOR))
			{
				nearestBase = base;
				minDistance = dist;
				continue;
			}
			else
			{
				register = false;
				break;
			}
		}
		
		if (register)
		{
			presence.SetID(m_iRemnantLocationsCnt + 1);
			m_aRemnantsPresence.Insert(presence);
			m_iRemnantLocationsCnt++;
			
			//if (nearestBase)
				//nearestBase.RegisterRemnants(presence);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected int GetRemainingRemnantsInfo(out notnull array<int> remnantsInfo)
	{
		int size;
		
		foreach (SCR_CampaignRemnantsPresence presence: m_aRemnantsPresence)
		{
			if (!presence)
				continue;
			
			SCR_AIGroup grp = presence.GetSpawnedGroup();
			
			if (presence.GetMembersAlive() < 0 && !grp && !presence.GetIsSpawned())
				continue;
			
			remnantsInfo.Insert(presence.GetID());
			
			if (grp)
				size = remnantsInfo.Insert(grp.GetAgentsCount());
			else
			{
				if (presence.GetIsSpawned())
					size = remnantsInfo.Insert(0);
				else
					size = remnantsInfo.Insert(presence.GetMembersAlive());
			}
			
			remnantsInfo.Insert(presence.GetRespawnTimestamp());
		}
		
		return size;
	}
	
	//------------------------------------------------------------------------------------------------
	void StoreRemnantsStates(out notnull array<ref SCR_CampaignRemnantInfoStruct> outEntries)
	{
		array<int> remnantsInfo = {};
		int cnt = GetRemainingRemnantsInfo(remnantsInfo);
		
		for (int i = 0; i < cnt; i++)
		{
			SCR_CampaignRemnantInfoStruct struct = new SCR_CampaignRemnantInfoStruct();
			struct.SetID(remnantsInfo[i]);
			struct.SetMembersAlive(remnantsInfo[i + 1]);
			struct.SetRespawnTimer(remnantsInfo[i + 2]);
			outEntries.Insert(struct);
			i += 2;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void LoadRemnantsStates(notnull array<ref SCR_CampaignRemnantInfoStruct> entries)
	{
		foreach (SCR_CampaignRemnantsPresence presence: m_aRemnantsPresence)
		{
			if (!presence)
				continue;
			
			foreach (SCR_CampaignRemnantInfoStruct info: entries)
			{
				if (info.GetID() == presence.GetID())
				{
					presence.SetMembersAlive(info.GetMembersAlive());
					presence.SetRespawnTimestamp(info.GetRespawnTimer());
				}
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void GetRequestedVehicles(out notnull array<Vehicle> vehicleList)
	{
		vehicleList = m_aRequestedVehicles;
	}
	
	//------------------------------------------------------------------------------------------------
	// ToDo: Remove once the old CampaignBuilding mode (old one) is removed
	void OnBuildingInterfaceClosed()
	{
	}
		
	//------------------------------------------------------------------------------------------------
	// Triggered each time player build a composition. Slot is the slot he used.
	void OnStructureBuilt(SCR_SiteSlotEntity slot, SCR_CampaignNetworkComponent networkComponent)
	{
		SCR_CampaignBase foundBase;
		array<ECampaignCompositionType> types = {};
		
		SCR_Enum.GetEnumValues(ECampaignCompositionType, types);
		
		foreach (SCR_CampaignBase base: SCR_CampaignBaseManager.GetBases())
		{
			if (!base)
				continue;
			
			if (!base.GetIsEnabled())
				continue;
			
			foreach (ECampaignCompositionType type : types)
			{
				if (base.GetAssignedSlot(type) == slot)
				{
					foundBase = base;
					break;
				}
			}
			
			if (foundBase)
				break;
		}
		
		if (!foundBase)
			return;
		
		IEntity structure = slot.GetOccupant();
		
		if (!structure)
			return;
		
		SCR_CampaignServiceComponent comp = SCR_CampaignServiceComponent.Cast(structure.FindComponent(SCR_CampaignServiceComponent));
		
		foundBase.RegisterAsParentBase(comp, true);
		s_OnServiceBuild.Invoke(foundBase, networkComponent);
	}
	
	//------------------------------------------------------------------------------------------------
	// Triggered each time player build a composition. Slot is the slot he used. SARGES TESTING METHOD
	void OnStructureBuilt(SCR_CampaignBase base, SCR_EditableEntityComponent entity, bool add)
	{	
		if (IsTutorial())
		{
			SCR_CampaignTutorialComponent tutorial = SCR_CampaignTutorialComponent.Cast(FindComponent(SCR_CampaignTutorialComponent));
			
			if (tutorial)
				tutorial.OnStructureBuilt(base, entity.GetOwner());
		}
		
		SCR_CampaignServiceComponent comp = SCR_CampaignServiceComponent.Cast(entity.GetOwner().FindComponent(SCR_CampaignServiceComponent));
		if (!comp)
			return;
		
		base.RegisterAsParentBase(comp, add);
		s_OnServiceBuild.Invoke(base);
	}
	
	//------------------------------------------------------------------------------------------------
	// Triggered each time when the strcture status (damaged / repaired) is changed. Base is the base to which the composition belongs to.
	void OnStructureChanged(notnull SCR_CampaignBase base, SCR_SiteSlotEntity slot, SCR_CampaignServiceComponent service, bool add)
	{
		bool found;
		SCR_BuildingDedicatedSlotData slotData;	
		ECampaignServicePointType serviceType;	
		
		if (slot)
		{
			found = m_aDedicatedSlots.Find(slot, slotData);
		
			if (!found)
				return;
			
			switch (slotData.GetCompositionType())
	        {
				//case ECampaignCompositionType.FUEL: {serviceType = ECampaignServicePointType.FUEL_DEPOT; break;}
				case ECampaignCompositionType.LIGHT_VEHICLE_DEPOT: {serviceType = ECampaignServicePointType.LIGHT_VEHICLE_DEPOT; break;}
				case ECampaignCompositionType.HEAVY_VEHICLE_DEPOT: {serviceType = ECampaignServicePointType.HEAVY_VEHICLE_DEPOT; break;}
				case ECampaignCompositionType.ARMORY: {serviceType = ECampaignServicePointType.ARMORY; break;}
				//case ECampaignCompositionType.HOSPITAL: {serviceType = ECampaignServicePointType.FIELD_HOSPITAL; break;}
				case ECampaignCompositionType.BARRACKS: {serviceType = ECampaignServicePointType.BARRACKS; break;}
			};
		}
		else if (service)
		{
			serviceType = service.GetType();
		}
		
		SCR_CampaignFaction owner = base.GetOwningFaction();
				
		if (add)
		{	
			if (owner && base.IsBaseInFactionRadioSignal(owner))
				switch (serviceType)
				{
					//case ECampaignServicePointType.FUEL_DEPOT: {owner.SendHQMessage(SCR_ERadioMsg.REPAIRED_FUEL, param: base.GetBaseID()); break;};
					case ECampaignServicePointType.ARMORY: {owner.SendHQMessage(SCR_ERadioMsg.REPAIRED_ARMORY, param: base.GetBaseID()); break;};
					case ECampaignServicePointType.LIGHT_VEHICLE_DEPOT: {owner.SendHQMessage(SCR_ERadioMsg.REPAIRED_REPAIR, param: base.GetBaseID()); break;};
					case ECampaignServicePointType.HEAVY_VEHICLE_DEPOT: {owner.SendHQMessage(SCR_ERadioMsg.REPAIRED_REPAIR, param: base.GetBaseID()); break;};
				}
		}
		else
		{	
			if (owner && base.IsBaseInFactionRadioSignal(owner))
				switch (serviceType)
				{
					//case ECampaignServicePointType.FUEL_DEPOT: {owner.SendHQMessage(SCR_ERadioMsg.DESTROYED_FUEL, param: base.GetBaseID()); break;};
					case ECampaignServicePointType.ARMORY: {owner.SendHQMessage(SCR_ERadioMsg.DESTROYED_ARMORY, param: base.GetBaseID()); break;};
					case ECampaignServicePointType.LIGHT_VEHICLE_DEPOT: {owner.SendHQMessage(SCR_ERadioMsg.DESTROYED_REPAIR, param: base.GetBaseID()); break;};
					case ECampaignServicePointType.HEAVY_VEHICLE_DEPOT: {owner.SendHQMessage(SCR_ERadioMsg.DESTROYED_REPAIR, param: base.GetBaseID()); break;};
				}
		}		
	}
		
	//------------------------------------------------------------------------------------------------
	// Time in seconds for how long only the player who requested the vehicle can get in as driver.
	int GetVehicleProtectionTime()
	{
		return m_iSpawnedVehicleTimeProtection;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetIsTutorial(bool isTutorial)
	{
		m_bIsTutorial = isTutorial;
		
		if (m_bIsTutorial)
			SCR_PopUpNotification.SetFilter(SCR_EPopupMsgFilter.TUTORIAL);
		else
			SCR_PopUpNotification.SetFilter(SCR_EPopupMsgFilter.ALL);
	}
	
	//------------------------------------------------------------------------------------------------
	bool IsTutorial()
	{
		return m_bIsTutorial;
	}
	
	//------------------------------------------------------------------------------------------------
	void AddActiveRespawnRadio(FactionKey faction, SCR_PlayerRadioSpawnPointCampaign processedSpawnpoint = null)
	{
		if (processedSpawnpoint)
		{
			if (m_aRadioSpawnPoints.Contains(processedSpawnpoint))
				return;
			
			m_aRadioSpawnPoints.Insert(processedSpawnpoint);
		}
		
		switch (faction)
		{
			case FACTION_BLUFOR: {m_iActiveRespawnRadiosCntWest++; break;};
			case FACTION_OPFOR: {m_iActiveRespawnRadiosCntEast++; break;};
		}
		
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	void RemoveActiveRespawnRadio(FactionKey faction, SCR_PlayerRadioSpawnPointCampaign processedSpawnpoint = null)
	{
		if (processedSpawnpoint)
		{
			int index = m_aRadioSpawnPoints.Find(processedSpawnpoint);
			
			if (index == -1)
				return;
		
			m_aRadioSpawnPoints.Remove(index);
		}
		
		bool reactivate = false;
		
		switch (faction)
		{
			case FACTION_BLUFOR: {if (m_iActiveRespawnRadiosCntWest == GetMaxRespawnRadios()) reactivate = true; m_iActiveRespawnRadiosCntWest--; break;};
			case FACTION_OPFOR: {if (m_iActiveRespawnRadiosCntEast == GetMaxRespawnRadios()) reactivate = true; m_iActiveRespawnRadiosCntEast--; break;};
		}
		
		Replication.BumpMe();
		
		// Check all players for radios if limit is no longer maxed, activate a dormant one
		if (reactivate)
		{
			array<SCR_SpawnPoint> allSpawnpoints = SCR_SpawnPoint.GetSpawnPoints();
			PlayerManager pMan = GetGame().GetPlayerManager();
			
			if (!pMan)
				return;
			
			foreach (SCR_SpawnPoint spawnpoint: allSpawnpoints)
			{
				if (spawnpoint == processedSpawnpoint)
					continue;
				
				SCR_PlayerRadioSpawnPointCampaign campaignSpawnpoint = SCR_PlayerRadioSpawnPointCampaign.Cast(spawnpoint);
				
				if (!campaignSpawnpoint)
					continue;
				
				if (campaignSpawnpoint.GetFactionKey() == faction)
					continue;
				
				SCR_ChimeraCharacter player = SCR_ChimeraCharacter.Cast(pMan.GetPlayerControlledEntity(campaignSpawnpoint.GetPlayerID()));
				
				if (!player)
					continue;
				
				CharacterControllerComponent charController = CharacterControllerComponent.Cast(player.FindComponent(CharacterControllerComponent));
				
				if (!charController)
					continue;
				
				if (charController.IsDead())
					continue;
				
				Faction playerFaction = player.GetFaction();
				
				if (!playerFaction)
					continue;
				
				if (playerFaction.GetFactionKey() != faction)
					continue;
				
				BaseLoadoutManagerComponent loadoutManager = BaseLoadoutManagerComponent.Cast(player.FindComponent(BaseLoadoutManagerComponent));
		
				if (!loadoutManager)
					continue;
				
				IEntity backpack = loadoutManager.GetClothByArea(LoadoutBackpackArea);
				
				if (!backpack || !backpack.FindComponent(SCR_RadioComponent))
					continue;
				
				SCR_CampaignBaseManager baseManager = SCR_CampaignBaseManager.GetInstance();
				
				if (!baseManager || !baseManager.IsEntityInFactionRadioSignal(player, playerFaction))
					continue;
				
				BaseLoadoutClothComponent loadoutCloth = BaseLoadoutClothComponent.Cast(backpack.FindComponent(BaseLoadoutClothComponent));
				
				if (loadoutCloth && loadoutCloth.GetAreaType().IsInherited(LoadoutBackpackArea))
				{
					campaignSpawnpoint.ActivateSpawnPointPublic();
					
					if (GetActiveRespawnRadiosCount(faction) == GetMaxRespawnRadios())
						return;
					else
						continue;
				}
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void SetMapCampaignUI(SCR_MapCampaignUI mapUi)
	{
		m_MapCampaignUI = mapUi;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetSpawnTime()
	{
		TimeAndWeatherManagerEntity manager = GetGame().GetTimeAndWeatherManager();
		
		if(manager)
				m_SpawnTime = manager.GetTime();
		else
			Print("Time And Weather manager not found", LogLevel.WARNING);
	}
	
	//------------------------------------------------------------------------------------------------
	TimeContainer GetSpawnTime()
	{
		return m_SpawnTime;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetActiveRespawnRadiosCount(FactionKey faction)
	{
		switch (faction)
		{
			case FACTION_BLUFOR: {return m_iActiveRespawnRadiosCntWest;};
			case FACTION_OPFOR: {return m_iActiveRespawnRadiosCntEast;};
		}
		
		return 0;
	}
	
	//************************//
	//PROTECTED MEMBER METHODS//
	//************************//
	
	//------------------------------------------------------------------------------------------------
	//! Returns faction of the player with the given ID
	Faction GetPlayerFaction(int playerID)
	{		
		if (s_RespawnSystemComponent)
			return s_RespawnSystemComponent.GetPlayerFaction(playerID);
		
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void CheckPlayerInsideRadioRange()
	{
		SCR_CampaignFactionManager fManager = SCR_CampaignFactionManager.GetInstance();
		if (!fManager) return;
		
		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		if (!player) return;
		
		SCR_CampaignFaction faction = SCR_CampaignFaction.Cast(SCR_RespawnSystemComponent.GetLocalPlayerFaction());
		if (!faction) return;
		
		SCR_CampaignBaseManager baseManager = SCR_CampaignBaseManager.GetInstance();
		if (!baseManager) return;
		
		bool isInRangeNow = baseManager.IsEntityInFactionRadioSignal(player, faction);
		
		if (isInRangeNow != m_bIsPlayerInRadioRange)
		{
			m_bIsPlayerInRadioRange = isInRangeNow;
			
			if (isInRangeNow)
				SCR_PopUpNotification.GetInstance().PopupMsg("#AR-Campaign_RadioRangeEntered-UC", duration: 3);
			else
				SCR_PopUpNotification.GetInstance().PopupMsg("#AR-Campaign_RadioRangeLeft-UC", duration: 3);
			
			SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_RADIO_ROGERBEEP);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void CheckRadioSpawnpointsSignalCoverage()
	{
		array<SCR_SpawnPoint> spawnpoints = SCR_SpawnPoint.GetSpawnPoints();
		SCR_CampaignFactionManager factionM = SCR_CampaignFactionManager.GetInstance();
		SCR_CampaignBaseManager baseManager = SCR_CampaignBaseManager.GetInstance();
		
		if (!baseManager || !factionM)
			return;
		
		foreach (SCR_SpawnPoint spawnpoint: spawnpoints)
		{	
			SCR_PlayerRadioSpawnPointCampaign spawnpointC = SCR_PlayerRadioSpawnPointCampaign.Cast(spawnpoint);
			
			if (!spawnpointC)
				continue;
			
			if (spawnpointC.GetFlags() & EntityFlags.STATIC)
				continue;
			
			if (spawnpointC.GetOrigin() == vector.Zero)
				continue;
			
			Faction faction = spawnpointC.GetCachedFaction();
			
			if (!faction)
				continue;
			
			bool isInRange = baseManager.IsEntityInFactionRadioSignal(spawnpointC, faction);
			
			if (isInRange)
				spawnpointC.SetFaction(faction);
			else
				spawnpointC.SetFaction(null);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void ShowHint(SCR_ECampaignHints hintID)
	{
		if (m_bIsTutorial)
			return;
		
		switch (hintID)
		{
			case SCR_ECampaignHints.SIGNAL:
			{	
				if (!m_bStartupHintsShown)
				{
					m_bStartupHintsShown = true;
					SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_Signal_Text", "#AR-Campaign_GamemodeName", 30, fieldManualEntry: EFieldManualEntryId.CONFLICT_OVERVIEW);
					GetGame().GetCallqueue().CallLater(ShowHint, 32000, false, SCR_ECampaignHints.SERVICES);
				}
				else
				{
					if (!m_bRespawnHintShown)
					{
						m_bRespawnHintShown = true;
						ShowHint(SCR_ECampaignHints.RESPAWN);
					}
				}
					
				return;
			}
			
			case SCR_ECampaignHints.SERVICES:
			{
				string hintString = "#AR-Campaign_Hint_ServicesText <h1 align='center' scale='4'><color rgba='0,112,20,255'><image set='{F7E8D4834A3AFF2F}UI/Imagesets/Conflict/conflict-icons-bw.imageset' name='Slot_Supplies'/></color></h1>";
				SCR_HintManagerComponent.ShowCustomHint(hintString, "#AR-Campaign_Hint_Services_Title", 30, fieldManualEntry: EFieldManualEntryId.CONFLICT_SERVICES);
				return;
			}
			
			case SCR_ECampaignHints.RESPAWN:
			{
				SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_Respawn_Text", "#AR-Campaign_Hint_Respawn_Title", 20, fieldManualEntry: EFieldManualEntryId.CONFLICT_RESPAWN);
				return;
			}
			
			case SCR_ECampaignHints.SEIZING:
			{
				SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_EnteringEnemyBase_Presence_Text", "#AR-Campaign_EnteringEnemyBase", 15, fieldManualEntry: EFieldManualEntryId.CONFLICT_SEIZING_BASES);
				m_iBaseSeizingHintsShown++;
				return;
			}
			
			case SCR_ECampaignHints.RECONFIGURING:
			{
				SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_EnteringEnemyRelay_Text", "#AR-EditableEntity_TransmitterTower_01_Name", 15);
				return;
			}
			
			case SCR_ECampaignHints.SUPPLY_RUNS:
			{
				SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_SupplyRun_Text", "#AR-Campaign_Hint_SupplyRun_Title", 30, fieldManualEntry: EFieldManualEntryId.CONFLICT_SUPPLIES);
				return;
			}
			
			case SCR_ECampaignHints.SUPPLIES_UNLOADING:
			{
				SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_SupplyRunUnload_Text", "#AR-Campaign_Hint_SupplyRun_Title", 10, fieldManualEntry: EFieldManualEntryId.CONFLICT_SUPPLIES);
				m_iSuppliesHintsShown++;
				return;
			}
			
			case SCR_ECampaignHints.KILL_XP:
			{
				SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_KillXP_Text", "#AR-Campaign_Hint_KillXP_Title", 10, fieldManualEntry: EFieldManualEntryId.CONFLICT_RANKS);
				return;
			}
			
			case SCR_ECampaignHints.MOBILE_ASSEMBLY:
			{
				SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_MobileAssembly_Text", "#AR-Vehicle_MobileAssembly_Name", 30, fieldManualEntry: EFieldManualEntryId.CONFLICT_MHQ);
				return;
			}
			
			case SCR_ECampaignHints.REINFORCEMENTS:
			{
				if (!m_bReinforcementsHintShown)
				{
					m_bReinforcementsHintShown = true;
					SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_Reinforcements_Text", "#AR-Campaign_Hint_Reinforcements_Title", 20, fieldManualEntry: EFieldManualEntryId.CONFLICT_REINFORCEMENTS);
				}
				
				return;
			}
			
			case SCR_ECampaignHints.TICKETS:
			{
				if (!m_bTicketsHintShown)
				{
					m_bTicketsHintShown = true;
					SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_Supplies_Text", "#AR-Campaign_Hint_Services_Title", 30, fieldManualEntry: EFieldManualEntryId.CONFLICT_RESPAWN);
				}
				
				return;
			}
			
			case SCR_ECampaignHints.BASE_LOST:
			{
				if (!m_bBaseLostHintShown)
				{
					m_bBaseLostHintShown = true;
					SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_BaseLost_Text", "#AR-Campaign_Hint_BaseLost_Title", 15, fieldManualEntry: EFieldManualEntryId.CONFLICT_SEIZING_BASES);
				}
				
				return;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void EnteringNewBase()
	{
		SCR_CampaignFaction f = SCR_CampaignFaction.Cast(SCR_RespawnSystemComponent.GetLocalPlayerFaction());
		
		if (!f)
			return;
		
		if (m_FirstBaseWithPlayer && m_FirstBaseWithPlayer != m_LastVisitedBase)
		{
			m_LastVisitedBase = m_FirstBaseWithPlayer;
			
			// Entering enemy base
			if (m_FirstBaseWithPlayer.GetOwningFaction() != f)
			{
				s_OnEnterEnemyBase.Invoke();
				
				if ((m_iBaseSeizingHintsShown < 5 || m_FirstBaseWithPlayer.GetType() == CampaignBaseType.RELAY) && m_FirstBaseWithPlayer.IsBaseInFactionRadioSignal(f))
				{
					if (m_FirstBaseWithPlayer.GetType() == CampaignBaseType.RELAY)
						ShowHint(SCR_ECampaignHints.RECONFIGURING);
					else
						ShowHint(SCR_ECampaignHints.SEIZING);
				}
			}
			
			// Entering a friendly base in a supply truck
			if (m_iSuppliesHintsShown < 5 && m_FirstBaseWithPlayer.GetOwningFaction() == f)
			{
				ChimeraCharacter player = ChimeraCharacter.Cast(SCR_PlayerController.GetLocalControlledEntity());
				if (!player) return;
				
				if (!player.IsInVehicle()) return;
				
				CompartmentAccessComponent compartmentAccessComponent = CompartmentAccessComponent.Cast(player.FindComponent(CompartmentAccessComponent));
				if (!compartmentAccessComponent) return;
				
				BaseCompartmentSlot compartmentSlot = compartmentAccessComponent.GetCompartment();
				if (!compartmentSlot) return;
				
				Vehicle vehicle = Vehicle.Cast(compartmentSlot.GetOwner());
				if (!vehicle) return;
				
				SCR_CampaignSuppliesComponent suppliesComponent = SCR_CampaignSuppliesComponent.Cast(vehicle.FindComponent(SCR_CampaignSuppliesComponent));
				if (!suppliesComponent) return;
				
				if (m_FirstBaseWithPlayer.GetType() == CampaignBaseType.SMALL)
				{
					if (suppliesComponent.GetSupplies() != 0)
						ShowHint(SCR_ECampaignHints.SUPPLIES_UNLOADING);
				}
				else
				{
					if (m_FirstBaseWithPlayer.GetType() != CampaignBaseType.RELAY)
					{
						if (suppliesComponent.GetSupplies() != suppliesComponent.GetSuppliesMax())
						{
							m_iSuppliesHintsShown++;
							SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_SupplyRunLoad_Text", "#AR-Campaign_Hint_SupplyRun_Title", 10, fieldManualEntry: EFieldManualEntryId.CONFLICT_SUPPLIES);
						}
					}
				}
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void ProcessFactionAssignment(Faction assignedFaction)
	{
		m_LastPlayerFaction = assignedFaction;
		s_OnFactionAssignedLocalPlayer.Invoke(assignedFaction);
	}
	
	//------------------------------------------------------------------------------------------------
	protected Widget CreateMuteWidget()
	{
		return GetGame().GetWorkspace().CreateWidgets("{F8FA8AA1F524640D}UI/layouts/HUD/CampaignMP/CampaignMutePlayerLine.layout", m_wPlayersListSlot);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void RegisterTasksShown()
	{
		SCR_PopUpNotification.GetInstance().HideCurrentMsg();
		GetGame().GetInputManager().RemoveActionListener("TasksOpen", EActionTrigger.DOWN, RegisterTasksShown);
		GetGame().GetInputManager().RemoveActionListener("GadgetMap", EActionTrigger.DOWN, RegisterTasksShown);
	}
	
	//------------------------------------------------------------------------------------------------
	//Checks whether some faction is winning the game
	void EvaluateGame()
	{
		SCR_CampaignBaseManager baseManager = SCR_CampaignBaseManager.GetInstance();
		
		if (!baseManager)
			return;
		
		array<SCR_CampaignBase> controlPoints = {};
		baseManager.GetControlPoints(controlPoints);
		FactionManager factionManager = GetGame().GetFactionManager();
		array<Faction> factions = {};
		factionManager.GetFactionsList(factions);
		int controlPointsHeld;
		int controlPointsContested;
		float currentTime = Replication.Time();
		float victoryTimestamp;
		float blockPauseTimestamp;
		
		foreach (Faction faction: factions)
		{
			SCR_CampaignFaction fCast = SCR_CampaignFaction.Cast(faction);
			
			if (!fCast)
				continue;
			
			if (!fCast.IsPlayable())
				continue;
			
			controlPointsHeld = 0;
			controlPointsContested = 0;
			
			foreach (SCR_CampaignBase controlPoint: controlPoints)
			{
				if (controlPoint.GetOwningFaction() == fCast && controlPoint.IsBaseInFactionRadioSignal(fCast))
				{
					controlPointsHeld++;
					
					if (controlPoint.GetCapturingFaction() && controlPoint.GetCapturingFaction() != fCast)
						controlPointsContested++
				}
			}
			
			if (fCast.GetFactionKey() == FACTION_BLUFOR && controlPointsHeld != m_iControlPointsHeldWest)
			{
				m_iControlPointsHeldWest = controlPointsHeld;
				Replication.BumpMe();
			}
			
			if (fCast.GetFactionKey() == FACTION_OPFOR && controlPointsHeld != m_iControlPointsHeldEast)
			{
				m_iControlPointsHeldEast = controlPointsHeld;
				Replication.BumpMe();
			}
			
			victoryTimestamp = fCast.GetVictoryTimestamp();
			blockPauseTimestamp = fCast.GetPauseByBlockTimestamp();
			
			if (controlPointsHeld >= m_iControlPointsThreshold)
			{
				if ((controlPointsHeld - controlPointsContested) < m_iControlPointsThreshold)
				{
					if (blockPauseTimestamp == 0)
						fCast.SetPauseByBlockTimestamp(currentTime);
				}
				else if (blockPauseTimestamp != 0)
				{
					fCast.SetVictoryTimestamp(currentTime + victoryTimestamp - blockPauseTimestamp);
					fCast.SetPauseByBlockTimestamp(0);
				}
				
				if (victoryTimestamp == 0)
					fCast.SetVictoryTimestamp(currentTime + (m_fVictoryTimer * 1000));
			}
			else
			{
				fCast.SetVictoryTimestamp(0);
				fCast.SetPauseByBlockTimestamp(0);
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void CheckForWinner()
	{
		FactionManager factionManager = GetGame().GetFactionManager();
		array<Faction> factions = {};
		factionManager.GetFactionsList(factions);
		float lowestVictoryTimestamp = float.MAX;
		float blockPauseTimestamp;
		float actualVictoryTimestamp;
		SCR_CampaignFaction winner;
		
		foreach (Faction faction: factions)
		{
			SCR_CampaignFaction fCast = SCR_CampaignFaction.Cast(faction);
			
			if (!fCast || !fCast.IsPlayable())
				continue;
			
			blockPauseTimestamp = fCast.GetPauseByBlockTimestamp();
			
			if (blockPauseTimestamp == 0)
				actualVictoryTimestamp = fCast.GetVictoryTimestamp();
			else
				actualVictoryTimestamp = Replication.Time() + fCast.GetVictoryTimestamp() - fCast.GetPauseByBlockTimestamp();
			
			if (actualVictoryTimestamp != 0 && actualVictoryTimestamp < lowestVictoryTimestamp)
			{
				lowestVictoryTimestamp = actualVictoryTimestamp;
				winner = fCast;
			}
		}
		
		if (winner)
		{
			if (lowestVictoryTimestamp <= Replication.Time())
			{
				GetGame().GetCallqueue().Remove(CheckForWinner);
				int winnerId = factionManager.GetFactionIndex(winner);
				RpcDo_EndMatch(winnerId);
				Rpc(RpcDo_EndMatch, winnerId);
			}
			else if (factionManager.GetFactionIndex(winner) != m_iWinningFactionId || winner.GetVictoryTimestamp() != m_fVictoryTimestamp || winner.GetPauseByBlockTimestamp() != m_fVictoryPauseTimestamp)
			{
				m_iWinningFactionId = factionManager.GetFactionIndex(winner);
				m_fVictoryTimestamp = winner.GetVictoryTimestamp();
				m_fVictoryPauseTimestamp = winner.GetPauseByBlockTimestamp();
				Replication.BumpMe();
			}
		}
		else if (m_iWinningFactionId != -1 || m_fVictoryTimestamp != 0)
		{
			m_iWinningFactionId = -1;
			m_fVictoryTimestamp = 0;
			m_fVictoryPauseTimestamp = 0;
			Replication.BumpMe();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_EndMatch(int winningFactionId)
	{
		m_bMatchOver = true;
		m_fGameEnd = Replication.Time() + ENDING_TIMEOUT;
		
		// Match is over, save it so if "Continue" is selected following this the game is not loaded at an end screen
		if (!IsProxy())
		{
			SCR_SaveLoadComponent saveLoadComponent = SCR_SaveLoadComponent.GetInstance();
			saveLoadComponent.Save();
		}
		
		FactionManager fManager = GetGame().GetFactionManager();
		array<Faction> factions = {};
		fManager.GetFactionsList(factions);
		Faction winningFaction = fManager.GetFactionByIndex(winningFactionId);
		
		if (winningFaction)
		{
			foreach (Faction faction: factions)
			{
				SCR_CampaignFaction f = SCR_CampaignFaction.Cast(faction);
			
				if (f)
				{
					if (winningFaction == f)
						f.SendHQMessage(SCR_ERadioMsg.VICTORY, param: winningFactionId);
					else
						f.SendHQMessage(SCR_ERadioMsg.DEFEAT, param: winningFactionId);
				}
			}
		}
		
		// For the server end the game, replicate to all clients.
		// listening components can react to this by e.g. showing end screen
		if (IsMaster())
		{
			SCR_GameModeEndData endData = SCR_GameModeEndData.CreateSimple(SCR_GameModeEndData.ENDREASON_SCORELIMIT, winnerFactionId: winningFactionId);
			super.EndGameMode(endData);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void UpdateHUD()
	{
		Faction left;
		Faction right;
		Color yellow = Color.FromRGBA(194,100,20,210);
		Color white = Color.FromRGBA(255,255,255,255);
		
		if (!m_wRoot || !m_wMapRoot)
		{
			HUDManagerComponent hManager = GetGame().GetHUDManager();
			FactionManager fManager = GetGame().GetFactionManager();
			
			if (!hManager)
				return;
			
			if (!fManager)
				return;
			
			m_wRoot = GetGame().GetHUDManager().CreateLayout(m_sCampaignHud, EHudLayers.MEDIUM, 0);
			m_wMapRoot = GetGame().GetHUDManager().CreateLayout(m_sCampaignMapHud, EHudLayers.ALWAYS_TOP, 0);
			m_wMapRoot.SetVisible(false);
			GetGame().GetHUDManager().GetSceneBrightnessChangedInvoker().Insert(UpdateOpacity);
			
			m_wInfoOverlay = m_wRoot.FindAnyWidget("Info");
			m_wCountdownOverlay = m_wRoot.FindAnyWidget("Countdown");
			m_wLeftFlag = ImageWidget.Cast(m_wRoot.FindAnyWidget("FlagSideBlue"));
			m_wRightFlag = ImageWidget.Cast(m_wRoot.FindAnyWidget("FlagSideRed"));
			m_wLeftScore = RichTextWidget.Cast(m_wRoot.FindAnyWidget("ScoreBlue"));
			m_wRightScore = RichTextWidget.Cast(m_wRoot.FindAnyWidget("ScoreRed"));
			m_wWinScore = RichTextWidget.Cast(m_wRoot.FindAnyWidget("TargetScore"));
			m_wCountdown = RichTextWidget.Cast(m_wRoot.FindAnyWidget("CountdownWin"));
			m_wFlavour = RichTextWidget.Cast(m_wRoot.FindAnyWidget("FlavourText"));
			m_wWinScoreSideLeft = ImageWidget.Cast(m_wRoot.FindAnyWidget("ObjectiveLeft"));
			m_wWinScoreSideRight = ImageWidget.Cast(m_wRoot.FindAnyWidget("ObjectiveRight"));
			
			m_wCountdownOverlayMap = m_wMapRoot.FindAnyWidget("Countdown");
			m_wLeftFlagMap = ImageWidget.Cast(m_wMapRoot.FindAnyWidget("FlagSideBlue"));
			m_wRightFlagMap = ImageWidget.Cast(m_wMapRoot.FindAnyWidget("FlagSideRed"));
			m_wLeftScoreMap = RichTextWidget.Cast(m_wMapRoot.FindAnyWidget("ScoreBlue"));
			m_wRightScoreMap = RichTextWidget.Cast(m_wMapRoot.FindAnyWidget("ScoreRed"));
			m_wWinScoreMap = RichTextWidget.Cast(m_wMapRoot.FindAnyWidget("TargetScore"));
			m_wCountdownMap = RichTextWidget.Cast(m_wMapRoot.FindAnyWidget("CountdownWin"));
			m_wFlavourMap = RichTextWidget.Cast(m_wMapRoot.FindAnyWidget("FlavourText"));
			m_wWinScoreSideLeftMap = ImageWidget.Cast(m_wMapRoot.FindAnyWidget("ObjectiveLeft"));
			m_wWinScoreSideRightMap = ImageWidget.Cast(m_wMapRoot.FindAnyWidget("ObjectiveRight"));
			
			if (!m_wInfoOverlay || !m_wCountdownOverlay)
				return;
			
			array<Faction> outFactions = {};
			fManager.GetFactionsList(outFactions);
			
			foreach (Faction faction : outFactions)
			{
				SCR_Faction f = SCR_Faction.Cast(faction);
				if (f.IsPlayable())
					if (left == null)
					{
						left = faction;
						m_wLeftFlag.LoadImageTexture(0,f.GetFactionFlag());
						m_wLeftFlagMap.LoadImageTexture(0,f.GetFactionFlag());
						continue;
					}
					else
						if (right == null)
						{
							right = faction;
							m_wRightFlag.LoadImageTexture(0,f.GetFactionFlag());
							m_wRightFlagMap.LoadImageTexture(0,f.GetFactionFlag());
							continue;
						}
						else
							break;
			}
			m_iHideHud = Replication.Time();
		}
		
		if (IsTutorial())
		{
			m_wInfoOverlay.SetVisible(false);
			m_wCountdownOverlay.SetVisible(false);
			m_wFlavour.SetVisible(false);
		}
		else
		{
			m_wInfoOverlay.SetVisible(true);
			m_wCountdownOverlay.SetVisible(true);
		}
		
		if (m_bMatchOver)
		{
			AnimateWidget.Opacity(m_wInfoOverlay, 0, 1, true);
			m_wCountdownOverlay.SetVisible(false);
			m_wFlavour.SetVisible(false);
		}
		else
		{	
			if (m_iShowEast != m_iControlPointsHeldEast || m_iShowWest != m_iControlPointsHeldWest)
				m_iHideHud = Replication.Time();
			m_wLeftScore.SetText(m_iControlPointsHeldWest.ToString());
			m_wRightScore.SetText(m_iControlPointsHeldEast.ToString());
			m_wWinScore.SetText(m_iControlPointsThreshold.ToString());
			m_wLeftScoreMap.SetText(m_iControlPointsHeldWest.ToString());
			m_wRightScoreMap.SetText(m_iControlPointsHeldEast.ToString());
			m_wWinScoreMap.SetText(m_iControlPointsThreshold.ToString());
			m_iShowEast = m_iControlPointsHeldEast;
			m_iShowWest = m_iControlPointsHeldWest;
			
			if (m_iWinningFactionId == -1)
			{
				m_wFlavour.SetVisible(false);
				m_wCountdownOverlay.SetVisible(false);
				m_wFlavourMap.SetVisible(false);
				m_wCountdownOverlayMap.SetVisible(false);
				m_wLeftScore.SetDesiredFontSize(20);
				m_wLeftScoreMap.SetDesiredFontSize(20);
				m_wRightScoreMap.SetDesiredFontSize(20);
				m_wRightScore.SetDesiredFontSize(20);
				m_wWinScoreSideRightMap.SetColor(white);
				m_wWinScoreSideLeftMap.SetColor(white);
				m_wWinScoreSideRight.SetColor(white);
				m_wWinScoreSideLeft.SetColor(white);
				m_wRightScoreMap.SetColor(white);
				m_wRightScore.SetColor(white);
				m_wLeftScoreMap.SetColor(white);
				m_wLeftScore.SetColor(white);
				m_wWinScoreMap.SetColor(white);
				m_wWinScore.SetColor(white);
			}
			else
			{
				bool isPaused = (m_fVictoryPauseTimestamp != 0);
				float winCountdown;
				
				if (isPaused)
					winCountdown = m_fVictoryTimestamp - m_fVictoryPauseTimestamp;
				else
					winCountdown = m_fVictoryTimestamp - Replication.Time();
				
				winCountdown = Math.Max(0, Math.Ceil(winCountdown / 1000));
				//string winner = GetGame().GetFactionManager().GetFactionByIndex(m_iWinningFactionId).GetFactionKey();
				//string shownTime = SCR_Global.GetTimeFormatting(winCountdown, ETimeFormatParam.DAYS | ETimeFormatParam.HOURS, ETimeFormatParam.DAYS | ETimeFormatParam.HOURS | ETimeFormatParam.MINUTES);
				
				/*if (isPaused)
				{
					m_wCountdown.SetTextFormat("<color rgba='170,170,170,175'>%1</color>", shownTime);
					m_wCountdownMap.SetTextFormat("<color rgba='170,170,170,175'>%1</color>", shownTime);
				}
				else
				{
					m_wCountdown.SetTextFormat("%1", shownTime);
					m_wCountdownMap.SetTextFormat("%1", shownTime);
				}*/
				//m_wFlavour.SetTextFormat("#AR-ConflictHUD_FlavourText_IsWinning", GetGame().GetFactionManager().GetFactionByIndex(m_iWinningFactionId).GetFactionName());
				//m_wFlavourMap.SetTextFormat("#AR-ConflictHUD_FlavourText_IsWinning", GetGame().GetFactionManager().GetFactionByIndex(m_iWinningFactionId).GetFactionName());
				
				m_wWinScore.SetColor(yellow);
				m_wWinScoreSideRight.SetColor(yellow);
				m_wWinScoreSideLeft.SetColor(yellow);
				
				m_wWinScoreMap.SetColor(yellow);
				m_wWinScoreSideRightMap.SetColor(yellow);
				m_wWinScoreSideLeftMap.SetColor(yellow);
				/*
				if (winner.Contains(FACTION_OPFOR))
				{
					m_wRightScoreMap.SetColor(yellow);
					m_wRightScore.SetColor(yellow);
					m_wLeftScoreMap.SetColor(white);
					m_wLeftScore.SetColor(white);
					m_wRightScoreMap.SetDesiredFontSize(24);
					m_wRightScore.SetDesiredFontSize(24);
					m_wLeftScore.SetDesiredFontSize(20);
					m_wLeftScoreMap.SetDesiredFontSize(20);
				}
				else
				{
					m_wLeftScoreMap.SetColor(yellow);
					m_wLeftScore.SetColor(yellow);
					m_wRightScoreMap.SetColor(white);
					m_wRightScore.SetColor(white);
					m_wLeftScore.SetDesiredFontSize(24);
					m_wLeftScoreMap.SetDesiredFontSize(24);
					m_wRightScoreMap.SetDesiredFontSize(20);
					m_wRightScore.SetDesiredFontSize(20);
				}*/
				//m_wCountdownOverlay.SetVisible(true);
				m_wFlavour.SetVisible(true);
				m_wFlavourMap.SetVisible(true);
				//m_wCountdownOverlayMap.SetVisible(true);
				AnimateWidget.Opacity(m_wInfoOverlay, 1, 1, true);
			}
		}
		
		if (Replication.Time() <= m_iHideHud + TIME_TO_HIDE_HUD)
		{
			if(GetGame().GetInputManager().IsContextActive("MenuContext"))
				m_iHideHud = Replication.Time();
		}
		else
		{
			if(m_iWinningFactionId == -1)
				AnimateWidget.Opacity(m_wInfoOverlay, 0, 1, true);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	Widget GetOverlayWidget()
	{
		return m_wRoot;
	}
	
	//------------------------------------------------------------------------------------------------	
	protected void UpdateOpacity(float opacity, float sceneBrightness, float sceneBrightnessRaw)
    {
        m_wRoot.SetOpacity(opacity);
    }

	
	//------------------------------------------------------------------------------------------------
	void OnMapOpen()
	{
		if (SCR_SelectSpawnPointSubMenu.GetInstance() == null && !IsTutorial())
		{
			m_wMapRoot.SetVisible(true);
			Print("BASEGAMEMODEWASTELAND::OnMapOpen::MapRootSetVisible");


		}
		//else
			//m_wMapRoot.SetVisible(false);
	}
	
	//------------------------------------------------------------------------------------------------
	void OnMapClose()
	{
		if (m_wMapRoot)
			m_wMapRoot.SetVisible(false);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Process UI
	//! \param timeSlice Time since last update
	protected void ProcessUI(float timeSlice)
	{
		SCR_PopUpNotification popup = SCR_PopUpNotification.GetInstance();
		SCR_PopupMessage currentMsg = popup.GetCurrentMsg();
		
		if (!m_FirstBaseWithPlayer)
		{
			if (currentMsg && currentMsg.m_iPriority == SCR_ECampaignSeizingMessagePrio.SEIZING_YOU)
				popup.HideCurrentMsg();
			
			if (currentMsg && currentMsg.m_iPriority == SCR_ECampaignSeizingMessagePrio.SEIZING_ENEMY)
				popup.HideCurrentMsg();
		}
		else
		{
			SCR_CampaignFaction baseWithPlayerCapturingFaction = SCR_CampaignFaction.Cast(m_FirstBaseWithPlayer.GetCapturingFaction());
			
			if (!baseWithPlayerCapturingFaction)
			{
				if (currentMsg && currentMsg.m_iPriority == SCR_ECampaignSeizingMessagePrio.SEIZING_YOU)
					popup.HideCurrentMsg();
			
				if (currentMsg && currentMsg.m_iPriority == SCR_ECampaignSeizingMessagePrio.SEIZING_ENEMY)
					popup.HideCurrentMsg();
			}
			else
			{
				GenericEntity player = GenericEntity.Cast(SCR_PlayerController.GetLocalControlledEntity());
				
				if (!player)
					return;
				
				FactionAffiliationComponent comp = FactionAffiliationComponent.Cast(player.FindComponent(FactionAffiliationComponent));
				float captureStart = m_FirstBaseWithPlayer.GetCaptureStartTimestamp();
				float captureEnd = captureStart + SCR_CampaignBase.RADIO_RECONFIGURATION_DURATION * 1000;
				SCR_CampaignSeizingComponent baseSeizingComp;
				
				if (m_FirstBaseWithPlayer.GetType() != CampaignBaseType.RELAY)
				{
					baseSeizingComp = SCR_CampaignSeizingComponent.Cast(m_FirstBaseWithPlayer.FindComponent(SCR_CampaignSeizingComponent));
					
					if (baseSeizingComp)
						captureEnd = baseSeizingComp.GetSeizingEndTimestamp();
				}
				
				if (baseWithPlayerCapturingFaction == comp.GetAffiliatedFaction())
				{
					if (m_FirstBaseWithPlayer.GetReconfiguredByID() != SCR_PlayerController.GetLocalPlayerId())
					{
						if (!currentMsg || currentMsg.m_iPriority != SCR_ECampaignSeizingMessagePrio.SEIZING_YOU)
							popup.PopupMsg("#AR-Campaign_SeizingFriendly-UC", -1, prio: SCR_ECampaignSeizingMessagePrio.SEIZING_YOU, progressStart: captureStart, progressEnd: captureEnd, category: SCR_EPopupMsgFilter.TUTORIAL);
						else if (baseSeizingComp && baseSeizingComp.GetRefreshUI())
						{
							baseSeizingComp.SetRefreshUI(false);
							SCR_PopUpNotification.GetInstance().ChangeProgressBarFinish(captureEnd);
						}
					}
					
					if (currentMsg && currentMsg.m_iPriority == SCR_ECampaignSeizingMessagePrio.SEIZING_ENEMY)
						popup.HideCurrentMsg();
				}
				else
				{
					if (!currentMsg || currentMsg.m_iPriority != SCR_ECampaignSeizingMessagePrio.SEIZING_ENEMY)
					{
						popup.PopupMsg("#AR-Campaign_SeizingEnemy-UC", -1, prio: SCR_ECampaignSeizingMessagePrio.SEIZING_YOU, progressStart: captureStart, progressEnd: captureEnd, category: SCR_EPopupMsgFilter.TUTORIAL);
					}
					else if (baseSeizingComp && baseSeizingComp.GetRefreshUI())
					{
						baseSeizingComp.SetRefreshUI(false);
						SCR_PopUpNotification.GetInstance().ChangeProgressBarFinish(captureEnd);
					}
					
					if (currentMsg && currentMsg.m_iPriority == SCR_ECampaignSeizingMessagePrio.SEIZING_YOU)
						popup.HideCurrentMsg();
				}
			}
		};
		
		float curTime = GetWorld().GetWorldTime();
		
		// Hide XP info if timer runs out
		if (m_wXPInfo && m_wXPInfo.IsVisible())
		{
			if(curTime > m_fHideXPInfo)
			{
				TextWidget title = TextWidget.Cast(m_wXPInfo.FindWidget("Title"));
				title.SetTextFormat("");
			}
			if (curTime > m_fHideXPInfo && !m_bIsShowingXPBar)
				m_wXPInfo.SetVisible(false);
			else
			{
				if (curTime > (m_fHideXPInfo - (XP_INFO_DURATION * 0.7)) && curTime <= m_fHideXPInfo)
				{
					ProgressBarWidget progressDiff = ProgressBarWidget.Cast(m_wXPInfo.FindWidget("ProgressDiff"));
					ProgressBarWidget progress = ProgressBarWidget.Cast(m_wXPInfo.FindWidget("Progress"));
					Color color1 = progressDiff.GetColor();
					Color color2;
					
					if (m_bNegativeXP)
						color2 = Color.FromRGBA(159, 40, 40, 0);
					else
						color2 = progress.GetColor();
					
					color1.Lerp(color2, Math.InverseLerp(m_fHideXPInfo - (XP_INFO_DURATION * 0.7), m_fHideXPInfo, curTime));
					progressDiff.SetColor(color1);
				}	
			}	
		}
	}
	
	//***********//
	//RPC METHODS//
	//***********//
	
	//------------------------------------------------------------------------------------------------
	void MobileAssemblyFeedback(ECampaignClientNotificationID msgID, int playerID, int factionID)
	{
		Rpc(RpcDo_MobileAssemblyFeedback, msgID, playerID, factionID);
				
		if (RplSession.Mode() != RplMode.Dedicated)
			RpcDo_MobileAssemblyFeedback(msgID, playerID, factionID);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnMobileAssemblyDeployChangedWest()
	{
		if (m_DeployedMobileAssemblyIDWest != RplId.Invalid())
			m_LastDeployedHQIDWest = m_DeployedMobileAssemblyIDWest;
		
		SCR_CampaignMobileAssemblyComponent comp = SCR_CampaignMobileAssemblyComponent.Cast(Replication.FindItem(m_LastDeployedHQIDWest));
		
		if (comp)
			comp.OnDeployChanged();
		
		s_OnMobileAssemblyDeployChanged.Invoke(FACTION_BLUFOR, m_DeployedMobileAssemblyIDWest.IsValid());
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnMobileAssemblyDeployChangedEast()
	{
		if (m_DeployedMobileAssemblyIDEast != RplId.Invalid())
			m_LastDeployedHQIDEast = m_DeployedMobileAssemblyIDEast;
		
		SCR_CampaignMobileAssemblyComponent comp = SCR_CampaignMobileAssemblyComponent.Cast(Replication.FindItem(m_LastDeployedHQIDEast));
		
		if (comp)
			comp.OnDeployChanged();
		
		s_OnMobileAssemblyDeployChanged.Invoke(FACTION_OPFOR, m_DeployedMobileAssemblyIDEast.IsValid());
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_MobileAssemblyFeedback(ECampaignClientNotificationID msgID, int playerID, int factionID)
	{
		SCR_CampaignFactionManager fManager = SCR_CampaignFactionManager.GetInstance();
		
		if (!fManager)
			return;
		
		Faction localPlayerFaction = SCR_RespawnSystemComponent.GetLocalPlayerFaction();
		
		if (fManager.GetFactionIndex(localPlayerFaction) != factionID)
			return;
		
		int duration = 6;
		LocalizedString text;
		LocalizedString text2;
		string playerName = GetGame().GetPlayerManager().GetPlayerName(playerID);
		
		switch (msgID)
		{
			case ECampaignClientNotificationID.ASSEMBLY_DEPLOYED:
			{
				text = "#AR-Campaign_MobileAssemblyDeployed-UC";
				text2 = "#AR-Campaign_MobileAssemblyPlayerName";
				break;
			}
			case ECampaignClientNotificationID.ASSEMBLY_DISMANTLED:
			{
				text = "#AR-Campaign_MobileAssemblyDismantled-UC";
				text2 = "#AR-Campaign_MobileAssemblyPlayerName";
				Faction reconfigurerFaction = GetPlayerFaction(playerID);
		
				if (reconfigurerFaction && localPlayerFaction != reconfigurerFaction)
					playerName = reconfigurerFaction.GetFactionName();
				
				break;
			}
			case ECampaignClientNotificationID.ASSEMBLY_DESTROYED:
			{
				text = "#AR-Campaign_MobileAssemblyDestroyed-UC";
				break;
			}
		}
		
		if (text != string.Empty)
		{
			if (text2 != string.Empty && playerName != string.Empty)
				SCR_PopUpNotification.GetInstance().PopupMsg(text, duration, text2: text2, text2param1: playerName, prio: SCR_ECampaignPopupPriority.MHQ);
			else
				SCR_PopUpNotification.GetInstance().PopupMsg(text, duration);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns XP reward amount
	int GetXPRewardAmount(CampaignXPRewards reward)
	{
		int rewardsCnt = m_aXPRewardList.Count();
		
		for (int i = 0; i < rewardsCnt; i++)
		{			
			if (m_aXPRewardList[i].GetRewardID() == reward)
				return m_aXPRewardList[i].GetRewardXP();
		}
		
		return 0;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns XP reward amount
	float GetXPMultiplier()
	{
		return m_fXpMultiplier;
	}
	
	//------------------------------------------------------------------------------------------------
	Faction GetLastPlayerFaction()
	{
		return m_LastPlayerFaction;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns XP reward name
	string GetXPRewardName(CampaignXPRewards reward)
	{
		int rewardsCnt = m_aXPRewardList.Count();
		
		for (int i = 0; i < rewardsCnt; i++)
		{			
			if (m_aXPRewardList[i].GetRewardID() == reward)
				return m_aXPRewardList[i].GetRewardName();
		}
		
		return "";
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns XP reward skill
	EProfileSkillID GetXPRewardSkill(CampaignXPRewards reward)
	{
		int rewardsCnt = m_aXPRewardList.Count();
		
		for (int i = 0; i < rewardsCnt; i++)
		{			
			if (m_aXPRewardList[i].GetRewardID() == reward)
				return m_aXPRewardList[i].GetRewardSkill();
		}
		
		return EProfileSkillID.GLOBAL;
	}
	
	//------------------------------------------------------------------------------------------------
	protected PlayerController GetPlayerControllerFromUID(string ID)
	{
		if (ID == string.Empty)
			return null;
		
		BackendApi bApi = GetGame().GetBackendApi();
		
		if (!bApi)
			return null;
		
		array<int> pcList = {};
		int playersCnt = GetGame().GetPlayerManager().GetPlayers(pcList);
		
		for (int i = 0; i < playersCnt; i++)
			if (bApi.GetPlayerUID(pcList[i]) == ID)
				return GetGame().GetPlayerManager().GetPlayerController(pcList[i]);
		
		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void ApplyClientData(int playerId)
	{
		SCR_CampaignClientData clientData = GetClientData(playerId);
		
		if (!clientData)
			return;
		
		PlayerController pc = GetGame().GetPlayerManager().GetPlayerController(playerId);
		
		if (!pc)
			return;
		
		SCR_RespawnSystemComponent respawnSystem = SCR_RespawnSystemComponent.Cast(GetRespawnSystemComponent());
		
		if (respawnSystem)
		{
			int forcedFaction = clientData.GetFactionIndex();
			
			if (forcedFaction != -1)
				respawnSystem.DoSetPlayerFaction(playerId, forcedFaction);
		}
		
		SCR_CampaignNetworkComponent campaignNetworkComponent = SCR_CampaignNetworkComponent.Cast(pc.FindComponent(SCR_CampaignNetworkComponent));
		
		if (campaignNetworkComponent)
			campaignNetworkComponent.AddPlayerXP(CampaignXPRewards.UNDEFINED, addDirectly: clientData.GetXP() - campaignNetworkComponent.GetPlayerXP());
	}
	
	//------------------------------------------------------------------------------------------------
	void LoadClientData(notnull array<ref SCR_CampaignPlayerStruct> data)
	{
		m_aRegisteredClients.Clear();
		
		foreach (SCR_CampaignPlayerStruct playerData : data)
		{
			SCR_CampaignClientData clientData = new SCR_CampaignClientData();
			
			clientData.SetID(playerData.GetID());
			clientData.SetXP(playerData.GetXP());
			clientData.SetFactionIndex(playerData.GetFactionIndex());
			
			m_aRegisteredClients.Insert(clientData);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void WriteAllClientsData()
	{
		array<int> pcList = {};
		int playersCnt = GetGame().GetPlayerManager().GetPlayers(pcList);
		
		for (int i = 0; i < playersCnt; i++)
		{
			PlayerController pc = GetGame().GetPlayerManager().GetPlayerController(pcList[i]);
			
			if (!pc)
				continue;
			
			int ID = pc.GetPlayerId();
			WriteClientData(ID, pc: pc);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	// !Save object with player's current data
 	protected void WriteClientData(int playerID, bool disconnecting = false, PlayerController pc = null)
	{
		SCR_CampaignClientData clientData = GetClientData(playerID);

		if (!clientData)
			return;

		if (!pc)
			pc = GetGame().GetPlayerManager().GetPlayerController(playerID);
		
		if (!pc)
			return;
		
		SCR_CampaignNetworkComponent campaignNetworkComponent = SCR_CampaignNetworkComponent.Cast(pc.FindComponent(SCR_CampaignNetworkComponent));
		
		if (!campaignNetworkComponent)
			return;
		
		// Set data readable from PlayerController
		clientData.SetXP(campaignNetworkComponent.GetPlayerXP());
		IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerID);
		
		// Set data from player's entity
		if (player)
			clientData.SetStartingPosition(player.GetOrigin());
	}
	
	//------------------------------------------------------------------------------------------------
	void SpawnMobileHQ(notnull SCR_CampaignFaction faction, vector pos, vector rot)
	{
		if (faction.GetDeployedMobileAssembly() != null)
			return;
		
		EntitySpawnParams params = EntitySpawnParams();
		GetWorldTransform(params.Transform);
		params.TransformMode = ETransformMode.WORLD;
		Math3D.AnglesToMatrix(rot, params.Transform);
		params.Transform[3] = pos;
		
		IEntity MHQ = GetGame().SpawnEntityPrefab(Resource.Load(faction.GetMobileHQPrefab()), null, params);
		
		if (!MHQ)
			return;
		
		BaseRadioComponent radioComponent = BaseRadioComponent.Cast(MHQ.FindComponent(BaseRadioComponent));
		
		if (radioComponent)
		{
			radioComponent.TogglePower(false);
			radioComponent.SetFrequency(faction.GetFactionRadioFrequency());
			radioComponent.SetEncryptionKey(faction.GetFactionRadioEncryptionKey());
		}
		
		SlotManagerComponent slotManager = SlotManagerComponent.Cast(MHQ.FindComponent(SlotManagerComponent));
		
		if (!slotManager)
			return;
		
		array<EntitySlotInfo> slots = {};
		slotManager.GetSlotInfos(slots);
		
		foreach (EntitySlotInfo slot: slots)
		{
			if (!slot)
				continue;
			
			IEntity truckBed = slot.GetAttachedEntity();
			
			if (!truckBed)
				continue;
			
			SCR_CampaignMobileAssemblyComponent mobileAssemblyComponent = SCR_CampaignMobileAssemblyComponent.Cast(truckBed.FindComponent(SCR_CampaignMobileAssemblyComponent));
			
			if (mobileAssemblyComponent)
			{
				mobileAssemblyComponent.SetParentFactionID(GetGame().GetFactionManager().GetFactionIndex(faction));
				mobileAssemblyComponent.UpdateRadioCoverage();
				mobileAssemblyComponent.Deploy(true);
				break;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Manager for spawning enemies inside bases
	//! \param timeSlice timeSlice from OnFrame event
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	protected void HandleRemnantForces(float timeSlice)
	{
		//Print("BaseGameWasteModeWasteland::HandleRemnantsForces::FunctionFired");

		if (m_bAllRemnantsSpawned)
		{
			//Print("BaseGameWasteModeWasteland::HandleRemnantsForces::m_bAllRemnantsSpawned = " + m_bAllRemnantsSpawned);
			//Print("BaseGameWasteModeWasteland::HandleRemnantsForces::returned in HandleRemantForces() on line 3235!");

			return;

		}
		
		m_fRemnantForcesTimer += timeSlice;
		int checkID = m_iLocationCheckedForPlayerProximity;
		
		if (m_iRemnantLocationsCnt == 0 || m_fRemnantForcesTimer < (2 / m_iRemnantLocationsCnt))
		{
			//Print("BaseGameWasteModeWasteland::HandleRemnantsForces::m_iRemnantLocationsCnt =" +m_iRemnantLocationsCnt);
			//Print("BaseGameWasteModeWasteland::HandleRemnantsForces::returned in HandleRemantForces() on line 3247");
			return;
		
		}
			
		
		m_fRemnantForcesTimer = 0;
		SCR_CampaignRemnantsPresence location = null;
		array<SCR_CampaignRemnantsPresence> locationMapChange = new array<SCR_CampaignRemnantsPresence>();
		while (!location || (location.GetMembersAlive() == 0 && !location.GetIsSpawned()) || location.GetRespawnTimestamp() > Replication.Time())
		{
			m_iLocationCheckedForPlayerProximity++;
			
			
			
			if (m_iLocationCheckedForPlayerProximity == m_iRemnantLocationsCnt)
			{
				m_iLocationCheckedForPlayerProximity = -1;
				
				if (checkID == -1)
					m_bAllRemnantsSpawned = true;
				
				
				//Print("BaseGameWasteModeWasteland::HandleRemnantsForces::returned in HandleRemantForces() on line 3247");

				return;
			}
			
			location = m_aRemnantsPresence[m_iLocationCheckedForPlayerProximity];
			
			
			
			
		};
		
		array<int> players = {};
		int playersCount = GetGame().GetPlayerManager().GetPlayers(players);
		bool playersNear = false;
		bool playersFar = true;
		
		// Calculate (de)spawn distance based on view distance, have it squared for faster distance calculation
		float spawnDistanceSq = Math.Pow(GetGame().GetViewDistance() / 2, 2);
		spawnDistanceSq = Math.Min(REMNANTS_SPAWN_RADIUS_MAX, spawnDistanceSq);
		spawnDistanceSq = Math.Max(REMNANTS_SPAWN_RADIUS_MIN, spawnDistanceSq);
		float despawnDistanceSq = spawnDistanceSq + REMNANTS_DESPAWN_RADIUS_DIFF;
		
		for (int i = 0; i < playersCount; i++)
		{
			IEntity playerEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(players[i]);
			
			if (!playerEntity)
				continue;

			float dist = vector.DistanceSq(playerEntity.GetOrigin(), location.GetCenter());

			if (dist < despawnDistanceSq)
			{
				playersFar = false;

				if (dist < spawnDistanceSq)
				{
					playersNear = true;
					break;
				}
			}
		}
		//Print("BaseGameWasteModeWasteland::HandleRemnantsForces::b_playersNear "+ playersNear);
		//Print("BaseGameWasteModeWasteland::HandleRemnantsForces::b_playersFar "+ playersFar);

		if (!location.GetIsSpawned() && playersNear)
		{
			//Print("BaseGameWasteModeWasteland::HandleRemnantsForces()::SpawnRemnants()");
			
			//need to pass MapDescriptor ref to RemnantsPresence
			SpawnRemnants(location, players);
			return;
		}
		//Print("BaseGameWasteModeWasteland::HandleRemnantsForces::functionEndWithNoRemnantsSpawn");

		// Delay is used so dying players don't see the despawn happen
		if (m_bAllowRemnantsDespawn && location.GetIsSpawned() && playersFar)
		{
			float despawnT = location.GetDespawnTimer();
			
			if (despawnT == -1)
				location.SetDespawnTimer(Replication.Time() + 10000);
			else
				if (Replication.Time() > despawnT)
				{
					DespawnRemnants(location);
				}
		}
		else
			location.SetDespawnTimer(-1);
	}
	
	
	
	
	SCRWST_AISpawnManager m_AISpawnManager;
	void registerAISpawnManager(SCRWST_AISpawnManager manager)
	{
		m_AISpawnManager = manager;
	}
	//------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//heavily modified for respawn ai mechanic and handling 
	protected void SpawnRemnants(notnull SCR_CampaignRemnantsPresence location, notnull array<int> playerIds)
	{
		Resource res = Resource.Load(location.GetGroupPrefab());
		
		if (!res)
			return;
		
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		
		// For normal non-respawning remnants, use the default spawnpoint
		// Otherwise, find a suitable location nearby (without player presence)
		if (location.GetRespawnTimestamp() == 0)
		{
			params.Transform[3] = location.GetSpawnpoint();
		}
		else
		{
			array<vector> suitableRespawns = {};
			array<vector> allRespawns = location.GetRespawns();
			int minDistance = Math.Pow(SCR_CampaignRemnantsPresence.RESPAWN_PLAYER_DISTANCE_THRESHOLD, 2);
			IEntity playerEntity;
			bool isSuitable;
			int playersCount = playerIds.Count();
			
			foreach (vector respawn : allRespawns)
			{
				isSuitable = true;
				//skip this 
				/*
				for (int i = 0; i < playersCount; i++)
				{
					playerEntity = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerIds[i]);
					
					if (!playerEntity)
						continue;
					
					if (vector.DistanceSqXZ(playerEntity.GetOrigin(), respawn) < minDistance)
					{
						isSuitable = false;
						break;
					}
				}*/
				
				if (isSuitable)
					suitableRespawns.Insert(respawn);
			}
			
			if (suitableRespawns.IsEmpty())
			{
				//fail fallback, array should be filled and positions randomized
				Print("FAILFALLBACK::BASEGAMEMODEWASTELAND::AI_RESPAWN");
				vector locationVector = location.GetSpawnpoint();  
				suitableRespawns.Insert(locationVector);


			}
				
			
			Math.Randomize(-1);
			params.Transform[3] = suitableRespawns.GetRandomElement();
		}
		
		SCR_AIGroup grp = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(res, null, params));
		
		
		
		if (!grp)
			return;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//MapDescriptor Update on AI group respawn
		SCR_CampaignRemnantsSpawnPoint sp = location.GetSpawnPointSP();
		string identi = sp.getIdent();
		RpcDo_ShowDescriptor(identi);
		Rpc(RpcDo_ShowDescriptor,identi);
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		location.SetDespawnTimer(-1);
		location.SetIsSpawned(true);
		
		if (location.GetMembersAlive() > 0)
			grp.SetMaxUnitsToSpawn(location.GetMembersAlive());
		
		grp.SpawnUnits();
		AIWaypoint wp = location.GetWaypoint();
		
		if (wp)
			grp.AddWaypoint(location.GetWaypoint());
		
		location.SetSpawnedGroup(grp);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void DespawnRemnants(notnull SCR_CampaignRemnantsPresence location)
	{
		location.SetDespawnTimer(-1);
		location.SetIsSpawned(false);
		
		SCR_AIGroup grp = location.GetSpawnedGroup();
		
		if (!grp)
		{
			location.SetMembersAlive(0);
			return;
		}
		
		array<AIAgent> units = new array<AIAgent>();
		grp.GetAgents(units);
		int cnt = grp.GetAgentsCount();
		location.SetMembersAlive(cnt);
		
		for (int i = 0; i < cnt; i++)
		{
			if (units[i])
			{
				grp.RemoveAIEntityFromGroup(units[i].GetControlledEntity());
				delete units[i].GetControlledEntity();
				delete units[i];
			}
		}

		delete grp;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void OnEnemyDetectedByDefenders(SCR_AIGroup grp, Faction detectedFaction)
	{
		if (!detectedFaction)
			return;
		
		array<SCR_CampaignBase> allBases = SCR_CampaignBaseManager.GetBases();
		
		foreach (SCR_CampaignBase base : allBases)
		{
			if (!base)
				continue;
			
			if (!base.GetIsEnabled())
				continue;
			
			if (base.GetDefendersGroup() == grp)
			{
				base.NotifyAboutEnemyAttack(detectedFaction);
				return;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	array<ref SCR_CampaignClientData> GetClientsData()
	{
		return m_aRegisteredClients;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Get corresponding client data, create new object if not found
	protected SCR_CampaignClientData GetClientData(int playerId)
	{
		if (playerId == 0)
			return null;
		
		string playerIdentity = SCR_CampaignPlayerStruct.GetPlayerIdentity(playerId);
		
		if (playerIdentity == string.Empty)
			return null;
		
		SCR_CampaignClientData clientData;
		int clientsCnt = m_aRegisteredClients.Count();
		
		// Check if the client is reconnecting
		for (int i = 0; i < clientsCnt; i++)
		{
			if (m_aRegisteredClients[i].GetID() == playerIdentity)
			{
				clientData = m_aRegisteredClients[i];
				break;
			}
		}
		
		if (!clientData)
		{
			clientData = new SCR_CampaignClientData;
			clientData.SetID(playerIdentity);
			m_aRegisteredClients.Insert(clientData);
		}

		return clientData;
	}
	
	//------------------------------------------------------------------------------------------------
	// Refresh building preview when composition was disassambled.
	void RefreshBuildingPreview(EntityID slotID)
	{
		Rpc(Rpc_RefreshBuildingPreview, slotID);
		Rpc_RefreshBuildingPreview(slotID);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Refresh building preview
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void Rpc_RefreshBuildingPreview(EntityID slotID)
	{
		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		if (!player) 
			return;
				
		SCR_CampaignBuildingComponent buildComp = SCR_CampaignBuildingComponent.Cast(player.FindComponent(SCR_CampaignBuildingComponent));		
		if (!buildComp || !buildComp.IsBuilding())
			return;
		
		SCR_SiteSlotEntity slotEnt = SCR_SiteSlotEntity.Cast(GetGame().GetWorld().FindEntityByID(slotID));
		if (!slotEnt)
			return;
		
		//-- Get the controller
		GetGame().GetWorld().QueryEntitiesBySphere(slotEnt.GetOrigin(), BUILDING_CONTROLLER_SEARCH_DISTANCE, FindController, null, EQueryEntitiesFlags.ALL);
		if (!m_BuildingController)
			return;
		
		SCR_CampaignBuildingControllerComponent buildContComp = SCR_CampaignBuildingControllerComponent.Cast(m_BuildingController.FindComponent(SCR_CampaignBuildingControllerComponent));
		if (!buildContComp)
			return;

		IEntity suppliesProvider =  buildContComp.GetSuppliesProvider();
		if (!suppliesProvider)
			return;
		
		SCR_CampaignFaction faction;
		ref array<ref SCR_CampaignSlotComposition> SlotData = new array<ref SCR_CampaignSlotComposition>();
		
		if (suppliesProvider.Type() == SCR_CampaignBase)
		{
			SCR_CampaignBase base = SCR_CampaignBase.Cast(suppliesProvider);
			faction = SCR_CampaignFaction.Cast(base.GetOwningFaction());
		}
		else
		{
			FactionAffiliationComponent factionAffiliationComponent = FactionAffiliationComponent.Cast(suppliesProvider.FindComponent(FactionAffiliationComponent));
			if (!factionAffiliationComponent)
			    return;
				
			faction = SCR_CampaignFaction.Cast(factionAffiliationComponent.GetDefaultAffiliatedFaction());
		}
		
		ResourceName slotResName = slotEnt.GetPrefabData().GetPrefabName();
		SlotData = faction.GetSlotResource(buildComp.GetCompositionType(slotResName));
		
		Color iconColor;
		if (faction)
			iconColor = faction.GetFactionColor();
	
		buildComp.SpawnNewPreview(SlotData[0], slotEnt, suppliesProvider, buildContComp, iconColor);
	}
	
	//------------------------------------------------------------------------------------------------
	// Search for a controller dedicated to the slot
	protected bool FindController(IEntity ent)
	{		
		SCR_CampaignBuildingControllerComponent contComp = SCR_CampaignBuildingControllerComponent.Cast(ent.FindComponent(SCR_CampaignBuildingControllerComponent));
		if (!contComp)
		    return true;
						
		m_BuildingController = ent;
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	void AfterAllBasesInitialized()
	{
		SCR_CampaignBaseManager.s_OnAllBasesInitialized.Remove(AfterAllBasesInitialized);
		
		SCR_CampaignFactionManager campaignFactionManager = SCR_CampaignFactionManager.GetInstance();
		SCR_CampaignFaction west = SCR_CampaignFaction.Cast(campaignFactionManager.GetFactionByKey(FACTION_BLUFOR));
		SCR_CampaignFaction east = SCR_CampaignFaction.Cast(campaignFactionManager.GetFactionByKey(FACTION_OPFOR));
		
		if (!west || !east)
			return;
		
		SCR_CampaignBase HQWest = west.GetMainBase();
		SCR_CampaignBase HQEast = east.GetMainBase();
		
		int baseIndex = 0;
		
		array<int> allIndexes = {};
		array<int> suppliesBufferWest = new array<int>();
		array<int> suppliesBufferEast = new array<int>();
		int intervalMultiplier = Math.Floor((STARTING_SUPPLIES_MAX - STARTING_SUPPLIES_MIN) / STARTING_SUPPLIES_INTERVAL);
		array<SCR_CampaignBase> bases = SCR_CampaignBaseManager.GetBases();
		array<SCR_CampaignBase> basesSorted = {};
		
		// Sort indexes by distance from BLUFOR HQ
		if (HQWest)
		{
			foreach (SCR_CampaignBase base: bases)
			{
				if (!base)
					continue;
				
				if (!base.GetIsEnabled())
					continue;
				
				bool done = false;
				float distanceToHQ = vector.DistanceSqXZ(HQWest.GetOrigin(), base.GetOrigin());
				
				for (int i = 0, cnt = basesSorted.Count(); i < cnt; i++)
				{
					SCR_CampaignBase baseCheckedAgainst = basesSorted[i];
					
					if (distanceToHQ < vector.DistanceSqXZ(HQWest.GetOrigin(), baseCheckedAgainst.GetOrigin()))
					{
						basesSorted.InsertAt(base, i);
						done = true;
						break;
					}
				}
				
				if (!done)
					basesSorted.Insert(base);
			}
		}
		else
			basesSorted.Copy(bases);
		
		if (!IsProxy())
		{
			for(int i = 0; i <= BASE_CALLSIGNS_COUNT-1; i++)
				allIndexes.Insert(i);
			
			// Get rid of Remnant patrols too close to HQs
			SCR_CampaignFaction blufor = SCR_CampaignFaction.Cast(GetGame().GetFactionManager().GetFactionByKey(FACTION_BLUFOR));
			SCR_CampaignFaction opfor = SCR_CampaignFaction.Cast(GetGame().GetFactionManager().GetFactionByKey(FACTION_OPFOR));
			
			if (blufor)
				GetGame().GetCallqueue().CallLater(ClearRemnantPatrolsAroundHQ, 1000, false, blufor);
			
			if (opfor)
				GetGame().GetCallqueue().CallLater(ClearRemnantPatrolsAroundHQ, 1000, false, opfor);
		}
		
		foreach (SCR_CampaignBase base: basesSorted)
		{
			if (!base)
				continue;
			
			if (!base.GetIsEnabled())
				continue;
			
			Math.Randomize(-1);
			baseIndex = allIndexes.GetRandomIndex();
			
			if (!IsProxy() && base.GetType() != CampaignBaseType.RELAY && baseIndex < BASE_CALLSIGNS_COUNT)
			{
				base.SetCallsignIndex(allIndexes[baseIndex]);
				allIndexes.Remove(baseIndex);
			}

			if (!IsProxy() && HQWest && HQEast)
			{
				base.SpawnAIWaypoints();
				
				if (!base.GetIsHQ())
				{
					SCR_CampaignFaction setFaction = base.GetOwningFaction();
					
					if (!setFaction)
					{
						vector pos = base.GetOrigin();
						float distHQWest = vector.DistanceSq(pos, HQWest.GetOrigin());
						float distHQEast = vector.DistanceSq(pos, HQEast.GetOrigin());
						setFaction = HQWest.GetOwningFaction();
						
						if (distHQWest > distHQEast)
							setFaction = HQEast.GetOwningFaction();
						
						// Add random starting supplies to small bases
						if (m_bRandomizeSupplies && setFaction)
						{
							int amount;
							FactionKey fKey = setFaction.GetFactionKey();
							
							// Check if we have preset supplies stored in buffer
							if (fKey == FACTION_BLUFOR && !suppliesBufferWest.IsEmpty())
							{
								amount = suppliesBufferWest[0];
								suppliesBufferWest.RemoveOrdered(0);
							}
							else if (fKey == FACTION_OPFOR && !suppliesBufferEast.IsEmpty())
							{
								amount = suppliesBufferEast[0];
								suppliesBufferEast.RemoveOrdered(0);
							}
							else
							{
								// Supplies from buffer not applied, add random amount, store to opposite faction's buffer
								amount = STARTING_SUPPLIES_MIN + (STARTING_SUPPLIES_INTERVAL * Math.RandomIntInclusive(0, intervalMultiplier));
								
								if (fKey == FACTION_BLUFOR)
									suppliesBufferEast.Insert(amount);
								else
									suppliesBufferWest.Insert(amount);
							}
							
							base.AddSupplies(amount - base.GetSupplies());
						}
					}
					
					base.SetBuildingsFaction(setFaction);
				}
				
				// Spawn pre-built structures only on server
				// Delay so we don't spawn stuff during init, and allow loaded state an override
				if (base.GetBuildingsFaction())
					GetGame().GetCallqueue().CallLater(base.SpawnBuildings, 2000, false, null);
			}
			
			// Prepare a list of dedicated Slots
			SCR_SiteSlotEntity slot;
			
			for (int i = ECampaignCompositionType.LAST; i >= 0; i--)
			{
				slot = base.GetAssignedSlot(i); 
				if (slot) 
				{
					SCR_BuildingDedicatedSlotData slotData = new SCR_BuildingDedicatedSlotData();	
					slotData.SetData(i, base);
					m_aDedicatedSlots.Insert(slot, slotData);
				}
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void ClearRemnantPatrolsAroundHQ(notnull SCR_CampaignFaction faction)
	{
		SCR_CampaignBase hq = faction.GetMainBase();
		
		if (!hq)
			return;
		
		int distanceLimit = Math.Pow(HQ_NO_REMNANTS_RADIUS, 2);
		int patrolDistanceLimit = Math.Pow(HQ_NO_REMNANTS_PATROL_RADIUS, 2);
		float distance;
		
		for (int i = m_aRemnantsPresence.Count() - 1; i >= 0; i--)
		{
			distance = vector.DistanceSqXZ(m_aRemnantsPresence[i].GetCenter(), hq.GetOrigin());

			if (distance < distanceLimit)
			{
				m_aRemnantsPresence[i].SetMembersAlive(0);
				continue;
			}
			
			if (distance < patrolDistanceLimit)
			{
				AIWaypointCycle waypoint = AIWaypointCycle.Cast(m_aRemnantsPresence[i].GetWaypoint());
				if (waypoint)
					m_aRemnantsPresence[i].SetMembersAlive(0);
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void OverrideBaseRadioRanges()
	{
		array<SCR_CampaignBase> bases = SCR_CampaignBaseManager.GetBases();
		
		foreach (SCR_CampaignBase base: bases)
		{
			if (!base.GetIsEnabled())
				continue;
			
			BaseRadioComponent comp = BaseRadioComponent.Cast(base.FindComponent(BaseRadioComponent));
			
			if (!comp)
				continue;
			
			comp.SetRange(m_iCustomRadioRange);
			base.LinkBases();
			base.HandleMapInfo();
		}
		
		SCR_CampaignBaseManager bm = SCR_CampaignBaseManager.GetInstance();
		
		if (bm)
			bm.UpdateBasesSignalCoverage()
	}
	
	//****************//
	//OVERRIDE METHODS//
	//****************//
	
	//------------------------------------------------------------------------------------------------
	//! Returns true if player with provided playerID is able to respawn, false otherwise
	override bool CanPlayerRespawn(int playerID)
	{
		return super.CanPlayerRespawn(playerID);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool RplSave(ScriptBitWriter writer)
	{
		if (!super.RplSave(writer))
			return false;
		
		writer.WriteInt(m_iCustomRadioRange);
		writer.WriteInt(m_iMaxRespawnRadios);
		writer.WriteBool(m_bIgnoreMinimumVehicleRank);
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool RplLoad(ScriptBitReader reader)
	{
		if (!super.RplLoad(reader))
			return false;
		
		reader.ReadInt(m_iCustomRadioRange);
		reader.ReadInt(m_iMaxRespawnRadios);
		reader.ReadBool(m_bIgnoreMinimumVehicleRank);
		
		if (m_iCustomRadioRange != -1)
			GetGame().GetCallqueue().CallLater(OverrideBaseRadioRanges, 250);
		
		SCR_PlayerSpawnPointManagerComponent comp = SCR_PlayerSpawnPointManagerComponent.Cast(FindComponent(SCR_PlayerSpawnPointManagerComponent));
		
		if (comp)
			comp.EnablePlayerSpawnPoints(m_iMaxRespawnRadios >= 0);
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnWorldPostProcess(World world)
	{	
		super.OnWorldPostProcess(world);

		SCR_CampaignBaseManager baseManager = null;//SCR_CampaignBaseManager.GetInstance();

		if(!baseManager)
			return;

		baseManager.UpdateBasesSignalCoverage();	
			
#ifdef CONFLICT_SPAWN_ALL_AI
		foreach (SCR_CampaignRemnantsPresence presence : m_aRemnantsPresence)
		{
			if (presence)
				SpawnRemnants(presence);
		}
#endif
	}
	
	//------------------------------------------------------------------------------------------------
	//Called once tasks are initialized
	override void HandleOnTasksInitialized()
	{
		super.HandleOnTasksInitialized();
	}

	//------------------------------------------------------------------------------------------------
	//! What happens when a player connects.
	//! Method is called from SCR_DeathmatchLobbyEntity
	//! \param playerID is a unique player identifier that defines which player has disconnected.
	override void OnPlayerConnected(int playerId)
	{
#ifdef ENABLE_DIAG
		if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_EXECUTE_GAMEMODE))
			return;
#endif
		super.OnPlayerConnected(playerId);
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPlayerRegistered(int playerId)
	{
		super.OnPlayerRegistered(playerId);
		
		if (RplSession.Mode() == RplMode.None)
			ApplyClientData(playerId);
		
		// See HandleOnFactionAssigned()
		if (SCR_PlayerController.GetLocalPlayerId() != 0 && !GetLastPlayerFaction())
		{
			for (int i = 0, cnt = m_aUnprocessedFactionAssignments.Count(); i < cnt; i++)
			{
				if (m_aUnprocessedFactionAssignments.GetKey(i) == SCR_PlayerController.GetLocalPlayerId())
					ProcessFactionAssignment(m_aUnprocessedFactionAssignments.GetElement(i));
			}			
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPlayerAuditSuccess(int iPlayerID)
	{
		super.OnPlayerAuditSuccess(iPlayerID);
		
		if (RplSession.Mode() != RplMode.None)
			ApplyClientData(iPlayerID);
		
		/*
		SCR_PlayerData replaces this, thus this is outdated and we need to remove it
		SCR_PlayerProfileManagerComponent comp = SCR_PlayerProfileManagerComponent.Cast(FindComponent(SCR_PlayerProfileManagerComponent));
		
		if (comp)
			comp.LoadConnectingPlayerProfile(iPlayerID);
		*/
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnPlayerAuditFail(int iPlayerID)
	{
		super.OnPlayerAuditFail(iPlayerID);
	}
	
	//------------------------------------------------------------------------------------------------
	//! What happens when a player disconnects.
	override void OnPlayerDisconnected(int playerId, KickCauseCode cause, int timeout)
	{
#ifdef ENABLE_DIAG
		if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_EXECUTE_GAMEMODE))
			return;
#endif
		super.OnPlayerDisconnected(playerId, cause, timeout);
		
		GetTaskManager().OnPlayerDisconnected(playerId);
		WriteClientData(playerId, true);
		
		// Disconnecting player is currently capturing a base; handle it
		foreach (SCR_CampaignBase base: SCR_CampaignBaseManager.GetBases())
		{
			if (!base)
				continue;
			
			if (!base.GetIsEnabled())
				continue;
			
			if (base.GetCapturingFaction() && base.GetReconfiguredByID() == playerId)
			{
				base.EndCapture();
				break;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Serves for enabling spawn hint on map. Also saves player spawn position
	void EnablePlayerSpawnHint(bool enable)
	{
		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		
		if (!player && enable)
			return;
		
		m_bCanShowSpawnPosition = enable;
		
		if (enable)
		{
			m_vFirstSpawnPosition = player.GetOrigin();
			SetSpawnTime();
		}
		else
		{
			m_vFirstSpawnPosition = vector.Zero;
			SetMapOpened(false);
			
			if (m_MapCampaignUI)
				m_MapCampaignUI.RemoveSpawnPositionHint();
		}

		GetGame().GetCallqueue().Remove(EnablePlayerSpawnHint);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns players position after spawning
	vector GetPlayerSpawnPos()
	{
		return m_vFirstSpawnPosition;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns true if map was already opened 
	bool WasMapOpened()
	{
		return m_bWasMapOpened;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Sets that map was already opened by player
	void SetMapOpened(bool wasOpened)
	{
		m_bWasMapOpened = wasOpened;
	}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////------------------------------------------------------------------------------------------------
//just enough to delete inventory (crashes and ud when deleting items in quickbar/weapon slot)
	void DeleteAllItems(SCR_InventoryStorageManagerComponent mmc)
	{
		array<IEntity> items = new array<IEntity>();
		

		array<BaseInventoryStorageComponent> availableStorages = new array<BaseInventoryStorageComponent>();
		int storageCount = mmc.GetStorages(availableStorages);
		
		
		int		itemsCount = mmc.GetAllItems(items,availableStorages[0]);
		Print("GameModeWasteland::Deleting item count " + items.Count());
		
		/*
		array<IEntity> aGadgets = new array<IEntity>();
		SCR_HoldableItemPredicate p_holdable = new SCR_HoldableItemPredicate();
		array<IEntity> magazines = new array<IEntity>();
		SCR_MagazinePredicate pMagazine = new SCR_MagazinePredicate();
		array<IEntity> medical = new array<IEntity>();
		SCR_ApplicableMedicalItemPredicate pMedical = new SCR_ApplicableMedicalItemPredicate();
		mmc.FindItems(aGadgets,p_holdable);
		mmc.FindItems(magazines,pMagazine);
		mmc.FindItems(medical,pMedical);
		
		Print("GameModeWasteland::item aGadgets start");
		foreach(IEntity it : aGadgets)
		{
			Print("GameModeWasteland::item aGadgets" +it.GetPrefabData().GetPrefabName());
			Print("GameModeWasteland::Deleting:  "+it.GetPrefabData().GetPrefabName());
			bool success = mmc.TryDeleteItem(it);
			Print("GameModeWasteland::Deleting success  "+ success);
		}		
		Print("GameModeWasteland::item magazines start");
		foreach(IEntity it : magazines)
		{
			Print("GameModeWasteland::magazines data" +it.GetPrefabData().GetPrefabName());
			Print("GameModeWasteland::Deleting:  "+it.GetPrefabData().GetPrefabName());
			bool success = mmc.TryDeleteItem(it);
			Print("GameModeWasteland::Deleting success  "+ success);
		}		
		Print("GameModeWasteland::item medical start");
		foreach(IEntity it : medical)
		{
			Print("GameModeWasteland::medical data" +it.GetPrefabData().GetPrefabName());
			Print("GameModeWasteland::Deleting:  "+it.GetPrefabData().GetPrefabName());
			bool success = mmc.TryDeleteItem(it);
			Print("GameModeWasteland::Deleting success  "+ success);
		}
		*/
		bool success = false;	
		for (int i = items.Count()-1;i>=0;--i)
		{
			IEntity item = items.Get(i);
			if(!item)
				continue;
			Print("GameModeWasteland::Deleting:  "+item.GetPrefabData().GetPrefabName());
			RplComponent rpl = RplComponent.Cast(item.FindComponent(RplComponent));
			
				success = mmc.TryDeleteItem(item);
				RplComponent.DeleteRplEntity(item,false);
			
			
				
			Print("GameModeWasteland::Deleting success  "+ success);

		}
		
	
	}
	
	void AddAllItems(SCR_InventoryStorageManagerComponent mmc,WST_GearObject o)
	{
		foreach(EntityPrefabData data : o.itemsOnPlayer)
			{
						
				Print("GameModeWasteland::Addding:  "+ data.GetPrefabName());

				ResourceName n = data.GetPrefabName();
				IEntity spawnedItem = GetGame().SpawnEntityPrefab(Resource.Load(n),GetGame().GetWorld());
				if(!spawnedItem)
					continue;
				MoneyComponent mc = MoneyComponent.Cast(spawnedItem.FindComponent(MoneyComponent));
				if(mc)
					continue;
				RplComponent rpl = RplComponent.Cast(spawnedItem.FindComponent(RplComponent));
				if (rpl)
					RplComponent.DeleteRplEntity(spawnedItem,false);
				
						
				//bool success = mmc.TryInsertItem(spawnedItem,EStoragePurpose.PURPOSE_ANY,null);
				bool success = mmc.TrySpawnPrefabToStorage(n);

				Print("GameModeWasteland::Succes:  "+success +"for Item "+data.GetPrefabName());
			}
		//create and insert money comp here
		if (o.walletValue > 0)
		{
			ResourceName walletPrefab = "{E63CDF1BD7C8CFF6}Prefabs/Items/Equipment/Compass/Money.et";
			bool success = mmc.TrySpawnPrefabToStorage(walletPrefab);
			
			if (success)
			{
				//mmc.m_OnItemAddedInvoker.Invoke();
				Print("GameModeWasteland::Succes:  "+success +"for Wallet");
	
			}
			
			WST_WalletPredicate walletPredicate = new WST_WalletPredicate();
			IEntity wallet = mmc.FindItem(walletPredicate);
			if (wallet)
			{
				IEntity owner = mmc.GetOwner();
				WST_MoneyConfigComponent config = WST_MoneyConfigComponent.Cast(owner.FindComponent(WST_MoneyConfigComponent));
				if (config)
				{
					Print("GameModeWasteland::Config found!");
					//client will set value, this is server side!
					config.alreadySet = true;

				}
				MoneyComponent mc = MoneyComponent.Cast(wallet.FindComponent(MoneyComponent));
				int savedValue = o.walletValue;
				Print("GameModeWasteland::setting wallet value  " + savedValue );
				
				mc.SetValue(savedValue);
			}
			
			
		}
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	override void OnPlayerSpawned(int playerId, IEntity controlledEntity)
	{
#ifdef ENABLE_DIAG
		if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_EXECUTE_GAMEMODE))
			return;
#endif
		super.OnPlayerSpawned(playerId, controlledEntity);
		
		PlayerController pc = GetGame().GetPlayerManager().GetPlayerController(playerId);
		ref SCR_InventoryStorageManagerComponent mmc = SCR_InventoryStorageManagerComponent.Cast(controlledEntity.FindComponent(SCR_InventoryStorageManagerComponent));
		mmc.OnItemAddedHandler();
		mmc.OnItemRemovedHandler();
		WST_WalletPredicate walletPredicate = new WST_WalletPredicate();
		IEntity wallet = mmc.FindItem(walletPredicate);
		if(wallet)
		{
			MoneyComponent mc = MoneyComponent.Cast(wallet.FindComponent(MoneyComponent));
			if (mc)
			{
				mmc.m_MoneyComponent = mc;
			}

		}
		
		//kick of gear persistence loading from here
		if (pc)
		{
			
			string playerUID = WST_GearPersistenceManager.GetPlayerIdentity(playerId);
			
			Print("GameModeWasteland::Player UID: "+ playerUID);
			ref WST_GearObject o = m_persistenceManager.GetGearObject(playerUID);
			if (o)
				Print("GameModeWasteland::GearObject Found! Loading gear ");


			
			if(IsMaster())
			{
				
				if (o)
				{
					if(mmc)
					{
						Print("GameModeWasteland::itemsOnPlayer count: "+ o.itemsOnPlayer.Count());
						if(o.itemsOnPlayer.Count() == 0)
							return;
					
						
						GetGame().GetCallqueue().CallLater(DeleteAllItems,1000,false,mmc);

						GetGame().GetCallqueue().CallLater(AddAllItems,1200,false,mmc,o);
					}
				}
			}
				
		}
				
				
			
			
			
			SCR_CampaignNetworkComponent campaignNetworkComponent = SCR_CampaignNetworkComponent.Cast(pc.FindComponent(SCR_CampaignNetworkComponent));
			
			if (campaignNetworkComponent)
			{
				campaignNetworkComponent.UpdatePlayerRank(false);
				campaignNetworkComponent.EnableShowingSpawnPosition(true)
			}
	}
	
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
	override void OnSpawnPointUsed(SCR_SpawnPoint spawnPoint, int playerId)
	{
		IEntity parent = spawnPoint.GetParent();
		SCR_CampaignBase base = null;
		
		while (parent)
		{
			if (parent.Type() == SCR_CampaignBase)
			{
				base = SCR_CampaignBase.Cast(parent);
				break;
			}
			
			parent = parent.GetParent();
		}
		
		if (!base)
			return;
		
		if (!base.GetIsHQ())			
			base.AddSupplies(-base.GetBaseSpawnCost());
		
		// Location popup for player
		PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerId);
	
		if (playerController)
		{
			SCR_CampaignNetworkComponent campaignNetworkComponent = SCR_CampaignNetworkComponent.Cast(playerController.FindComponent(SCR_CampaignNetworkComponent));
			
			if (campaignNetworkComponent)
				campaignNetworkComponent.RespawnLocationPopup(base.GetBaseID());
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void OnSpawnPointFactionAssigned(SCR_SpawnPoint spawnpoint)
	{
		SCR_PlayerRadioSpawnPointCampaign spawnpointC = SCR_PlayerRadioSpawnPointCampaign.Cast(spawnpoint);
		
		if (!spawnpointC)
			return;
		
		FactionKey faction = spawnpointC.GetFactionKey();
		
		if (!faction.IsEmpty())
		{
			if (GetActiveRespawnRadiosCount(faction) >= GetMaxRespawnRadios())
				spawnpointC.DeactivateSpawnPointPublic();
			else
				AddActiveRespawnRadio(faction, spawnpointC);
		}
		else
		{
			Faction previousFaction = spawnpointC.GetCachedFaction();
			
			if (previousFaction)
				RemoveActiveRespawnRadio(previousFaction.GetFactionKey(), spawnpointC);
		}	
	}
	
	//------------------------------------------------------------------------------------------------
	void OnSpawnPointRemoved(SCR_SpawnPoint spawnpoint)
	{
		SCR_PlayerRadioSpawnPointCampaign spawnpointC = SCR_PlayerRadioSpawnPointCampaign.Cast(spawnpoint);
		
		if (!spawnpointC)
			return;
		
		FactionKey faction = spawnpointC.GetFactionKey();
		
		if (faction.IsEmpty())
			return;
		
		RemoveActiveRespawnRadio(faction, spawnpointC);
	}
	
	//------------------------------------------------------------------------------------------------
	override void HandleOnCharacterDeath(notnull CharacterControllerComponent characterController, IEntity instigator)
	{
		super.HandleOnCharacterDeath(characterController, instigator);
		
		// Game ended
		if (m_fGameEnd != -1)
			return;
		
		if (!characterController)
			return;
		
		// Get rid of player map icon
		SCR_ChimeraCharacter character = characterController.GetCharacter();
		if (!character)
			return;
		
		MapDescriptorComponent comp = MapDescriptorComponent.Cast(character.FindComponent(MapDescriptorComponent));
		
		if (comp)
		{
			MapItem item = comp.Item();
			
			if (item)
				item.SetVisible(false);
		}
		
		if (character == ChimeraCharacter.Cast(SCR_PlayerController.GetLocalControlledEntity()))
		{
			m_aBasesWithPlayer.Clear();
			m_FirstBaseWithPlayer = null;
			m_SupplyDepotWithPlayer = null;
			SCR_PopUpNotification.GetInstance().HideCurrentMsg();
		}
		
		if (IsProxy())
			return;
		
		// Handle remnants respawn
		AIControlComponent control = AIControlComponent.Cast(character.FindComponent(AIControlComponent));
		
		if (control)
		{
			AIAgent agent = control.GetControlAIAgent();
			
			if (agent)
			{
				SCR_AIGroup group = SCR_AIGroup.Cast(agent.GetParentGroup());
				
				if (group)
				{
					int respawnPeriod;
					
					foreach (SCR_CampaignRemnantsPresence remnantPresence : m_aRemnantsPresence)
					{
						if (group != remnantPresence.GetSpawnedGroup())
							continue;
						
						// If this was not the last member alive, do nothing
						if (group.GetAgentsCount() > 1)
							break;
////////////////////////////////////////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////////////////////////////////////////	
						//hanlde AI Map descriptor update on group death					
						//if this was the last agent, disable map descriptor!!						
						SCR_CampaignRemnantsSpawnPoint sp = remnantPresence.GetSpawnPointSP();
						string identi = sp.getIdent();
						RpcDo_HideDescriptor(identi);
						Print("BaseGameModeWasteland::HideDescriptor ident: " +identi);
						Rpc(RpcDo_HideDescriptor,identi);
						
////////////////////////////////////////////////////////////////////////////////////////////////////////////	
////////////////////////////////////////////////////////////////////////////////////////////////////////////
						respawnPeriod = remnantPresence.GetRespawnPeriod();
						
						// Set up respawn timestamp, convert s to ms, reset original group size
						if (respawnPeriod > 0)
						{
							remnantPresence.SetRespawnTimestamp(Replication.Time() + (respawnPeriod * 1000));
							remnantPresence.SetMembersAlive(-1);
							remnantPresence.SetIsSpawned(false);
						}
						
						break;
					}
				}
			}
		}
		
		// Handle XP for kill
		//Entry point for money handling?!
		if (!instigator)
			return;
		
		SCR_ChimeraCharacter instigatorChar;
	
		// Instigator is a vehicle, find the driver
		if (instigator.IsInherited(Vehicle))
		{
			instigatorChar = SCR_PlayerPenaltyComponent.GetInstigatorFromVehicle(instigator);
		}
		else
		{
			// Check if the killer is a regular soldier on foot
			instigatorChar = SCR_ChimeraCharacter.Cast(instigator);
			
			// If all else fails, check if the killer is in a vehicle turret
			if (!instigatorChar)
				instigatorChar = SCR_PlayerPenaltyComponent.GetInstigatorFromVehicle(instigator, true);
		}
		
		if (!instigatorChar)
			return;
		
		auto foundComponentVictim = character.FindComponent(FactionAffiliationComponent);
		auto foundComponentKiller = instigatorChar.FindComponent(FactionAffiliationComponent);

		if (!foundComponentKiller || !foundComponentVictim)
			return;

		auto castedComponent = FactionAffiliationComponent.Cast(foundComponentKiller);
		Faction killerFaction = castedComponent.GetAffiliatedFaction();
		castedComponent = FactionAffiliationComponent.Cast(foundComponentVictim);
		Faction victimFaction = castedComponent.GetAffiliatedFaction();
		
		if (killerFaction && victimFaction)
		{
			if (killerFaction == victimFaction)
			{
				if (instigatorChar != character)
					AwardXP(instigatorChar, CampaignXPRewards.FRIENDLY_KILL);
			}
			else
			{
				float multiplier = 1;
				
				//if (SCR_CampaignDefendTask.IsCharacterInAnyDefendTaskRange(instigatorChar))
					//multiplier = 1.25;
				
				if (instigatorChar.IsInVehicle())
					AwardXP(instigatorChar, CampaignXPRewards.ENEMY_KILL_VEH);
				else
					AwardXP(instigatorChar, CampaignXPRewards.ENEMY_KILL);
			}
		}
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcDo_HideDescriptor(string identifier)
	{
		foreach (SCR_CampaignRemnantsPresence p : m_aRemnantsPresence)
		{
			SCR_CampaignRemnantsSpawnPoint sp = p.GetSpawnPointSP();
			if (identifier == sp.getIdent())
			{
				Print("BaseGameModeWasteland::HideDescriptor ident: " +identifier);

				p.GetMapDescriptor().Item().SetVisible(false);
			
			}
		}
		
	}
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcDo_ShowDescriptor(string identifier)
	{
		foreach (SCR_CampaignRemnantsPresence p : m_aRemnantsPresence)
		{
			SCR_CampaignRemnantsSpawnPoint sp = p.GetSpawnPointSP();
			if (identifier == sp.getIdent())
			{
				Print("BaseGameModeWasteland::ShowDescriptor ident: " +identifier);

				p.GetMapDescriptor().Item().SetVisible(true);
			
			}
		}
		
	}
	//------------------------------------------------------------------------------------------------
	void ShowXP(bool visible)
	{
		m_bIsShowingXPBar = visible;
		SCR_HUDManagerComponent hudManager = GetGame().GetHUDManager();
		
		if(m_wXPInfo && hudManager)
		{
			m_wXPInfo.SetVisible(visible);
			return;
		}
		
		SCR_CampaignNetworkComponent networkComp;
		PlayerController pc = GetGame().GetPlayerController();
		if (pc)
			networkComp = SCR_CampaignNetworkComponent.Cast(pc.FindComponent(SCR_CampaignNetworkComponent));
		
		if (!networkComp)
			return;
		
		int totalXP = networkComp.GetPlayerXP();
		
		SCR_CampaignFaction faction = SCR_CampaignFaction.Cast(SCR_RespawnSystemComponent.GetLocalPlayerFaction());		
		if (!faction)
			return;
			
		SCR_CampaignFactionManager campaignFactionManager = SCR_CampaignFactionManager.GetInstance();
		ECharacterRank curRank = campaignFactionManager.GetRankByXP(totalXP);
		string rankText = faction.GetRankName(curRank);
		ResourceName rankIconName = faction.GetRankInsignia(curRank);
			
		m_wXPInfo = hudManager.CreateLayout("{E0B82B4FCC95EE05}UI/layouts/HUD/CampaignMP/RankProgress.layout", EHudLayers.MEDIUM, 0);

		if (!m_wXPInfo)
			return;
			
		TextWidget title = TextWidget.Cast(m_wXPInfo.FindWidget("Title"));
		TextWidget rank = TextWidget.Cast(m_wXPInfo.FindWidget("Rank"));
		TextWidget rankNoIcon = TextWidget.Cast(m_wXPInfo.FindWidget("RankNoIcon"));
		ImageWidget rankIcon = ImageWidget.Cast(m_wXPInfo.FindWidget("RankIcon"));
		TextWidget skill = TextWidget.Cast(m_wXPInfo.FindWidget("Skill"));
			
		rank.SetTextFormat(rankText);
		if (!rankIconName.IsEmpty())
			rankIcon.LoadImageTexture(0, rankIconName);
		rankIcon.SetColor(Color.FromRGBA(226,168,79,255));
		rankIcon.SetVisible(true);
		rankNoIcon.SetTextFormat("");
			
		ProgressBarWidget progress = ProgressBarWidget.Cast(m_wXPInfo.FindWidget("Progress"));
		ProgressBarWidget progressDiff = ProgressBarWidget.Cast(m_wXPInfo.FindWidget("ProgressDiff"));
		
		progress.SetMin(campaignFactionManager.GetRankXP(curRank));
		progress.SetMax(campaignFactionManager.GetRankXP(campaignFactionManager.GetRankNext(curRank)));
		progress.SetCurrent(totalXP);
		progressDiff.SetMin(0);
		progressDiff.SetMax(1);
		progressDiff.SetCurrent(0);
		title.SetTextFormat("");
		m_wXPInfo.SetVisible(visible);
	}
	
	//------------------------------------------------------------------------------------------------	
	override void OnPlayerKilled(int playerId, IEntity player, IEntity killer)
	{
#ifdef ENABLE_DIAG
		if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_EXECUTE_GAMEMODE))
			return;
#endif
		super.OnPlayerKilled(playerId, player, killer);
		//get trader component and send Rpc to close all menus
		PlayerController pc = GetGame().GetPlayerManager().GetPlayerController(playerId);
		
		if (pc)
		{
			IEntity ie = pc.GetControlledEntity();
			WST_TraderComponent trdComp = WST_TraderComponent.Cast(ie.FindComponent(WST_TraderComponent));
			//close rpc
			trdComp.CloseAllMenus();
			
			
			
			SCR_CampaignNetworkComponent campaignNetworkComponent = SCR_CampaignNetworkComponent.Cast(pc.FindComponent(SCR_CampaignNetworkComponent));
			
			if (campaignNetworkComponent)
				campaignNetworkComponent.EnableShowingSpawnPosition(false)
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! What happens when a player is assigned a faction
	//! Called from SCR_RespawnSystemComponent
	//! \param playerID the id of the target player
	//! \param assignedFaction the faction that player was assigned or null if none
	override void HandleOnFactionAssigned(int playerID, Faction assignedFaction)
	{
		super.HandleOnFactionAssigned(playerID, assignedFaction);
		
		// Save faction selected in client's data
		if (!IsProxy())
		{
			SCR_CampaignClientData clientData;
			clientData = GetClientData(playerID);
			
			if (clientData && assignedFaction)
				clientData.SetFactionIndex(GetRespawnSystemComponent().GetFactionIndex(assignedFaction));
		}
		
		// When a faction is being assigned to the client automatically by server, playerId might not yet be registered
		// In that case, this saves the connecting player's data and processes them later in local OnPlayerRegistered()
		if (RplSession.Mode() != RplMode.Dedicated && !GetLastPlayerFaction())
		{
			if (SCR_PlayerController.GetLocalPlayerId() == playerID)
				ProcessFactionAssignment(assignedFaction);
			else
				m_aUnprocessedFactionAssignments.Insert(playerID, assignedFaction);
		}
		
		s_OnFactionAssigned.Invoke(playerID, assignedFaction);
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		if (NotPlaying())
			return;
		
#ifdef ENABLE_DIAG
		if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_EXECUTE_GAMEMODE))
			return;
#endif
		super.EOnInit(owner);
		
		// Initialize players list UI
		m_wPlayersList = GetGame().GetWorkspace().CreateWidgets("{3370BBD6566BB16C}UI/layouts/HUD/CampaignMP/CampaignMutePlayersUI.layout");
		m_wPlayersList.SetVisible(false);
		
		SCR_RespawnSystemComponent respawnSystem = SCR_RespawnSystemComponent.Cast(FindComponent(SCR_RespawnSystemComponent));		

		if (!respawnSystem)
			Print("There is no RespawnSystemComponent attached to the GameMode entity. Faction scoring will not work.");
		else 
			s_RespawnSystemComponent = respawnSystem;
		
		if (!IsProxy())
		{
			SCR_SpawnPoint.Event_SpawnPointFactionAssigned.Insert(OnSpawnPointFactionAssigned);
			SCR_SpawnPoint.Event_SpawnPointRemoved.Insert(OnSpawnPointRemoved);
			
			if (m_iCustomRadioRange != -1)
				OverrideBaseRadioRanges();
			
			SCR_FactionManager campaignFactionManager = SCR_FactionManager.Cast(GetGame().GetFactionManager());
			
			if (campaignFactionManager)
			{
				SCR_Faction fWest = SCR_Faction.Cast(campaignFactionManager.GetFactionByKey(FACTION_BLUFOR));
				SCR_Faction fEast = SCR_Faction.Cast(campaignFactionManager.GetFactionByKey(FACTION_OPFOR));
				
				/*if (fWest && fEast)
				{
					m_rAIVehicleWest = Resource.Load(fWest.GetAIGroupVehiclePrefab());
					m_rAIVehicleEast = Resource.Load(fEast.GetAIGroupVehiclePrefab());
				}*/
			}
			
			m_rRemnants = Resource.Load(m_RemnantsGroupPrefab);
			
			//GetGame().GetCallqueue().CallLater(CheckMobileAssemblies, 500, true);
			//GetGame().GetCallqueue().CallLater(CheckForWinner, 1000, true);
			
			SCR_PlayerSpawnPointManagerComponent comp = SCR_PlayerSpawnPointManagerComponent.Cast(FindComponent(SCR_PlayerSpawnPointManagerComponent));
			
			if (comp)
			{
				if (GetMaxRespawnRadios() >= 0)
				{
					comp.EnablePlayerSpawnPoints(true);
//					GetGame().GetCallqueue().CallLater(CheckRadioSpawnpointsSignalCoverage, 500, true);
				}
				else
					comp.EnablePlayerSpawnPoints(false);
			}
		}
		
		SCR_UITaskManagerComponent.s_OnTaskListVisible.Insert(ShowXP);
		
		if (RplSession.Mode() != RplMode.Dedicated)
		{
			//GetGame().GetCallqueue().CallLater(CheckPlayerInsideRadioRange, 3000, true);
			//GetGame().GetCallqueue().CallLater(EnteringNewBase, 3000, true);
			//GetGame().GetCallqueue().CallLater(CheckForBasesWithPlayer, 500, true);
			GetGame().GetCallqueue().CallLater(UpdateHUD, 250, true);
			//GetGame().GetCallqueue().CallLater(CheckPlayerPresenceInSupplyDepots, 2000, true);
			
			GetGame().GetInputManager().AddActionListener("TasksOpen", EActionTrigger.DOWN, RegisterTasksShown);
			GetGame().GetInputManager().AddActionListener("GadgetMap", EActionTrigger.DOWN, RegisterTasksShown);
		}
		
		foreach (SCR_CampaignSuppliesComponent comp : m_aRemnantSupplyDepots)
		{
			IEntity depot = comp.GetOwner();
			
			if (!depot)
				return;
			
			SCR_MapDescriptorComponent desc = SCR_MapDescriptorComponent.Cast(depot.FindComponent(SCR_MapDescriptorComponent));
			
			if (desc)
			{
				MapItem item = desc.Item();
				item.SetVisible(true);
				item.SetImageDef("Slot_Supplies");
				
				MapDescriptorProps props = item.GetProps();
				props.SetIconSize(32, 0.25, 0.25);
				props.SetFrontColor(GetGame().GetFactionManager().GetFactionByKey(SCR_GameModeCampaignMP.FACTION_INDFOR).GetFactionColor());
				props.SetTextVisible(false);
				props.Activate(true);
			}
			
			IEntity child = depot.GetChildren();
			SCR_SiteSlotEntity slot;
			
			while (child && !slot)
			{
				slot = SCR_SiteSlotEntity.Cast(child);
				
				if (!slot)
					child = child.GetSibling();
			}
			
			if (!slot)
				return;
			
			SCR_CampaignFaction fia = SCR_CampaignFaction.Cast(GetGame().GetFactionManager().GetFactionByKey(SCR_GameModeCampaignMP.FACTION_INDFOR));
			
			if (!fia)
				return;
			
			Resource res = Resource.Load(fia.GetBuildingPrefab(ECampaignCompositionType.SUPPLIES));
			
			if (!res)
				return;
			
			slot.SpawnEntityInSlot(res);
		}
		
		m_MapEntity = SCR_MapEntity.Cast(GetGame().GetMapManager());
		
		GetGame().GetCallqueue().CallLater(setTask,5000,false);
		
	

	}
	
	ref array<vector> m_MoveTaskDestinations = new array<vector>();
	void CreateMoveTaskDestinations()
	{
		if(IsProxy())
			return;
		if(!IsMaster())
			return;
		
		SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
	       if (!core)
	           return;
		set<SCR_EditableEntityComponent> entities = new set<SCR_EditableEntityComponent>();
		array<SCR_EditableEntityComponent> commentEntities = new array<SCR_EditableEntityComponent>();
		
		core.GetAllEntities(entities,true,true);
		//Print("SCRWST_AISpawnManager::EOnInit::AllSCR_EditableEntityComponent"+ entities);
		
		//filter out EEditableEntityType.COMMENT
		foreach (SCR_EditableEntityComponent ent : entities)
		{
			if (ent.GetEntityType() == 6)
				commentEntities.Insert(ent);
			//Print("SCRWST_AISpawnManagerClass::EOnInit()::EntityType = " +ent.GetEntityType() );
			
			
		}
		
		
		foreach (SCR_EditableEntityComponent ent : commentEntities)
		{
		
			GenericEntity parent =ent.GetOwner();
			SCR_MapDescriptorComponent mapDescr = SCR_MapDescriptorComponent.Cast(parent.FindComponent(SCR_MapDescriptorComponent));
			string LocationName;
			if (!mapDescr)
				return;
			MapItem item = mapDescr.Item();
			LocationName = item.GetDisplayName();
			string EntityName = parent.GetName();
			
			//filter out name with a capital "C" at the beginning. Those are Citys.
			//format city:
			//-> C_LocationChotain
			//format Hills 
			//-> H_HumboldtHill
			if(EntityName.Contains("C_"))
			{	
				//City 
				Print("BaseGameModeWasteland::CreateMoveTaskDestinations::Adding city, " + EntityName);
				vector v = parent.GetOrigin();
				m_MoveTaskDestinations.Insert(v);

				
			
			}else if (EntityName.Contains("H_"))
			{
				
				
			
			}
			
			Print("BaseGameModeWasteland::CreateMoveTaskDestinations::LocationName" + LocationName);
	
		}
	}
	
	
	void setTask()
	{
	
	//get support
		
		if(IsProxy())
			return;
		if(!IsMaster())
			return;
		CreateMoveTaskDestinations();
		vector v = m_MoveTaskDestinations.GetRandomElement();
		WST_DeliverTaskSupportEntity m_pSupportEntity;
		SCR_BaseTaskManager tmanager;
		tmanager = GetTaskManager();
		if (!GetTaskManager().FindSupportEntity(WST_DeliverTaskSupportEntity))
		{
			Print("CP: Default Task support entity not found in the world, task won't be created!");
			
		}
		m_pSupportEntity = WST_DeliverTaskSupportEntity.Cast(GetTaskManager().FindSupportEntity(WST_DeliverTaskSupportEntity));
		
		//get prefab
		if (!m_pSupportEntity.GetTaskPrefab())
		{
			
			Print("CP: Task prefab not set, task won't be created!");
			
			
			
			
			//m_pSupportEntity.SetTaskPrefab(m_sTaskPrefab);


			
		}
		//create object to deliver first, then create task 
		//OR 
		//define a item type to deliver and create task here
		WST_DeliverTask m_pTask;
	
		Faction faction = GetGame().GetFactionManager().GetFactionByKey("A");	
		m_pTask = m_pSupportEntity.CreateNewDeliverTask(faction,m_MoveTaskDestinations.GetRandomElement(),null,WST_Type.WST_WEAPON);
		
		
		
		
		
			
		//m_pTask.SetTargetFaction(GetGame().GetFactionManager().GetFactionByKey("A"));	
		if (!m_pTask)
		{
			PrintFormat("CP: Creating of task failed! Task manager refused to create it.");
			
		}
		if(m_pTask)
		{
			
			//GetTaskManager().RegisterTask(m_pTask);
			
		
		}
		
		
		//Create simple test task 
		/*
		protected bool SetSupportEntity()
	{
		if (!GetTaskManager().FindSupportEntity(SCR_CP_TaskSupportEntity))
		{
			Print("CP: Default Task support entity not found in the world, task won't be created!");
			return false;
		}
		m_pSupportEntity = SCR_CP_TaskSupportEntity.Cast(GetTaskManager().FindSupportEntity(SCR_CP_TaskSupportEntity));
		return m_pSupportEntity != null;	
	}
		
		protected bool SetTaskPrefab()
	{
		if (!m_pSupportEntity.GetTaskPrefab())
		{
			if (m_sTaskPrefab.IsEmpty())
			{
				Print("CP: Task prefab not set, task won't be created!");
				return false;
			}
			m_pSupportEntity.SetTaskPrefab(m_sTaskPrefab);
		}
		return true;
	}
		
		
		*/
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ref array<WST_VehicleSpawnPoint> m_vehicleSpawnPoints = new array<WST_VehicleSpawnPoint>();
	WST_GearPersistenceManager m_persistenceManager;
	SCR_MoneyManager GetMoneyManager()
	{
		if(IsMaster())
			return m_MoneyManager;
		return null;
	}
	void SetMoneyManager(SCR_MoneyManager manager)
	{
		 m_MoneyManager = manager;
	}
	void SetGearPersistenceManager(WST_GearPersistenceManager manager)
	{
		m_persistenceManager = manager;
	}
	WST_GearPersistenceManager GetGearPersistenceManager()
	{
		return m_persistenceManager;
	}
	void registerVehicleSpawn(WST_VehicleSpawnPoint vs)
	{
		m_vehicleSpawnPoints.Insert(vs);
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnFrame(IEntity owner, float timeSlice)
	{
#ifdef ENABLE_DIAG
		if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_RANK_UP))
		{
			DiagMenu.SetValue(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_RANK_UP, 0);
			PlayerController pc = GetGame().GetPlayerController();
			
			if (pc)
			{
				SCR_CampaignNetworkComponent comp = SCR_CampaignNetworkComponent.Cast(pc.FindComponent(SCR_CampaignNetworkComponent));
			
				if (comp)
					comp.CheatRank();
			}
		}
		
		if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_RANK_DOWN))
		{
			DiagMenu.SetValue(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_RANK_DOWN, 0);
			PlayerController pc = GetGame().GetPlayerController();
			
			if (pc)
			{
				SCR_CampaignNetworkComponent comp = SCR_CampaignNetworkComponent.Cast(pc.FindComponent(SCR_CampaignNetworkComponent));
			
				if (comp)
					comp.CheatRank(true);
			}
		}
		
		if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_EXECUTE_GAMEMODE))
			return;
#endif
		super.EOnFrame(owner, timeSlice);

		if (RplSession.Mode() != RplMode.Dedicated)
			ProcessUI(timeSlice);

#ifndef CONFLICT_SPAWN_ALL_AI	
		//if (!IsProxy())
			//Print("BaseGameModeWasteland::EOnFrame::SpawnAI and noProxy");
		//if (GetGame().AreGameFlagsSet( EGameFlags.SpawnAI ))
			//Print("BaseGameModeWasteland::GameFlags.SpawnAI = true");
			
		if (!IsProxy()) //&& GetGame().AreGameFlagsSet( EGameFlags.SpawnAI ))
			HandleRemnantForces(timeSlice);
#endif
		
		// End session
		if (m_fGameEnd != -1 && Replication.Time() >= m_fGameEnd)
		{
			m_fGameEnd = -1;
			
			if (RplSession.Mode() == RplMode.Dedicated)
			{
				/*BackendApi bApi = GetGame().GetBackendApi();
				
				if (bApi)
				{
					DSSession session = bApi.GetDSSession();
					
					if (session)
						session.RequestRestart();
				}*/
				GetGame().RequestClose();
			}
			else
				GameStateTransitions.RequestGameplayEndTransition();
		}
		
		if (m_Preload)
		{
			bool finished = m_Preload.Update(timeSlice);
			
			if (finished)
				m_Preload = null;
		}
	}
	
	
	
	

	//------------------------------------------------------------------------------------------------
	void SCR_BaseGameModeWasteland(IEntitySource src, IEntity parent)
	{
		FACTION_BLUFOR = m_sBLUFORFactionKey;
		FACTION_OPFOR = m_sOPFORFactionKey;
		FACTION_INDFOR = m_sINDFORFactionKey;
		
		
		if (NotPlaying())
			return;
		
#ifdef ENABLE_DIAG
		DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_EXECUTE_GAMEMODE, "", "Execute gamemode", "Conflict", true);
		DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_RANK_UP, "", "Promotion", "Conflict");
		DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_CAMPAIGN_RANK_DOWN, "", "Demotion", "Conflict");
#endif
		if (!s_Instance)
			s_Instance = this;
		
		SetEventMask(EntityEvent.INIT | EntityEvent.FRAME);
		SetFlags(EntityFlags.ACTIVE, true);
		
#ifdef TDM_CLI_SELECTION
		m_bApplyPresetOwners = true;
#endif
		
		if (!IsProxy())
		{
			SCR_MissionHeader header = SCR_MissionHeader.Cast(GetGame().GetMissionHeader());
			
			if (header)
			{
				// Set maximum amount of players and AI, header value cannot be higher than entity preset
				int playersLimitHeader = header.m_iPlayerCount;
			
				if (playersLimitHeader >= 0)
					m_iTotalPlayersLimit = Math.Min(m_iTotalPlayersLimit, playersLimitHeader);
				
				if (playersLimitHeader > m_iTotalPlayersLimit)
					Print("Header value of max players is higher than gamemode entity setting. Using entity setting...", LogLevel.WARNING);
				
				SCR_MissionHeaderCampaign campaignHeader = SCR_MissionHeaderCampaign.Cast(header);
	
				if (campaignHeader)
				{
					m_bAllowRemnantsDespawn = campaignHeader.m_bDespawnRemnants;
					m_iCustomRadioRange = campaignHeader.m_iCustomRadioRange;
					m_fXpMultiplier = campaignHeader.m_fXpMultiplier;
					m_bIgnoreMinimumVehicleRank = campaignHeader.m_bIgnoreMinimumVehicleRank;
					
					int suppliesMax = campaignHeader.m_iMaximumBaseSupplies;
					int suppliesMin = campaignHeader.m_iMinimumBaseSupplies;
					int respawnRadiosCnt = campaignHeader.m_iMaximumRespawnRadios;
					int controlPointsLimit = campaignHeader.m_iControlPointsCap;
					int victoryTimeout = campaignHeader.m_fVictoryTimeout;
					
					if (suppliesMax != -1)
						STARTING_SUPPLIES_MAX = suppliesMax;
					
					if (suppliesMin != -1)
						STARTING_SUPPLIES_MIN = suppliesMin;
					
					if (respawnRadiosCnt != -1)
						m_iMaxRespawnRadios = respawnRadiosCnt;
					
					if (controlPointsLimit != -1)
						m_iControlPointsThreshold = controlPointsLimit;
					
					if (victoryTimeout != -1)
						m_fVictoryTimer = victoryTimeout;
					
					Replication.BumpMe();
				}
			}
		
			// Players limit should be an even number
			if (m_iTotalPlayersLimit % 2 != 0)
				m_iTotalPlayersLimit++;
			
			// Set AI group sizes based on players limit
			m_iAIGroupSizeWest = Math.Round((m_iTotalPlayersLimit / 2) / AI_GROUPS_PER_FACTION);
			m_iAIGroupSizeEast = m_iAIGroupSizeWest;
		};
		
		//Parse & register XP reward list
		m_aXPRewardList = new array<ref SCR_CampaignXPRewardInfo>;
		Resource container2 = BaseContainerTools.LoadContainer("{E6FC4537B53EA00B}Configs/Campaign/XPRewards.conf");
		SCR_CampaignXPRewardList list2 = SCR_CampaignXPRewardList.Cast(BaseContainerTools.CreateInstanceFromContainer(container2.GetResource().ToBaseContainer()));
		list2.GetRewardList(m_aXPRewardList);
		
		//Register to script invokers
		s_OnBaseCaptured.Insert(OnBaseCaptured);
		SCR_CampaignBaseManager.s_OnAllBasesInitialized.Insert(AfterAllBasesInitialized);
		
		SCR_MapEntity.GetOnMapOpen().Insert(OnMapOpen);
		SCR_MapEntity.GetOnMapClose().Insert(OnMapClose);
	}

	//------------------------------------------------------------------------------------------------
	void ~SCR_BaseGameModeWasteland()
	{
		m_aRegisteredClients = null;
		m_aAIGroups = null;
		m_rRemnants = null;
		m_rAIVehicleWest = null;
		m_rAIVehicleEast = null;
		m_aBasesWithPlayer = null;
		m_aDedicatedSlots = null;
		m_Preload = null;
		m_aRemnantsPresence.Clear();
		m_aRemnantsPresence = null;
		
		if (m_wXPInfo)
			m_wXPInfo.RemoveFromHierarchy();
		
		if (m_wPlayersList)
			m_wPlayersList.RemoveFromHierarchy();
		
		if (m_wInfoOverlay)
			m_wInfoOverlay.RemoveFromHierarchy();
		
		if (m_wCountdown)
			m_wCountdown.RemoveFromHierarchy();
		
		delete SCR_CampaignBaseManager.GetInstance();
		
		//Unregister from script invokers
		s_OnBaseCaptured.Remove(OnBaseCaptured);
		SCR_UITaskManagerComponent.s_OnTaskListVisible.Remove(ShowXP);
		SCR_SpawnPoint.Event_SpawnPointFactionAssigned.Remove(OnSpawnPointFactionAssigned);
		SCR_SpawnPoint.Event_SpawnPointRemoved.Remove(OnSpawnPointRemoved);
		
		SCR_MapEntity.GetOnMapOpen().Remove(OnMapOpen);
		SCR_MapEntity.GetOnMapClose().Remove(OnMapClose);
	}

};

class WST_WalletPredicate: InventorySearchPredicate
	{
	void WST_WalletPredicate()
	{
		QueryComponentTypes.Insert(MoneyComponent);
	}

	override protected bool IsMatch(BaseInventoryStorageComponent storage, IEntity item, array<GenericComponent> queriedComponents, array<BaseItemAttributeData> queriedAttributes)
	{		
		MoneyComponent mc = MoneyComponent.Cast(item.FindComponent(MoneyComponent));
		if(mc)
			return true;	
		return false;
	}
};


enum WST_Type
{
	WST_WEAPON,
	WST_ATTACHMENT,
	WST_OPTIC,
	WST_EQUIPMENT,
	WST_VEST,
	WST_JACKET,
	WST_PANTS,
	WST_HELMET,
	WST_BACKPACK,
	WST_AMMO,
	WST_GRENADES,
	WST_EXPLOSIVES,
	WST_VEHICLES,
	WST_CONSUMABLE,
	WST_MEDIC,
	WST_VEHICLE
};
