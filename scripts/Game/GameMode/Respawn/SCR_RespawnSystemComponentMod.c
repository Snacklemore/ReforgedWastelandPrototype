

//! Scripted implementation that handles spawning and respawning of players.
//! Should be attached to a GameMode entity.

modded class SCR_RespawnSystemComponent
{
	
	//------------------------------------------------------------------------------------------------
	// Called from SCR_RespawnComponent
	void DoSetPlayerFaction(int playerId, int factionIndex)
	{
		
		Print("SCR_RespawnSystemComponent::DoSetPlayerFaction(playerId: "+playerId+", factionIndex: "+factionIndex+")");

		// Verify that faction makes sense
		if (!CanSetFaction(playerId, factionIndex))
		{
			ERespawnSelectionResult res = ERespawnSelectionResult.ERROR_FORBIDDEN;
			// In case no player controller is present, respawn component will not be present either,
			// can happen in case of disconnection
			SCR_RespawnComponent respawnComponent = SCR_RespawnComponent.Cast(GetGame().GetPlayerManager().GetPlayerRespawnComponent(playerId));
			if (respawnComponent)
				respawnComponent.AcknowledgePlayerFactionSet(res);
			return;
		}

		Faction faction = GetPlayerFaction(playerId);
		
		int oldIndex = GetFactionIndex(faction);
		if (oldIndex != factionIndex)
		{
			if (oldIndex != SCR_PlayerRespawnInfo.RESPAWN_INFO_INVALID_INDEX)
			{
				if (IsIntArrayIndexValid(m_aFactionPlayerCount, oldIndex))
				{
					m_aFactionPlayerCount[oldIndex] = m_aFactionPlayerCount[oldIndex]-1;
					Replication.BumpMe();
				}
			}

			if (IsIntArrayIndexValid(m_aFactionPlayerCount, factionIndex))
			{
				m_aFactionPlayerCount[factionIndex] = m_aFactionPlayerCount[factionIndex]+1;
				Replication.BumpMe();
			}
			else if (factionIndex != SCR_PlayerRespawnInfo.RESPAWN_INFO_INVALID_INDEX)
				Print("Provided Faction index in " + this.ToString() + " is out of array bounds!", LogLevel.ERROR);
		}

#ifdef RESPAWN_COMPONENT_VERBOSE
		Print("SCR_RespawnSystemComponent::DoSetPlayerFaction(playerId: "+playerId+", factionIndex: "+factionIndex+")");
#endif
		m_OnPlayerFactionChanged.Invoke(playerId, factionIndex);
		RpcDo_SetPlayerFaction(playerId, factionIndex);
		Rpc(RpcDo_SetPlayerFaction, playerId, factionIndex);

		Replication.BumpMe();

		ERespawnSelectionResult res = ERespawnSelectionResult.OK;
		// In case no player controller is present, respawn component will not be present either,
		// can happen in case of disconnection
		SCR_RespawnComponent respawnComponent = SCR_RespawnComponent.Cast(GetGame().GetPlayerManager().GetPlayerRespawnComponent(playerId));
		if (respawnComponent)
		{
			respawnComponent.AcknowledgePlayerFactionSet(res);
			Print("SCR_RespawnSystemComponent::DoSetPlayerFaction()::AcknowledgePlayerFactionSet");


		}
	}

	
	
	override void SetPlayerFaction(int playerId, int factionIndex)
	{
		
		Print("SCR_RespawnSystemComponent::SetPlayerFaction:Fired");

		SCR_PlayerRespawnInfo playerRespawnInfo = FindPlayerRespawnInfo(playerId);
		if (!playerRespawnInfo)
			playerRespawnInfo = RegisterPlayerRespawnInfo(playerId);

		Faction faction = GetFactionByIndex(factionIndex);

		if (playerRespawnInfo)
		{
			Print("SCR_RespawnSystemComponent::SetPlayerFaction:playerRespawnInfoAssert:FactionIndex = " + factionIndex);

			playerRespawnInfo.SetPlayerFactionIndex(factionIndex);

		}

		if (m_pGameMode)
		{
			//getting called on server
			Print("SCR_RespawnSystemComponent::SetPlayerFaction:callGameModeHandleOnFactionAssigned");

			m_pGameMode.HandleOnFactionAssigned(playerId, faction);
		}
					
	}
	
	//BROADCAST NOT FIRING?
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_SetPlayerFaction(int playerId, int factionIndex)
	{
#ifdef RESPAWN_COMPONENT_VERBOSE
		Print("SCR_RespawnSystemComponent::RpcDo_SetPlayerFaction(playerId: "+playerId+", factionIndex: "+factionIndex+")");
#endif
		Print("SCR_RespawnSystemComponent::RpcDo_SetPlayerFaction(playerId: "+playerId+", factionIndex: "+factionIndex+")");

		// Reset loadouts and spawn points as these might be faction specific.
		SetPlayerLoadout(playerId, -1);
		SetPlayerSpawnPoint(playerId, RplId.Invalid());

		SetPlayerFaction(playerId, factionIndex);
	}
//------------------------------------------------------------------------------------------------
	// Called when a spawn is requested
	// Asks the gamemode with PickPlayerSpawnPoint query expecting to get a spawn point
	// at which the player should be spawned
	protected override GenericEntity RequestSpawn(int playerId)
	{
		PlayerManager pm = GetGame().GetPlayerManager();
		SCR_ReconnectComponent reconComp = SCR_ReconnectComponent.GetInstance();
		if (!reconComp || !reconComp.IsReconnectEnabled() || !reconComp.IsInReconnectList(playerId) )
		{
		}
		else 
		{
			IEntity ent = reconComp.ReturnControlledEntity(playerId);
			if (ent)
				return GenericEntity.Cast(ent);
		}

		// Catch illicit requests,
		// TODO@AS:
		// TODO@LK:
		// We should probably make it so RequestRespawn()
		// is not even called from client if !CanPlayerRespawn(playerId)
		// and only resort to this as a safety measure
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (gameMode && !gameMode.CanPlayerRespawn(playerId) && !m_bCustomRespawn)
		{
			Print("Requested spawn denied! GameMode returned false in CanPlayerRespawn() for playerId=" + playerId, LogLevel.WARNING);
			return null;
		}
		SCR_SpawnPoint spawnPoint = GetPlayerSpawnPoint(playerId);
		m_bCustomRespawn = true;
		m_sCustomRespawnPrefab = "{65BB473E0E099118}Prefabs\Characters\Campaign\Final\Campaign_USSR_PlayerWaste.et";
		vector pos ;
		vector rot ;
		spawnPoint.GetPositionAndRotation(pos,rot);
		m_vCustomRespawnPos = pos;
		m_vCustomRespawnRot = rot;
		if (m_bCustomRespawn)
		{
			m_CustomSpawnedEntity = DoSpawn(m_sCustomRespawnPrefab, m_vCustomRespawnPos, m_vCustomRespawnRot);
			if (!m_CustomSpawnedEntity)
				return null;

			FactionAffiliationComponent affiliationComp = FactionAffiliationComponent.Cast(m_CustomSpawnedEntity.FindComponent(FactionAffiliationComponent));
			if (affiliationComp)
			{
				//Faction faction = affiliationComp.GetAffiliatedFaction();
				Faction faction = GetPlayerFaction(playerId);
				affiliationComp.SetAffiliatedFaction(faction);
				if (faction)
					DoSetPlayerFaction(playerId, GetFactionIndex(faction));
			}
			
			
			

			return m_CustomSpawnedEntity;
		}
		return m_CustomSpawnedEntity;
		
	}
	
	//------------------------------------------------------------------------------------------------

	void HandlePlayerRespawnInfos()
	{
		// if (RplSession.Mode() == RplMode.Dedicated)
		// 	return;

		// int plId = SCR_PlayerController.GetLocalPlayerId();
		// SCR_PlayerRespawnInfo info = FindPlayerRespawnInfo(plId);
		// if (info || plId > 0)
		// 	m_bInfoRegistered = true;


		//Print("SCR_RespawnSystemComponent::HandlePlayerRespawnInfos - playerId: " + plId + ", found: " + (info != null));

		for (int i = 0; i < m_mPlayerRespawnInfos.Count(); i++)
		{
			SCR_PlayerRespawnInfo pInfo = m_mPlayerRespawnInfos[i];
			Print("  item " + i + ", playerId: " + pInfo.GetPlayerID());
		}

	}


};