

modded class SCR_CharacterTriggerEntity 
{	

	
	//------------------------------------------------------------------------------------------------
	void SetActivationPresence(TA_EActivationPresence EActivationPresence)
	{
		m_EActivationPresence = EActivationPresence;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetSpecificClass(string sClassName)
	{
		m_sSpecificClassName = sClassName;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetSpecificPrefabName(string sPrefabName)
	{
		m_sSpecificPrefabName = sPrefabName;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetOnce(bool bOnce)
	{
		m_bOnce = bOnce;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetNotificationEnabled(bool notificationEnabled)
	{
		m_bNotificationEnabled = notificationEnabled;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetEnableAudio(bool enableAudio)
	{
		m_bEnableAudio = enableAudio;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetMinimumPlayersNeeded(float minimumPlayersNeededPercentage)
	{
		m_fMinimumPlayersNeededPercentage = minimumPlayersNeededPercentage;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetPlayerActivationNotificationTitle(string sTitle)
	{
		m_sPlayerActivationNotificationTitle  = sTitle;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetPlayerActivationNotificationSubtitle(string sSubtitle)
	{
		m_sPlayerActivationNotificationSubtitle   = sSubtitle;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetActivationCountdownTimer(int activationCountdownTimer)
	{
		m_iActivationCountdownTimer = activationCountdownTimer;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetActivationCountdownTimerNotification(string sTitle)
	{
		m_sActivationCountdownTimerNotification   = sTitle;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetCountdownAudio(string sAudioName)
	{
		m_sCountdownAudio  = sAudioName;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void SetNotificationCanPopUp()
	{
		bNotificationCanPopUp  = true;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetOwnerFaction(FactionKey sFaction)
	{
		FactionManager factionManager = GetGame().GetFactionManager();
		if (factionManager)
			m_OwnerFaction = factionManager.GetFactionByKey(sFaction);
	}
	
	//------------------------------------------------------------------------------------------------
	bool IsMaster()// IsServer
	{
		RplComponent comp = RplComponent.Cast(FindComponent(RplComponent));
		return comp && comp.IsMaster();
	}
	
	//------------------------------------------------------------------------------------------------
	int GetCountEntitiesInside()
	{
		GetEntitiesInside(m_aEntitiesInside);
		return m_aEntitiesInside.Count();
	}
	
	//------------------------------------------------------------------------------------------------
	protected int GetPlayersCountByFaction()
	{
		int iCnt = 0;
		array<int> aPlayerIDs = {};
		SCR_PlayerController pPlayerCtrl;
		GetGame().GetPlayerManager().GetPlayers(aPlayerIDs);
		foreach (int iPlayerID: aPlayerIDs)
		{
			if (!m_OwnerFaction) 
			{
				iCnt++;			//Faction not set == ANY faction
			}
			else
			{
				pPlayerCtrl = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(iPlayerID));
				if (!pPlayerCtrl)
					continue;
				if (pPlayerCtrl.GetLocalControlledEntityFaction() == m_OwnerFaction)
					iCnt++;
			}
		}
		return iCnt;
	}
	
	//------------------------------------------------------------------------------------------------
	protected int GetSpecificClassCountInsideTrigger()
	{
		int iCnt = 0;
		GetEntitiesInside(m_aEntitiesInside);
		foreach (IEntity entity: m_aEntitiesInside)
		{
			if (entity.IsInherited(m_sSpecificClassName.ToType()))
				iCnt++;
		}
		return iCnt;
	}
	
	//------------------------------------------------------------------------------------------------
	protected int GetSpecificPrefabCountInsideTrigger()
	{
		int iCnt = 0;
		GetEntitiesInside(m_aEntitiesInside);
		foreach (IEntity entity: m_aEntitiesInside)
		{
			EntityPrefabData pPrefabData = entity.GetPrefabData();
			if (!pPrefabData)
				continue;
			
			ResourceName sPrefabName = pPrefabData.GetPrefabName();
			if (sPrefabName.IsEmpty() || sPrefabName != m_sSpecificPrefabName)
				continue;
			
			iCnt++;
		}
		return iCnt;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetCharacterCountByFactionInsideTrigger(Faction faction)
	{
		int iCnt = 0;
		SCR_ChimeraCharacter pChimeraChar;
		GetEntitiesInside(m_aEntitiesInside);
		foreach (IEntity entity: m_aEntitiesInside)
		{
			if (!faction) 
			{
				pChimeraChar = SCR_ChimeraCharacter.Cast(entity);
				if (!pChimeraChar)
					continue;
				
				iCnt++;			//Faction not set == ANY faction
			}
			else
			{
				pChimeraChar = SCR_ChimeraCharacter.Cast(entity);
				if (!pChimeraChar)
					continue;
				
				if (pChimeraChar.GetFaction() == faction)
					iCnt++;
			}
		}
		return iCnt;
	}
	
	
	
	//------------------------------------------------------------------------------------------------
	protected void GetPlayersByFactionInsideTrigger(notnull out array<IEntity> aOut)
	{
		SCR_ChimeraCharacter pChimeraChar;
		GetEntitiesInside(m_aEntitiesInside);
		foreach (IEntity entity : m_aEntitiesInside)
		{
			//Faction not set == ANY faction
			if (!m_OwnerFaction) 
			{
				pChimeraChar = SCR_ChimeraCharacter.Cast(entity);
				if (!pChimeraChar)
					continue;
				
				if (!EntityUtils.IsPlayer(entity))
					continue;
				
				aOut.Insert(entity)
			}
			else
			{
				
				pChimeraChar = SCR_ChimeraCharacter.Cast(entity);
				if (!pChimeraChar)
					continue;
				
				if (!EntityUtils.IsPlayer(entity))
					continue;
				
				if (pChimeraChar.GetFaction() == m_OwnerFaction)
					aOut.Insert(entity);
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void GetPlayersByFaction(notnull out array<IEntity> aOut)
	{
		array<int> aPlayerIDs = {};
		SCR_PlayerController pPlayerCtrl;
		GetGame().GetPlayerManager().GetPlayers(aPlayerIDs);
		foreach (int iPlayerID: aPlayerIDs)
		{
			pPlayerCtrl = SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(iPlayerID));
			if (!pPlayerCtrl)
				continue;
			if (pPlayerCtrl.GetLocalControlledEntityFaction() == m_OwnerFaction)
				aOut.Insert(pPlayerCtrl.GetLocalMainEntity());
		}
	}
	
	
	//------------------------------------------------------------------------------------------------
	//! Override this method in inherited class to define a new filter.
	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
 		if (ChimeraCharacter.Cast(ent) && !IsAlive(ent))
			return false;
		
		// take only players
		if (m_EActivationPresence == TA_EActivationPresence.PLAYER)
		{
			if (!EntityUtils.IsPlayer(ent))
				return false;
		}
		else if (m_EActivationPresence == TA_EActivationPresence.SPECIFIC_CLASS)
		{
			if (!ent.IsInherited(m_sSpecificClassName.ToType()))
				return false;
		}
			
		else if (m_EActivationPresence == TA_EActivationPresence.SPECIFIC_PREFAB_NAME)
		{
			EntityPrefabData pPrefabData = ent.GetPrefabData();
			if (!pPrefabData)
				return false;
			ResourceName sPrefabName = pPrefabData.GetPrefabName();
			if (sPrefabName.IsEmpty() || sPrefabName != m_sSpecificPrefabName)
				return false;
		}
				
		if (!m_OwnerFaction)
			return true;	//if faction is not specified, any faction can (de)activate the trigger
		FactionAffiliationComponent pFaciliation = FactionAffiliationComponent.Cast(ent.FindComponent(FactionAffiliationComponent));
		if (!pFaciliation)
			return false;
		return pFaciliation.GetAffiliatedFaction() == m_OwnerFaction;
	}
	
	//------------------------------------------------------------------------------------------------
	void DecreaseCountdown()
	{
		if (m_bNotificationEnabled)
		{
			string title = "";
			if ((tempWaitTime % 5) == 0)
				title = string.Format(m_sActivationCountdownTimerNotification, tempWaitTime);
								
			if (((tempWaitTime - 1) % 5) == 0)
				title = string.Format(m_sActivationCountdownTimerNotification, (tempWaitTime - 1));
								
			if (tempWaitTime <= 0)
				title = string.Format(m_sActivationCountdownTimerNotification, 0);
								
			if (title != "")
				PopUpMessage(title, -1, "", 1, 0, 0);
		}
		//This is because the trigger is querried every 2 seconds
		tempWaitTime -= 2;
		m_bTriggerCountdownOverlap = false;
	}
	
	//------------------------------------------------------------------------------------------------
	void FinishTrigger(IEntity ent)
	{
		if (m_sCountdownAudio != "" && m_bEnableAudio)
		{
			StopMusic(m_sCountdownAudio);
			m_bCountdownMusicPlaying = false;
		}
		if (m_bOnce)
			Deactivate();
		
		m_OnActivate.Invoke(ent);
		OnChange(ent);
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnActivate(IEntity ent)
	{
		if (!IsMaster())
			return;
		
		if (m_EActivationPresence == TA_EActivationPresence.PLAYER)
		{
			int playersCountByFactionInsideTrigger = GetPlayersCountByFactionInsideTrigger(m_OwnerFaction);
			float minPlayersNeeded = Math.Ceil(GetPlayersCountByFaction() * m_fMinimumPlayersNeededPercentage);
			
			if (!m_bTriggerCountdownOverlap)
			{
				m_iPlayerCountInsideTrigger = playersCountByFactionInsideTrigger;
				m_bTriggerCountdownOverlap = true;
			}
			
			if (playersCountByFactionInsideTrigger >= minPlayersNeeded)
			{
				if (m_bInitSequenceDone)
				{
					if (tempWaitTime <= -1)
					{
						FinishTrigger(ent);
					}
					else
					{
						if (m_iPlayerCountInsideTrigger <= 1)
							DecreaseCountdown();
						else
							m_iPlayerCountInsideTrigger--;
						
						if (!m_bCountdownMusicPlaying && m_bEnableAudio)
						{
							PlayMusic(m_sCountdownAudio);
							m_bCountdownMusicPlaying = true;	
						}
					}	
				}
			}
			else
			{
				tempWaitTime = m_iActivationCountdownTimer;
				PopUpMessage("", 1, "", 1, 0, 0);
				if (m_sCountdownAudio != "" || m_bEnableAudio)
				{
					StopMusic(m_sCountdownAudio);
					m_bCountdownMusicPlaying = false;
				}
				if (m_bNotificationEnabled && playersCountByFactionInsideTrigger > 0 && bNotificationCanPopUp)
				{
					string title = string.Format(m_sPlayerActivationNotificationTitle, playersCountByFactionInsideTrigger, GetPlayersCountByFaction());
					string description;
					if (minPlayersNeeded > 0)
						description = string.Format(m_sPlayerActivationNotificationSubtitle, minPlayersNeeded);
					
					PopUpMessage(title, 4, description, -1, 0, 0);
					bNotificationCanPopUp = false;
					//We do not want to spam the notifications that often, so we set the variable on and off after some time
					GetGame().GetCallqueue().CallLater(SetNotificationCanPopUp, 10000);
				}
			}
		}
		else if (m_EActivationPresence == TA_EActivationPresence.ANY_CHARACTER)
		{
			int countInsideTrigger = GetCharacterCountByFactionInsideTrigger(m_OwnerFaction);
			if (!m_bTriggerCountdownOverlap)
			{
				m_iCharCountInsideTrigger = countInsideTrigger;
				m_bTriggerCountdownOverlap = true;
			}
			
			if (countInsideTrigger > 0)
			{
				if (m_bInitSequenceDone)
				{
					if (tempWaitTime <= -1)
					{
						FinishTrigger(ent);
					}
					else
					{
						if (m_iCharCountInsideTrigger <= 1)
							DecreaseCountdown();
						else
							m_iCharCountInsideTrigger--;
						
						if (!m_bCountdownMusicPlaying && m_bEnableAudio)
						{
							PlayMusic(m_sCountdownAudio);
							m_bCountdownMusicPlaying = true;
						}
					}	
				}
			}
			else
			{
				tempWaitTime = m_iActivationCountdownTimer;
				PopUpMessage("", 1, "", 1, 0, 0);
			}	
		}
		else if (m_EActivationPresence == TA_EActivationPresence.SPECIFIC_CLASS)
		{
			int countInsideTrigger = GetSpecificClassCountInsideTrigger();
			if (!m_bTriggerCountdownOverlap)
			{
				m_iSpecificClassCountInsideTrigger = countInsideTrigger;
				m_bTriggerCountdownOverlap = true;
			}
			if (countInsideTrigger > 0)
			{
				if (m_bInitSequenceDone)
				{
					if (tempWaitTime <= -1)
					{
						FinishTrigger(ent);
					}
					else
					{
						if (m_iSpecificClassCountInsideTrigger <= 1)
							DecreaseCountdown();
						else
							m_iSpecificClassCountInsideTrigger--;
						
						if (!m_bCountdownMusicPlaying && m_bEnableAudio)
						{
							PlayMusic(m_sCountdownAudio);
							m_bCountdownMusicPlaying = true;
						}
					}	
				}
			}
			else
			{
				tempWaitTime = m_iActivationCountdownTimer;
				PopUpMessage("", 1, "", 1, 0, 0);
			}	
		}
		else if (m_EActivationPresence == TA_EActivationPresence.SPECIFIC_PREFAB_NAME)
		{
			int countInsideTrigger = GetSpecificPrefabCountInsideTrigger();
			if (!m_bTriggerCountdownOverlap)
			{
				m_iSpecificPrefabCountInsideTrigger = countInsideTrigger;
				m_bTriggerCountdownOverlap = true;
			}
			if (countInsideTrigger > 0)
			{
				if (m_bInitSequenceDone)
				{
					if (tempWaitTime <= -1)
					{
						FinishTrigger(ent);
					}
					else
					{
						if (m_iSpecificPrefabCountInsideTrigger <= 1)
							DecreaseCountdown();
						else
							m_iSpecificPrefabCountInsideTrigger--;
						
						if (!m_bCountdownMusicPlaying && m_bEnableAudio)
						{
							PlayMusic(m_sCountdownAudio);
							m_bCountdownMusicPlaying = true;
						}
					}	
				}
			}
			else
			{
				tempWaitTime = m_iActivationCountdownTimer;
				PopUpMessage("", 1, "", 1, 0, 0);
			}
		}	
		else
		{
			tempWaitTime = m_iActivationCountdownTimer;
			PopUpMessage("", 1, "", 1, 0, 0);
			if (m_sCountdownAudio != "" || !m_bEnableAudio)
			{
				StopMusic(m_sCountdownAudio);;
				m_bCountdownMusicPlaying = false;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override protected event void OnDeactivate(IEntity ent)
	{
		m_bTriggerCountdownOverlap = false;
		
		if (	m_EActivationPresence == TA_EActivationPresence.PLAYER || 
				m_EActivationPresence == TA_EActivationPresence.ANY_CHARACTER || 
				m_EActivationPresence == TA_EActivationPresence.SPECIFIC_CLASS ||
				m_EActivationPresence == TA_EActivationPresence.SPECIFIC_PREFAB_NAME 
		  )
		{
			if (m_bInitSequenceDone)
			{
				PopUpMessage("", 1, "", 1, 0, 0);
				if (m_sCountdownAudio != "" || !m_bEnableAudio)
				{
					StopMusic(m_sCountdownAudio);
					m_bCountdownMusicPlaying = false;
				}
				
				m_OnDeactivate.Invoke();
				OnChange(ent);
			}
		}
		
	}
	
	//------------------------------------------------------------------------------------------------
	void PlayMusic(string sAudio)
	{
		
		if(!m_MusicManager)
		{
			ChimeraWorld world = GetGame().GetWorld();
			if (world)
				m_MusicManager = world.GetMusicManager();
		}
		
		if (m_MusicManager)
			m_MusicManager.Play(sAudio);
		
		Rpc(RpcDo_PlayMusic, sAudio);
		
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcDo_PlayMusic(string sAudio)
	{
		if(!m_MusicManager)
		{
			ChimeraWorld world = GetGame().GetWorld();
			if (world)
				m_MusicManager = world.GetMusicManager();
		}
		
		if (m_MusicManager)
			m_MusicManager.Play(sAudio);
	}
	
	//------------------------------------------------------------------------------------------------
	void StopMusic(string sAudio)
	{
		if(!m_MusicManager)
		{
			ChimeraWorld world = GetGame().GetWorld();
			if (world)
				m_MusicManager = world.GetMusicManager();
		}
		
		if (m_MusicManager)
			m_MusicManager.Stop(sAudio);
		
		Rpc(RpcDo_StopMusic, sAudio);
		
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcDo_StopMusic(string sAudio)
	{
		if(!m_MusicManager)
		{
			ChimeraWorld world = GetGame().GetWorld();
			if (world)
				m_MusicManager = world.GetMusicManager();
		}
		
		if (m_MusicManager)
			m_MusicManager.Stop(sAudio);
	}
	
	//------------------------------------------------------------------------------------------------
	void PopUpMessage(string sTitle, float fDuration, string sSubtitle, int iPrio, float fProgressStart, float fProgressEnd)
	{
		if (IsMaster())
		{
			SCR_PopUpNotification.GetInstance().HideCurrentMsg();
			SCR_PopUpNotification.GetInstance().PopupMsg(text: sTitle, duration: fDuration, text2: sSubtitle, prio: iPrio, progressStart: fProgressStart, progressEnd: fProgressEnd);
		}
		
		Rpc(RpcDo_PopUpMessage, sTitle, fDuration, sSubtitle, iPrio, fProgressStart, fProgressEnd);
		
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcDo_PopUpMessage(string sTitle, float fDuration, string sSubtitle, int iPrio, float fProgressStart, float fProgressEnd)
	{
		SCR_PopUpNotification.GetInstance().HideCurrentMsg();
		SCR_PopUpNotification.GetInstance().PopupMsg(text: sTitle, duration: fDuration, text2: sSubtitle, prio: iPrio, progressStart: fProgressStart, progressEnd: fProgressEnd);
	}
	
	//------------------------------------------------------------------------------------------------
	void OnChange(IEntity ent)
	{
		if (m_OnChange)
		{
			CP_Param<IEntity> param = new CP_Param<IEntity>(ent);
			m_OnChange.Invoke(param);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	ScriptInvoker GetOnChange()
	{
		if (!m_OnChange)
			m_OnChange = new ScriptInvoker();
		return m_OnChange;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void SetInitSequenceDone()
	{
		m_bInitSequenceDone = true;
	}
	void SetSpawnPoint(SCR_SpawnPoint sp)
	{
		m_SeizeableFactionSpawn = sp;	
	
	}
	void GetNearbySpawnPoints(float f_radius)
	{
		vector pos = this.GetOrigin();
		if (GetGame().GetWorld())
			GetGame().GetWorld().QueryEntitiesBySphere(pos,f_radius,spawnPoint,TrueWhenEntityIsSpawnPoint,EQueryEntitiesFlags.ALL);
	
	}
	bool spawnPoint(notnull IEntity entity)
	{
		SCR_SpawnPoint sp = SCR_SpawnPoint.Cast(entity);
		if (!sp)
		{
			Print("SCR_CharacterTriggerEntityMod::spawnPoints()::notaspawnpointerror");
			return false;
		}
		SetSpawnPoint(sp);
		return true;
		

	}
	bool TrueWhenEntityIsSpawnPoint(IEntity entity)
	{
		SCR_SpawnPoint sp = SCR_SpawnPoint.Cast(entity);
		if (!sp)
		{
			return false;
		}
		return true;
	
	}
	
	vector mat;
	bool isContested;
	SCR_SpawnPoint m_SeizeableFactionSpawn;
	void HandleSpawnPoint()
	{
	
		if (!IsMaster())
			return;
		Print("SCR_CharacterTriggerEntity::OnTriggerChangeTestprint");
		if (isContested)
			return;
		//get players for Factions 
		//set spawn point to the faction with most players inside trigger
		Faction FactionA = GetGame().GetFactionManager().GetFactionByKey("A");				
		Faction FactionB = GetGame().GetFactionManager().GetFactionByKey("B");

		int FactionCountA = this.GetCharacterCountByFactionInsideTrigger(FactionA);
				Print("SCR_CharacterTriggerEntity::HandleSpawnPoint::FactionCountA = " + FactionCountA);

		int FactionCountB = this.GetCharacterCountByFactionInsideTrigger(FactionB);
				Print("SCR_CharacterTriggerEntity::HandleSpawnPoint::FactionCountB = " + FactionCountB);

		
		//no spawn
		
		
		GetGame().GetCallqueue().CallLater(CheckForNeutrality,3000,true);
		//workaround with preset spawnpoints that just get activated by players
		if (FactionCountA > FactionCountB)
		{
			m_SeizeableFactionSpawn.SetFactionKey("A");
			
		}
		if (FactionCountB > FactionCountA)
		{
			//spawn for faction B
			m_SeizeableFactionSpawn.SetFactionKey("B");

		}
		//keep last faction?
		if ( FactionCountB == FactionCountA)
		{
			m_SeizeableFactionSpawn.SetFactionKey("FIA");

		}

		
		
	}
	void CheckForNeutrality()
	{
		
		Faction FactionA = GetGame().GetFactionManager().GetFactionByKey("A");				
		Faction FactionB = GetGame().GetFactionManager().GetFactionByKey("B");

		int FactionCountACallQueue = this.GetCharacterCountByFactionInsideTrigger(FactionA);
				Print("SCR_CharacterTriggerEntity::CheckForNeutrality::FactionCountA = " + FactionCountACallQueue);

		int FactionCountBCallQueue = this.GetCharacterCountByFactionInsideTrigger(FactionB);
				Print("SCR_CharacterTriggerEntity::CheckForNeutrality::FactionCountB = " + FactionCountBCallQueue);
		
		if ((FactionCountACallQueue == 0) && (FactionCountBCallQueue == 0) )
		{
			Print("SCR_CharacterTriggerEntity::HandleSpawnPoint::return::b=0;a=0;");
			isContested = false;
			m_SeizeableFactionSpawn.SetFactionKey("FIA");
			GetGame().GetCallqueue().Remove(CheckForNeutrality);
			return;

			
		}
		
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void EOnInit(IEntity owner)
	{
		//TODO: we need a time to spawn entities inside the trigger, but we don't want to activate the trigger yet. 
		//It will be done better by knowing the entities inside the trigger on its creation
		GetGame().GetCallqueue().CallLater(SetInitSequenceDone, 1000);	
		SetOwnerFaction(m_sOwnerFactionKey);
		
		GetOnChange().Insert(HandleSpawnPoint);
		
		GetNearbySpawnPoints(25.0);
		ChimeraWorld world = GetGame().GetWorld();
		if (world)
			m_MusicManager = world.GetMusicManager();
	}
};
