

//------------------------------------------------------------------------------------------------
//! Dummy communicator for RespawnSystem.
//! Must be attached to PlayerController entity.s
modded class SCR_RespawnComponent 
{
	// Parent entity (owner) - has to be a player controller for RPCs
	protected PlayerController m_PlayerController;
	// Parent entity's rpl component
	protected RplComponent m_RplComponent;
	// RespawnSystemComponent - has to be attached on a gameMode entity
	protected SCR_RespawnSystemComponent m_RespawnSystemComponent;

	static ref ScriptInvoker s_OnSpawn = new ScriptInvoker();

	#ifdef RESPAWN_COMPONENT_DIAG
		static bool s_DebugRegistered;
	#endif


	// Local faction selection lock w/ timeout
	private ref SCR_RespawnComponentLock m_pFactionLock = new SCR_RespawnComponentLock();
	// Local loadout selection lock w/ timeout
	private ref SCR_RespawnComponentLock m_pLoadoutLock = new SCR_RespawnComponentLock();
	// Local spawnPoint selection lock w/ timeout
	private ref SCR_RespawnComponentLock m_pSpawnPointLock = new SCR_RespawnComponentLock();
	// Local RequestRespawn selection lock w/ timeout, handled on gamecode side
	private ref LockStateInvoker m_pRespawnLock = new LockStateInvoker();

	/*!
		Returns invoker that is invoked when faction request lock engages or disengages.
		\return Invoker instance.
	*/
	LockStateInvoker GetFactionLockInvoker() { return m_pFactionLock.GetInvoker(); }
	/*!
		Returns invoker that is invoked when loadout request lock engages or disengages.
		\return Invoker instance.
	*/
	LockStateInvoker GetLoadoutLockInvoker() { return m_pLoadoutLock.GetInvoker(); }
	/*!
		Returns invoker that is invoked when spawn point request lock engages or disengages.
		\return Invoker instance.
	*/
	LockStateInvoker GetSpawnPointLockInvoker() { return m_pSpawnPointLock.GetInvoker(); }
	/*!
		Returns invoker that is invoked when request respawn lock engages or disengages.
		\return Invoker instance.
	*/
	LockStateInvoker GetRespawnLockInvoker() { return m_pRespawnLock; }
	
	#ifdef RESPAWN_COMPONENT_DIAG
	static string ResponseToString(ERespawnSelectionResult response)
	{
		switch (response)
		{
			case ERespawnSelectionResult.OK:
				return "OK";
			
			case ERespawnSelectionResult.ERROR:
				return "ERROR";
			
			case ERespawnSelectionResult.ERROR_FORBIDDEN:
				return "ERROR_FORBIDDEN";
		}
		
		return "UNREACHABLE_STATE";
	}
	#endif

	#ifdef RESPAWN_COMPONENT_DIAG
	//------------------------------------------------------------------------------------------------
	void DrawDebugInfo()
	{
		if (!m_RespawnSystemComponent)
		{
			Print("RespawnComponent::RespawnSystemNotFound!", LogLevel.DEBUG);
			return;
		}

		int localId = -1;
		ArmaReforgerScripted game = GetGame();
		if (game)
		{
			PlayerController playerController = game.GetPlayerController();
			localId = playerController.GetPlayerId();
		}

		SCR_PlayerRespawnInfo selfInfo = null;
		array<ref SCR_PlayerRespawnInfo> playerRespawnMap = m_RespawnSystemComponent.GetPlayerRespawnInfoMap();
		int cnt = playerRespawnMap.Count();

		const string header = "[PlayerID] [LoadoutIndex] [FactionIndex] [LoadoutName] [FactionKey] [SpawnPointIndex] [SpawnPointName] [PlayerRespawnInfo]";
		const string tableFmt = "%1 | %2 | %3 | %4 | %5 | %6 | %7 | %8 ";


		DbgUI.Begin("Respawn System Diag");
		DbgUI.Spacer(10);
		DbgUI.Text( header );
		for (int i = 0 ; i < cnt; i++)
		{
			SCR_PlayerRespawnInfo respawnInfo = playerRespawnMap[i];
			if (respawnInfo)
			{
				string respInfo = respawnInfo.ToString();
				int playerId = respawnInfo.GetPlayerID();
				int loadoutId = respawnInfo.GetPlayerLoadoutIndex();
				int factionId = respawnInfo.GetPlayerFactionIndex();
				RplId spawnPointId = respawnInfo.GetPlayerSpawnPointIdentity();

				if (localId != -1 && playerId == localId)
					selfInfo = respawnInfo;

				const string INVALID_STR = "INVALID";
				string loadoutName = INVALID_STR;
				string factionKey = INVALID_STR;
				string spawnPointName = INVALID_STR;

				Faction playerFaction = m_RespawnSystemComponent.GetPlayerFaction(playerId);
				SCR_BasePlayerLoadout playerLoadout = m_RespawnSystemComponent.GetPlayerLoadout(playerId);
				SCR_SpawnPoint spawnPoint = m_RespawnSystemComponent.GetPlayerSpawnPoint(playerId);
				if (playerFaction)
					factionKey = playerFaction.GetFactionKey();

				if (playerLoadout)
					loadoutName = playerLoadout.GetLoadoutName();

				if (spawnPoint)
					spawnPointName = spawnPoint.GetName();

				DbgUI.Text( string.Format(tableFmt, playerId, loadoutId, factionId, loadoutName, factionKey, spawnPointId, spawnPointName, respInfo) );
				DbgUI.Spacer(4);
			}
		}

		if (selfInfo)
		{
			int oldFac = selfInfo.GetPlayerFactionIndex();
			int oldLd = selfInfo.GetPlayerLoadoutIndex();
			RplId oldSpawn = selfInfo.GetPlayerSpawnPointIdentity();

			int requestedFaction = oldFac;
			int requestedLoadout = oldLd;
			int requestedSpawnpoint = oldSpawn;

			DbgUI.Spacer( 16 );
			DbgUI.Text(" --- Request RespawnInfo Change ---" );
			DbgUI.InputInt("new Loadout ID: ", requestedLoadout, 64);
			DbgUI.InputInt("new Faction ID: ", requestedFaction, 64);
			DbgUI.InputInt("new SpawnPoint ID: ", requestedSpawnpoint, 64);

			if (DbgUI.Button("Request new data", 64))
			{
				// if server has multiple ply controllers, call only from the "local one"
				// for players this check should always be ok
				// TODO: maybe move diag menu somewhere so it's not created for every instance of RespawnComponent
				if (m_RplComponent && m_RplComponent.IsOwner())
				{
					RequestPlayerLoadoutIndex(requestedLoadout);
					RequestPlayerFactionIndex(requestedFaction);
					RequestPlayerSpawnPointIdentity(requestedSpawnpoint);
					const string format = "--- RespawnSystem:RequestChange ---\nPlayerID: %1\nLoadout: %2 -> %3\nFaction: %4 -> %5\nSpawnPoint: %6 -> %7";
					Print( string.Format(format, localId, oldLd, requestedLoadout, oldFac, requestedFaction, oldSpawn, requestedSpawnpoint ));
				}
			}

			if (DbgUI.Button("Toggle Respawn Menu"))
				SCR_RespawnSystemComponent.ToggleRespawnMenu();
		}

		DbgUI.End();
	}
	#endif

	//------------------------------------------------------------------------------------------------
	static SCR_RespawnComponent GetInstance()
	{
		PlayerController playerController = GetGame().GetPlayerController();
		if (playerController)
			return SCR_RespawnComponent.Cast(playerController.FindComponent(SCR_RespawnComponent));
		else
			return null;
	}

	//------------------------------------------------------------------------------------------------
	RplComponent GetRplComponent()
	{
		return m_RplComponent;
	}

	bool RequestClearPlayerFaction()
	{
		if (!m_RespawnSystemComponent)
			return false;

		RequestPlayerFactionIndex(SCR_PlayerRespawnInfo.RESPAWN_INFO_INVALID_INDEX);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	bool RequestClearPlayerLoadout()
	{
		if (!m_RespawnSystemComponent)
			return false;

		RequestPlayerLoadoutIndex(SCR_PlayerRespawnInfo.RESPAWN_INFO_INVALID_INDEX);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	void RequestClearPlayerSpawnPoint()
	{
		RequestPlayerSpawnPointIdentity(RplId.Invalid());
	}

	//------------------------------------------------------------------------------------------------
	//! Ask the server to assign the local player to this loadout ptr
	bool RequestPlayerLoadout(SCR_BasePlayerLoadout loadout)
	{
		if (!m_RespawnSystemComponent)
			return false;

		int loadoutIndex = m_RespawnSystemComponent.GetLoadoutIndex(loadout);
		if (loadoutIndex == SCR_PlayerRespawnInfo.RESPAWN_INFO_INVALID_INDEX)
			return false;

		RequestPlayerLoadoutIndex(loadoutIndex);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	//! Ask the server to assign the local player to this loadout ptr
	bool RequestPlayerFaction(Faction faction)
	{
		if (!m_RespawnSystemComponent)
			return false;

		if (m_pFactionLock.IsLocked())
			return false;

		int factionIndex = m_RespawnSystemComponent.GetFactionIndex(faction);
		if (factionIndex == SCR_PlayerRespawnInfo.RESPAWN_INFO_INVALID_INDEX)
			return false;

		RequestPlayerFactionIndex(factionIndex);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	//! Ask the server to assign the local player to this spawnPoint
	bool RequestPlayerSpawnPoint(SCR_SpawnPoint spawnPoint)
	{
		if (!m_RespawnSystemComponent)
			return false;

		RplId spawnPointIdentity = SCR_SpawnPoint.GetSpawnPointRplId(spawnPoint);
		if (spawnPointIdentity == RplId.Invalid())
			return false;

		RequestPlayerSpawnPointIdentity(spawnPointIdentity);
		return true;
	}

	//------------------------------------------------------------------------------------------------
	//! Used by UseCLISpawn to set PlayerRespawnInfo fields without (for example) the loadout's faction
	//! check failing because the faction RPC hadn't completed yet.
	bool RequestCLISpawnInfo(Faction faction, SCR_BasePlayerLoadout loadout, SCR_SpawnPoint spawnPoint)
	{
		if (!m_RespawnSystemComponent)
			return false;

		if (m_pFactionLock.IsLocked() || m_pLoadoutLock.IsLocked() || m_pSpawnPointLock.IsLocked())
			return false;

		int factionIndex = m_RespawnSystemComponent.GetFactionIndex(faction);
		int loadoutIndex = m_RespawnSystemComponent.GetLoadoutIndex(loadout);
		RplId spawnPointIdentity = SCR_SpawnPoint.GetSpawnPointRplId(spawnPoint);

		if (factionIndex == SCR_PlayerRespawnInfo.RESPAWN_INFO_INVALID_INDEX ||
			loadoutIndex == SCR_PlayerRespawnInfo.RESPAWN_INFO_INVALID_INDEX ||
			spawnPointIdentity == RplId.Invalid())
		{
			return false;
		}

		m_pFactionLock.Lock();
		m_pLoadoutLock.Lock();
		m_pSpawnPointLock.Lock();

		Rpc(RpcAsk_SetPlayerFaction, factionIndex);
		Rpc(RpcAsk_SetPlayerLoadout, loadoutIndex);
		Rpc(RpcAsk_SetPlayerSpawnPoint, spawnPointIdentity);

		return true;
	}

	//------------------------------------------------------------------------------------------------
	void RequestPlayerSuicide()
	{
		if (!m_PlayerController)
			return;

		GenericEntity controlledEntity = GenericEntity.Cast(m_PlayerController.GetControlledEntity());
		if (!controlledEntity)
			return;

		CharacterControllerComponent characterController = CharacterControllerComponent.Cast(controlledEntity.FindComponent(CharacterControllerComponent));
		if (characterController)
		{
			characterController.ForceDeath();
			return;
		}
	}

	//------------------------------------------------------------------------------------------------
	void NotifyOnPlayerSpawned(int playerID)
	{
		if (m_PlayerController && playerID == m_PlayerController.GetPlayerId())
			Rpc(RpcAsk_NotifyOnPlayerSpawned);
	}

	//------------------------------------------------------------------------------------------------
	//! Ask the server to assign the local player to this loadoutIndex
	protected void RequestPlayerLoadoutIndex(int loadoutIndex)
	{
		if (m_pLoadoutLock.IsLocked())
			return;
		
		// Verify that loadout is allowed
		if (!m_RespawnSystemComponent.CanSetLoadout(m_PlayerController.GetPlayerId(), loadoutIndex))
		{
			Print("SCR_RespawnComponent::RequestPlayerLoadoutIndex request failed, forbidden request caught!", LogLevel.WARNING);
			return;
		}
		
		SCR_BasePlayerLoadout loadout = m_RespawnSystemComponent.GetLoadoutByIndex(loadoutIndex);
		#ifdef RESPAWN_COMPONENT_VERBOSE
			Print("SCR_RespawnComponent::RequestPlayerLoadoutIndex(loadoutIndex: "+loadoutIndex+")");
		#endif
		
		m_pLoadoutLock.Lock();
		Rpc(RpcAsk_SetPlayerLoadout, loadoutIndex);
	}

	//------------------------------------------------------------------------------------------------
	//! Ask the server to assign the local player to this factionIndex
	protected void RequestPlayerFactionIndex(int factionIndex)
	{
		if (m_pFactionLock.IsLocked())
			return;
		
		// Verify that faction is allowed
		if (!m_RespawnSystemComponent.CanSetFaction(m_PlayerController.GetPlayerId(), factionIndex))
		{
			Print("SCR_RespawnComponent::RequestPlayerFactionIndex request failed, forbidden request caught!", LogLevel.WARNING);
			return;
		}
		
		Faction faction = m_RespawnSystemComponent.GetFactionByIndex(factionIndex);
		
		#ifdef RESPAWN_COMPONENT_VERBOSE
			Print("SCR_RespawnComponent::RequestPlayerFactionIndex(factionIndex: "+factionIndex+")");
		#endif

		m_pFactionLock.Lock();
		Rpc(RpcAsk_SetPlayerFaction, factionIndex);
	}

	//------------------------------------------------------------------------------------------------
	//! Ask the server to assign the local player to this spawnPointIdentity
	protected void RequestPlayerSpawnPointIdentity(RplId spawnPointIdentity)
	{
		if (m_pSpawnPointLock.IsLocked())
			return;
		
		// Verify that loadout is allowed
		if (!m_RespawnSystemComponent.CanSetSpawnPoint(m_PlayerController.GetPlayerId(), spawnPointIdentity))
		{
			Print("SCR_RespawnComponent::RequestPlayerSpawnPointIdentity request failed, forbidden request caught!", LogLevel.WARNING);
			return;
		}

		#ifdef RESPAWN_COMPONENT_VERBOSE
			int id = spawnPointIdentity;
			Print("SCR_RespawnComponent::RequestPlayerSpawnPointIndex(spawnPointIdentity: "+id+")");
		#endif
		m_pSpawnPointLock.Lock();
		Rpc(RpcAsk_SetPlayerSpawnPoint, spawnPointIdentity);
	}

	//------------------------------------------------------------------------------------------------
	//! Ask the server to assign loadout by index  to the player that requested it
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_SetPlayerLoadout(int loadoutIndex)
	{
		if (m_PlayerController && m_RespawnSystemComponent)
		{
			#ifdef RESPAWN_COMPONENT_VERBOSE
				Print("SCR_RespawnComponent::RpcAsk_SetPlayerLoadout(loadoutIndex: "+loadoutIndex+")");
			#endif
			int playerId = m_PlayerController.GetPlayerId();
			m_RespawnSystemComponent.DoSetPlayerLoadout(playerId, loadoutIndex);
		}

	}
	//------------------------------------------------------------------------------------------------
	/*! Acknowledge player request was processed, send response to owner client. Server only.
		\param response The request response result.
	*/
	void AcknowledgePlayerLoadoutSet(ERespawnSelectionResult response)
	{
		Rpc(Rpc_RespondPlayerLoadoutSet_O);
	}

	//------------------------------------------------------------------------------------------------
	//! Notify the owner that request was processed.
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void Rpc_RespondPlayerLoadoutSet_O(ERespawnSelectionResult result)
	{
		m_pLoadoutLock.Break();
		
		#ifdef RESPAWN_COMPONENT_DIAG
		Print("SCR_RespawnComponent::PlayerLoadoutSet response received: " + ResponseToString(result));
		#endif
	}

	//------------------------------------------------------------------------------------------------
	//! Ask the server to assign faction by index to the player that requested it
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_SetPlayerFaction(int factionIndex)
	{
		

		if (m_PlayerController && m_RespawnSystemComponent)
		{
			#ifdef RESPAWN_COMPONENT_VERBOSE
				Print("SCR_RespawnComponent::RpcAsk_SetPlayerFaction(factionIndex: "+factionIndex+")");
			#endif

			int playerId = m_PlayerController.GetPlayerId();
			m_RespawnSystemComponent.DoSetPlayerFaction(playerId, factionIndex);
		}
	}

	//------------------------------------------------------------------------------------------------
	/*! Acknowledge player request was processed, send response to owner client. Server only.
		\param response The request response result.
	*/
	void AcknowledgePlayerFactionSet(ERespawnSelectionResult response)
	{

		Rpc(Rpc_RespondPlayerFactionSet_O);
	}
	
	
	static string ResponseToString(ERespawnSelectionResult response)
	{
		switch (response)
		{
			case ERespawnSelectionResult.OK:
				return "OK";
			
			case ERespawnSelectionResult.ERROR:
				return "ERROR";
			
			case ERespawnSelectionResult.ERROR_FORBIDDEN:
				return "ERROR_FORBIDDEN";
		}
		
		return "UNREACHABLE_STATE";
	}

	//------------------------------------------------------------------------------------------------
	//! Notify the owner that request was processed.
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void Rpc_RespondPlayerFactionSet_O(ERespawnSelectionResult result)
	{
		m_pFactionLock.Break();

		#ifdef RESPAWN_COMPONENT_DIAG
		Print("SCR_RespawnComponent::PlayerFactionSet response received: " + ResponseToString(result));
		#endif
	}

	//------------------------------------------------------------------------------------------------
	//! Ask the server to assign spawn point by rpl id to the player that requested it
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_SetPlayerSpawnPoint(RplId spawnPointIdentity)
	{
		if (m_PlayerController && m_RespawnSystemComponent)
		{
			#ifdef RESPAWN_COMPONENT_VERBOSE
				int id = spawnPointIdentity;
				Print("SCR_RespawnComponent::RpcAsk_SetPlayerSpawnPoint(spawnPointIdentity: "+id+")");
			#endif
			int playerId = m_PlayerController.GetPlayerId();
			m_RespawnSystemComponent.DoSetPlayerSpawnPoint(playerId, spawnPointIdentity);
		}
	}

	//------------------------------------------------------------------------------------------------
	/*! Acknowledge player request was processed, send response to owner client. Server only.
		\param response The request response result.
	*/
	void AcknowledgePlayerSpawnPointSet(ERespawnSelectionResult response)
	{
		Rpc(Rpc_RespondPlayerSpawnPointSet_O);
	}

	//------------------------------------------------------------------------------------------------
	//! Notify the owner that request was processed.
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void Rpc_RespondPlayerSpawnPointSet_O(ERespawnSelectionResult result)
	{
		m_pSpawnPointLock.Break();
		#ifdef RESPAWN_COMPONENT_DIAG
		Print("SCR_RespawnComponent::PlayerSpawnPointSet response received: " + ResponseToString(result));
		#endif
	}

	//------------------------------------------------------------------------------------------------
	// Notify the owner about being spawned
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RpcAsk_NotifyOnPlayerSpawned()
	{
		s_OnSpawn.Invoke();
	}
	
	//------------------------------------------------------------------------------------------------
	// ! HOTFIX for getting stuck in deploy menu when spawn killed
	void NotifyOnPlayerKilled()
	{
		Rpc(Rpc_NotifyOnPlayerKilled);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void Rpc_NotifyOnPlayerKilled()
	{
		SCR_RespawnSuperMenu menu = SCR_RespawnSuperMenu.GetInstance();
		if (menu)
			menu.ResetDeployRequest();
	} // HOTFIX end

	//------------------------------------------------------------------------------------------------
	/*!
		Acknowledge that player was enqueued for respawn to owner client. Server only.
	*/
	void AcknowledgePlayerEnqueued()
	{
		Rpc(Rpc_RespondPlayerSpawnEnqueued_O);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void Rpc_RespondPlayerSpawnEnqueued_O()
	{
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		gameMode.GetRespawnHandlerComponent().OnLocalPlayerEnqueued();
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Acknowledge that player was dequeued from respawn to owner client. Server only.
	*/
	void AcknowledgePlayerDequeued()
	{
		Rpc(Rpc_RespondPlayerSpawnDequeued_O);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void Rpc_RespondPlayerSpawnDequeued_O()
	{
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		gameMode.GetRespawnHandlerComponent().OnLocalPlayerDequeued();
	}

	//------------------------------------------------------------------------------------------------
	void RequestQuickRespawn()
	{
		Rpc(Rpc_RequestPlayerQuickRespawn);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void Rpc_RequestPlayerQuickRespawn()
	{
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (!gameMode)
			return;

		SCR_RespawnMenuHandlerComponent menuHandler = SCR_RespawnMenuHandlerComponent.Cast(gameMode.GetRespawnHandlerComponent());
		if (!menuHandler)
			return;

		menuHandler.RequestQuickRespawn(m_PlayerController.GetPlayerId());
	}


	//------------------------------------------------------------------------------------------------
	void RequestRespawn()
	{
		Rpc(Rpc_RequestPlayerRespawn);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void Rpc_RequestPlayerRespawn()
	{
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (!gameMode)
			return;

		SCR_RespawnMenuHandlerComponent menuHandler = SCR_RespawnMenuHandlerComponent.Cast(gameMode.GetRespawnHandlerComponent());
		if (!menuHandler)
			return;

		menuHandler.RequestRespawn(m_PlayerController.GetPlayerId());
	}

	//------------------------------------------------------------------------------------------------
	#ifdef RESPAWN_COMPONENT_DIAG
	override void OnDiag(IEntity owner, float timeSlice)
	{
		// Draw diag menu
		if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_RESPAWN_SYSTEM_INFO))
			DrawDebugInfo();
	}
	#endif

	/*!
		Event raised when state of lock for loadout selection changes.
		Always only relevant for local client.
		\param isLocked New state of the lock, true when engaged, false otherwise.
	*/
	protected void OnLoadoutLockChanged(bool isLocked)
	{
		#ifdef RESPAWN_COMPONENT_LOCKS_VERBOSE
		if (isLocked)
			Print("Loadout req. lock lock engaged! [X]");
		else
			Print("Loadout req. lock lock disengaged! [ ]");
		#endif
	}

	/*!
		Event raised when state of lock for faction selection changes.
		Always only relevant for local client.
		\param isLocked New state of the lock, true when engaged, false otherwise.
	*/
	protected void OnFactionLockChanged(bool isLocked)
	{
		#ifdef RESPAWN_COMPONENT_LOCKS_VERBOSE
		if (isLocked)
			Print("Faction req. lock lock engaged! [X]");
		else
			Print("Faction req. lock lock disengaged! [ ]");
		#endif
	}

	/*!
		Event raised when state of lock for spawn point selection changes.
		Always only relevant for local client.
		\param isLocked New state of the lock, true when engaged, false otherwise.
	*/
	protected void OnSpawnPointLockChanged(bool isLocked)
	{
		#ifdef RESPAWN_COMPONENT_LOCKS_VERBOSE
		if (isLocked)
			Print("SpawnPoint req. lock lock engaged! [X]");
		else
			Print("SpawnPoint req. lock lock disengaged! [ ]");
		#endif
	}
	
	/*!
		Event raised when state of lock for respawn request changes.
		Always only relevant for local client.
		\param isLocked New state of the lock, true when engaged, false otherwise.
	*/
	protected void OnRespawnLockChanged(bool isLocked)
	{
		#ifdef RESPAWN_COMPONENT_LOCKS_VERBOSE
		if (isLocked)
			Print("RequestRespawn lock lock engaged! [X]");
		else
			Print("RequestRespawn. lock lock disengaged! [ ]");
		#endif
	}
	
	/*!
		Called when player controller request respawn lock is engaged,
		i.e. a request was fired. Valid until response is received or
		until timeout. Relevant to the owner client.
	*/
	protected override void OnRequestLockEngaged()
	{
		if (m_pRespawnLock)
			m_pRespawnLock.Invoke(true);
		OnRespawnLockChanged(true);
	}
	
	/*!
		Called when player controller request respawn lock is disengaged,
		ie. a response is received from the server or on timeout(s).
		Relevant to the owner client.
		\param response The response from the server why the lock was lifted.
	*/
	protected override void OnRequestLockDisengaged(ERespawnResult result)
	{
		if (m_pRespawnLock)
			m_pRespawnLock.Invoke(false);
		OnRespawnLockChanged(false);
	}

	//------------------------------------------------------------------------------------------------
	override void OnFrame(IEntity owner, float timeSlice)
	{
		// Break all locks
		if (m_pFactionLock.IsLocked())
			m_pFactionLock.TryUnlock();

		if (m_pLoadoutLock.IsLocked())
			m_pLoadoutLock.TryUnlock();

		if (m_pSpawnPointLock.IsLocked())
			m_pSpawnPointLock.TryUnlock();
	}

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		m_PlayerController = PlayerController.Cast(owner);
		if (!m_PlayerController)
		{
			Print("SCR_RespawnComponent must be attached to PlayerController!", LogLevel.ERROR);
			return;
		}

		m_RplComponent = RplComponent.Cast(m_PlayerController.FindComponent(RplComponent));


		m_pFactionLock.GetInvoker().Insert(OnFactionLockChanged);
		m_pLoadoutLock.GetInvoker().Insert(OnLoadoutLockChanged);
		m_pSpawnPointLock.GetInvoker().Insert(OnSpawnPointLockChanged);

		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (!gameMode)
		{
			Print("No game mode found in the world, SCR_RespawnComponent will not function correctly!", LogLevel.ERROR);
			return;
		}

		m_RespawnSystemComponent = SCR_RespawnSystemComponent.Cast(gameMode.FindComponent(SCR_RespawnSystemComponent));

		gameMode.GetOnPlayerSpawned().Insert(NotifyOnPlayerSpawned);
		gameMode.GetOnPlayerKilled().Insert(NotifyOnPlayerSpawned);
	}

	//------------------------------------------------------------------------------------------------
	void SCR_RespawnComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
	{
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (gameMode)
		{
			gameMode.GetOnPlayerSpawned().Remove(NotifyOnPlayerSpawned);
			gameMode.GetOnPlayerKilled().Remove(NotifyOnPlayerSpawned);
		}

		#ifdef RESPAWN_COMPONENT_DIAG
		if (!s_DebugRegistered)
		{
			DiagMenu.RegisterBool(SCR_DebugMenuID.DEBUGUI_RESPAWN_SYSTEM_INFO, "", "Respawn System Diag", "Network");
			s_DebugRegistered = true;
		}
		#endif
	}

	//------------------------------------------------------------------------------------------------
	void ~SCR_RespawnComponent()
	{
		#ifdef RESPAWN_COMPONENT_DIAG
		DiagMenu.Unregister(SCR_DebugMenuID.DEBUGUI_RESPAWN_SYSTEM_INFO);
		s_DebugRegistered = false;
		#endif
	}
};
