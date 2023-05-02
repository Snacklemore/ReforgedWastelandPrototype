
//------------------------------------------------------------------------------------------------
//! Takes care of Campaign-specific server <> client communication and requests
modded class SCR_CampaignNetworkComponent 
{	
	
	//********************************//
	//RUNTIME SYNCHED MEMBER VARIABLES//
	//********************************//
	[RplProp(condition: RplCondition.OwnerOnly)]
	protected int m_iPlayerXP = 0;
	[RplProp(condition: RplCondition.OwnerOnly)]
	protected float m_fLastAssetRequestTimestamp = -int.MAX;
	[RplProp(condition: RplCondition.OwnerOnly)]
	protected float m_fLastHQRadioMessageTimestamp;
	
	//------------------------------------------------------------------------------------------------
	static SCR_CampaignNetworkComponent GetCampaignNetworkComponent(int playerID)
	{
		PlayerController playerController = GetGame().GetPlayerManager().GetPlayerController(playerID);
		
		if (!playerController)
			return null;
		
		SCR_CampaignNetworkComponent networkComponent = SCR_CampaignNetworkComponent.Cast(playerController.FindComponent(SCR_CampaignNetworkComponent));
		
		return networkComponent;
	}
	
	//*********************//
	//PUBLIC MEMBER METHODS//
	//*********************//
	
	map<SCR_SiteSlotEntity, ref SCR_CompositionBuildingQueue> GetBuildingQueue()
	{
		return m_mCurrentBuilding;
	}
	
	//------------------------------------------------------------------------------------------------
	void XPInfoOnKillsHint()
	{
		SCR_BaseGameModeWasteland campaign = SCR_BaseGameModeWasteland.GetInstance();
		
		if (!campaign)
			return;
		
		campaign.ShowHint(SCR_ECampaignHints.KILL_XP);
	}
	
	//------------------------------------------------------------------------------------------------
	void UpdatePlayerRank(bool notify = true)
	{
		IEntity player = m_PlayerController.GetMainEntity();
		
		if (!player)
			return;

		SCR_CharacterRankComponent comp = SCR_CharacterRankComponent.Cast(player.FindComponent(SCR_CharacterRankComponent));
		
		if (!comp)
			return;
		
		ECharacterRank curRank = comp.GetCharacterRank(player);
		ECharacterRank newRank = SCR_CampaignFactionManager.GetInstance().GetRankByXP(m_iPlayerXP);
		
		if (newRank == curRank)
			return;
		
		comp.SetCharacterRank(newRank);
		
		if (!notify)
			return;
		
		// Notification
		FactionAffiliationComponent affil = FactionAffiliationComponent.Cast(player.FindComponent(FactionAffiliationComponent));
		
		if (!affil)
			return;
		
		SCR_CampaignFaction f = SCR_CampaignFaction.Cast(affil.GetAffiliatedFaction());
		
		if (!f)
			return;
		
		SCR_ERadioMsg radio = SCR_ERadioMsg.NONE;
		
		if (newRank > curRank)
		{
			switch (newRank)
			{
				case ECharacterRank.PRIVATE: {radio = SCR_ERadioMsg.PROMOTION_PRIVATE; break;};
				case ECharacterRank.CORPORAL: {radio = SCR_ERadioMsg.PROMOTION_CORPORAL; break;};
				case ECharacterRank.SERGEANT: {radio = SCR_ERadioMsg.PROMOTION_SERGEANT; break;};
				case ECharacterRank.LIEUTENANT: {radio = SCR_ERadioMsg.PROMOTION_LIEUTENANT; break;};
				case ECharacterRank.CAPTAIN: {radio = SCR_ERadioMsg.PROMOTION_CAPTAIN; break;};
				case ECharacterRank.MAJOR: {radio = SCR_ERadioMsg.PROMOTION_MAJOR; break;};
			}
		} else {				
			switch (newRank)
			{
				case ECharacterRank.RENEGADE: {radio = SCR_ERadioMsg.DEMOTION_RENEGADE; break;};
				default: {radio = SCR_ERadioMsg.DEMOTION; break;}
			}
		}
		
		f.SendHQMessage(radio, calledID: m_PlayerController.GetPlayerId(), public: false, param: newRank)
	}
	
	//------------------------------------------------------------------------------------------------
	//! Addition to player XP
	void AddPlayerXP(CampaignXPRewards rewardID, float multiplier = 1.0, bool volunteer = false, int addDirectly = 0)
	{	
		SCR_BaseGameModeWasteland campaign = SCR_BaseGameModeWasteland.GetInstance();
		
		if (IsProxy() || !campaign /*|| campaign.IsTutorial()*/)
			return;
		
		if (addDirectly != 0)
		{
			m_iPlayerXP += (addDirectly);
			Replication.BumpMe();
			UpdatePlayerRank(false);
			Rpc(RpcDo_OnPlayerXPChanged, m_iPlayerXP, addDirectly, false, CampaignXPRewards.UNDEFINED, false, 0);
			return;
		}
		
		int XP = campaign.GetXPRewardAmount(rewardID);
		float skillXPMultiplier = 1;
		EProfileSkillID skillID = campaign.GetXPRewardSkill(rewardID);
		//auto profileManager = campaign.FindComponent(SCR_PlayerProfileManagerComponent); Replaced by SCR_PlayerData
		
		// 20% XP bonus when the player volunteered for the task
		if (volunteer)
			multiplier += 0.2;
		
		int XPToAdd = Math.Round(XP * multiplier * campaign.GetXPMultiplier());
		
		int XPToAddBySkill = 0;
		bool profileUsed = false;
		int skillLevel = 0;
		/***** Replaced by SCR_PlayerData
		//****
		// Handle skill XP
		if (profileManager && XP > 0)
		{
			SCR_PlayerProfileManagerComponent profileManagerCast = SCR_PlayerProfileManagerComponent.Cast(profileManager);
			CareerBackendData profile = profileManagerCast.GetPlayerProfile(m_PlayerController.GetPlayerId());
			
			if (profile)
			{
				profileUsed = true;
				skillLevel = Math.Min(Math.Floor(profile.GetSkillXP(skillID) / SCR_GameModeCampaignMP.SKILL_LEVEL_XP_COST), SCR_GameModeCampaignMP.SKILL_LEVEL_MAX);
				XPToAddBySkill = Math.Round(XPToAdd * skillLevel * SCR_GameModeCampaignMP.SKILL_LEVEL_XP_BONUS);
				profile.AddSkillXP(skillID, XP * multiplier);
			}
		}
		//****
		*****/
		m_iPlayerXP += (XPToAdd + XPToAddBySkill);
		Replication.BumpMe();
		UpdatePlayerRank();
		
		Rpc(RpcDo_OnPlayerXPChanged, m_iPlayerXP, XPToAdd, volunteer, rewardID, profileUsed, skillLevel);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Getter for player XP
	int GetPlayerXP()
	{
		return m_iPlayerXP;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Getter for request cooldown
	float GetLastRequestTimestamp()
	{
		return m_fLastAssetRequestTimestamp;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Getter for request cooldown
	void SetLastRequestTimestamp(float timestamp)
	{
		if (IsProxy())
			return;
		
		m_fLastAssetRequestTimestamp = timestamp;
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Checks if the session is run as client
	protected bool IsProxy()
	{
		return (m_RplComponent && m_RplComponent.IsProxy());
	}
	
	//------------------------------------------------------------------------------------------------
	//! Send server request to build object - requested on truck
	//! \param player slotEnt is a slot where to build
	//! \param suppliesComponent is a sup. comp. of the Vehicle
	//! \param compIndex is an index of composition in array of compositions 
	//! \param compValue is a value (cost) of the composition
	void BuildVehicle(EntityID slotID, notnull SCR_CampaignSuppliesComponent suppliesComponent, int compIndex, int compValue, vector slotAngles)
	{
		Rpc(RpcAsk_BuildVehicle, slotID, Replication.FindId(suppliesComponent), compIndex, compValue, slotAngles);
	}
		
	//------------------------------------------------------------------------------------------------
	//! Send server request to build object
	//! \param player slotEnt is a slot where to build
	//! \param base is a base where player is trying to build something.
	//! \param compIndex is an index of composition in array of compositions 
	//! \param compValue is a value (cost) of the composition
	
	void BuildBase(EntityID slotID, notnull SCR_CampaignBase base, int compIndex, int compValue, vector slotAngles)
	{
		Rpc(RpcAsk_BuildBase, slotID, Replication.FindId(base), compIndex, compValue, slotAngles);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Send server request to disassemble object
	//! \param player slotEnt is a slot used by a composition 
	//! \param base is a base where building mode was executed.
	//! \param refundValue is amount of resources which will be returned back to the player.
	void DisassembleComposition(EntityID slotID, notnull SCR_CampaignBase base, int refundValue)
	{
		Rpc(RpcAsk_DisassembleComposition, slotID, Replication.FindId(base), refundValue);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Send server request to disassemble composition - requested on truck
	//! \param player slotEnt is a slot used by a composition 
	//! \param suppliesComponent is a sup. comp. of the Vehicle
	//! \param refundValue is amount of resources which will be returned back to the player.
	void DisassembleCompositionVehicle(EntityID slotID, notnull SCR_CampaignSuppliesComponent suppliesComponent, int refundValue)
	{
		Rpc(RpcAsk_DisassembleCompositionVehicle,slotID, Replication.FindId(suppliesComponent), refundValue);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Repair destroyed mandatory part of composition
	void RepairComposition(int index, int repairCost, int destructibleID, SCR_SiteSlotEntity slotEnt, notnull SCR_CampaignBase base)
	{
#ifdef ENABLE_BASE_DESTRUCTION
		Rpc(RpcAsk_RepairComposition, index, repairCost, destructibleID, Replication.FindId(slotEnt), Replication.FindId(base));
#endif
	}
	
	//------------------------------------------------------------------------------------------------
	//! Send server request to load supplies at a base
	//! \param truckID Unique ID identifying the supply truck
	//! \param player Player trying to unload supplies
	//! \param base Base at which the supplies are being unloaded
	void LoadSupplies(RplId suppliesID, IEntity player, SCR_CampaignBase base, int amount = 0)
	{
		if (!player || !base)
			return;
		
		RplId baseID = Replication.FindId(base);
		
		if (!m_PlayerController)
			return;
		
		int playerID = m_PlayerController.GetPlayerId();
		Rpc(RpcAsk_LoadSupplies, suppliesID, playerID, baseID, amount);
	}
	
	//------------------------------------------------------------------------------------------------
	void LoadSuppliesStandalone(RplId suppliesID, IEntity player, SCR_CampaignSuppliesComponent depot, int amount = 0)
	{
		if (!player || !depot)
			return;
		
		RplId depotID = Replication.FindId(depot);
		
		if (!m_PlayerController)
			return;
		
		int playerID = m_PlayerController.GetPlayerId();
		Rpc(RpcAsk_LoadSuppliesStandalone, suppliesID, playerID, depotID, amount);
	}
	
	//------------------------------------------------------------------------------------------------
	void StartLoading(RplId suppliesID, int supplies, bool IsUnloading = false)
	{
		int playerID = m_PlayerController.GetPlayerId();
		Rpc(RpcAsk_SuppliesLoadingStarted, suppliesID, playerID, supplies, IsUnloading);
	}
	
	//------------------------------------------------------------------------------------------------
	void StopLoading(RplId suppliesID, bool IsUnloading = false)
	{
		int playerID = m_PlayerController.GetPlayerId();
		Rpc(RpcAsk_SuppliesLoadingCanceled, suppliesID, playerID, IsUnloading);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Send server request to unload supplies at a base
	//! \param truckID Unique ID identifying the supply truck
	//! \param player Player trying to unload supplies
	//! \param base Base at which the supplies are being unloaded
	void UnloadSupplies(RplId suppliesID, IEntity player, SCR_CampaignBase base, int amount = 0)
	{
		if (!player || !base)
			return;
		
		RplId baseID = Replication.FindId(base);
		
		if (!m_PlayerController)
			return;
		
		int playerID = m_PlayerController.GetPlayerId();
		Rpc(RpcAsk_UnloadSupplies, suppliesID, playerID, baseID, amount);
	}
	
	//------------------------------------------------------------------------------------------------
	void AddRadio()
	{
		Rpc(RpcAsk_AddRadio, SCR_PlayerController.GetLocalPlayerId());
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_AddRadio(int playerID)
	{
		PlayerManager pMan = GetGame().GetPlayerManager();
		
		if (!pMan)
			return;
		
		SCR_ChimeraCharacter player = SCR_ChimeraCharacter.Cast(pMan.GetPlayerControlledEntity(playerID));
		
		if (!player)
			return;
		
		EntitySpawnParams spawnParams = EntitySpawnParams();	
		spawnParams.TransformMode = ETransformMode.WORLD;
		player.GetWorldTransform(spawnParams.Transform);
		
		SCR_CampaignFaction faction = SCR_CampaignFaction.Cast(player.GetFaction());
		
		if (!faction)
			return;
		
		Resource res = Resource.Load(faction.GetRadioPrefab());
		
		if (!res)
			return;
		
		IEntity radio = GetGame().SpawnEntityPrefab(res, GetGame().GetWorld(),spawnParams);
		
		if (!radio)
			return;
		
		RplComponent rplC = RplComponent.Cast(radio.FindComponent(RplComponent));
		
		if (!rplC)
			return;
		
		Rpc(RpcDo_AddRadio, rplC.Id());
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RpcDo_AddRadio(RplId ID)
	{
		GetGame().GetCallqueue().CallLater(FindRadioDelayed, 10, true, ID)
	}
	
	//------------------------------------------------------------------------------------------------
	protected void FindRadioDelayed(RplId ID)
	{
		RplComponent rplC = RplComponent.Cast(Replication.FindItem(ID));
		
		if (!rplC)
			return;
		
		GetGame().GetCallqueue().Remove(FindRadioDelayed);
		IEntity player = SCR_PlayerController.GetLocalControlledEntity();
		
		if (!player)
			return;
		
		IEntity radio = rplC.GetEntity();
		
		if (!radio)
			return;
		
		SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(player.FindComponent(SCR_InventoryStorageManagerComponent));
		
		if (!inventory)
			return;
			
		inventory.InsertItem(radio);
	}
	
	//------------------------------------------------------------------------------------------------
	void DeployMobileAsembly(notnull SCR_CampaignMobileAssemblyComponent comp, bool deploy)
	{
		Rpc(RpcAsk_DeployMobileAsembly, Replication.FindId(comp), deploy, SCR_PlayerController.GetLocalPlayerId());
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_DeployMobileAsembly(RplId assemblyComponentID, bool deploy, int playerID)
	{
		SCR_CampaignMobileAssemblyComponent comp = SCR_CampaignMobileAssemblyComponent.Cast(Replication.FindItem(assemblyComponentID));
		
		if (!comp)
			return;
		
		float depth;
		if (SCR_WorldTools.IsObjectUnderwater(comp.GetOwner(), vector.Zero, -1, depth) && depth > SCR_CampaignMobileAssemblyComponent.MAX_WATER_DEPTH)
			return;
		
		bool deployedSuccessfully = false;
		
		if (comp.IsDeployed() != deploy)
			deployedSuccessfully = comp.Deploy(deploy);
		
		if (deployedSuccessfully)
		{
			int factionID = comp.GetParentFactionID();
			SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
			
			if (!campaign)
				return;
			
			if (deploy)
				campaign.MobileAssemblyFeedback(ECampaignClientNotificationID.ASSEMBLY_DEPLOYED, playerID, factionID);
			else
				campaign.MobileAssemblyFeedback(ECampaignClientNotificationID.ASSEMBLY_DISMANTLED, playerID, factionID);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Send server request to capture a base (change its owner)
	//! \param base Base to be captured
	void CaptureBase(SCR_CampaignBase base)
	{
		if (!base)
			return;
		
		Faction faction = SCR_RespawnSystemComponent.GetLocalPlayerFaction();
		int factionID = SCR_CampaignFactionManager.GetInstance().GetFactionIndex(faction);
		int playerID = m_PlayerController.GetPlayerId();
		IEntity player = m_PlayerController.GetControlledEntity();
		
		if (!player)
			return;
		
		CharacterControllerComponent comp = CharacterControllerComponent.Cast(player.FindComponent(CharacterControllerComponent));
		
		if (!comp)
			return;
		
		if (comp.IsDead())
			return;
		
		Rpc(RpcAsk_CaptureBase, Replication.FindId(base), factionID, playerID);
	}
	
	//------------------------------------------------------------------------------------------------
	void CaptureBaseGM(SCR_CampaignBase base, int factionIndex)
	{
		if (!base)
			return;
		
		Rpc(RpcAsk_CaptureBase, Replication.FindId(base), factionIndex, SCR_CampaignBase.INVALID_PLAYER_INDEX);
	}
	
	//------------------------------------------------------------------------------------------------
	void ToggleBaseCapture(notnull SCR_CampaignBase base, bool isBeingCaptured)
	{
		Faction faction = SCR_RespawnSystemComponent.GetLocalPlayerFaction();
		int factionID = SCR_CampaignFactionManager.GetInstance().GetFactionIndex(faction);
		int playerID = m_PlayerController.GetPlayerId();
		
		if (isBeingCaptured)
			Rpc(RpcAsk_CaptureBaseBegin, Replication.FindId(base), factionID, playerID);
		else
			Rpc(RpcAsk_CaptureBaseEnd, Replication.FindId(base));
	}
	
	//------------------------------------------------------------------------------------------------
	//! Cheat method to change player's rank
	void CheatRank(bool demote = false)
	{
		if (!m_PlayerController)
			return;
		
		int playerID = m_PlayerController.GetPlayerId();
		Rpc(RpcAsk_CheatRank, playerID, demote);
	}
	
	//***********//
	//RPC METHODS//
	//***********//
	
	//------------------------------------------------------------------------------------------------
	//! Cheat method to change player's rank - server side
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_CheatRank(int playerID, bool demote)
	{
		SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
		
		if (!campaign)
			return;
		
		SCR_CampaignFactionManager campaignFactionManager = SCR_CampaignFactionManager.GetInstance();
		
		ECharacterRank rank = campaignFactionManager.GetRankByXP(m_iPlayerXP);
		int reqXP;
		
		if (demote)
		{
			ECharacterRank newRank = campaignFactionManager.GetRankPrev(rank);
			
			if (newRank == ECharacterRank.INVALID)
				return;
			
			reqXP = campaignFactionManager.GetRankXP(newRank) - m_iPlayerXP;
		}
		else
		{
			ECharacterRank newRank = campaignFactionManager.GetRankNext(rank);
			
			if (newRank == ECharacterRank.INVALID)
				return;
			
			reqXP = campaignFactionManager.GetRankXP(newRank) - m_iPlayerXP;
		}
		
		IEntity controlledEnt = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerID);
		campaign.AwardXP(controlledEnt, CampaignXPRewards.CHEAT, reqXP);
	}

	//------------------------------------------------------------------------------------------------
	//! Build composition - Vehicle
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_BuildVehicle(EntityID slotID, RplId suppliesComponentID, int compIndex, int compValue, vector slotAngles)
	{
		
		if (compIndex  == -1|| compValue  == -1)
			return;
		
		SCR_SiteSlotEntity slotEnt = SCR_SiteSlotEntity.Cast(GetGame().GetWorld().FindEntityByID(slotID));
		if (!slotEnt || slotEnt.IsOccupied())
			return;

		SCR_CampaignSuppliesComponent suppliesComponent = SCR_CampaignSuppliesComponent.Cast(Replication.FindItem(suppliesComponentID));
		if (!suppliesComponent || suppliesComponent.GetSupplies() < compValue)
			return;

		Vehicle truck = Vehicle.Cast(suppliesComponent.GetOwner().GetParent());
		if (!truck)
		    return;
		
		SCR_CampaignFaction owningFaction = SCR_CampaignFaction.Cast(truck.GetDefaultFaction());
		if (!owningFaction)
		    return;

		// Spawn a trigger which handles the area of composition on server. Once it's clear, the composition is spawned.		
		SCR_CampaignBuildingServerTrigger trigger = CreateBuildingAreaTrigger(slotEnt);
		if (trigger)
			SetBuildingTrigger(trigger, slotEnt, owningFaction, compIndex, slotAngles);
		
		suppliesComponent.AddSupplies(-compValue);			
	}
		
	//------------------------------------------------------------------------------------------------
	//! Build composition - base
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_BuildBase(EntityID slotID, RplId baseID, int compIndex, int compValue, vector slotAngles)
	{
		if (compIndex == -1 || compValue == -1)
			return;
		
		SCR_SiteSlotEntity slotEnt = SCR_SiteSlotEntity.Cast(GetGame().GetWorld().FindEntityByID(slotID));
		if (!slotEnt || slotEnt.IsOccupied())
			return;
		
		SCR_CampaignBase base = SCR_CampaignBase.Cast(Replication.FindItem(baseID));
		if (!base || base.GetSupplies() < compValue)
			return;
		
		SCR_CampaignFaction owningFaction = base.GetOwningFaction();
		if (!owningFaction)
			return;	
				
		ResourceName resName = owningFaction.GetAvailableSlotResources()[compIndex];
		Resource composition = Resource.Load(resName);
		if (!composition.IsValid())
			return;
		
		base.AddSupplies(-compValue);
		
		// Spawn a trigger which handles the area of composition on server. Once it's clear, the composition is spawned.
		SCR_CampaignBuildingServerTrigger trigger = CreateBuildingAreaTrigger(slotEnt);
		if (trigger)
			SetBuildingTrigger(trigger, slotEnt, owningFaction, compIndex, slotAngles, base);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Setup of the spawned trigger.
	protected void SetBuildingTrigger(notnull SCR_CampaignBuildingServerTrigger trigger, notnull SCR_SiteSlotEntity slotEnt, notnull SCR_CampaignFaction owningFaction, int compIndex, vector slotAngles, SCR_CampaignBase base = null)
	{
		trigger.SetSlot(slotEnt);
		trigger.SetOwningFaction(owningFaction);
		trigger.SetCompositionIndex(compIndex);
		trigger.SetSlotAngle(slotAngles);
		if (base)
			trigger.SetBase(base);
		trigger.SetNetworkComponent(this);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Core of the composition building - create a queue of phases if exist, or spawn composition directly. 
	void BuildComposition(notnull SCR_SiteSlotEntity slotEnt, notnull SCR_CampaignFaction campaignFaction, int compIndex, vector slotAngles)
	{
		// Adjust the slot angle
		slotEnt.SetAngles(slotAngles);

		// Get the list of all phases in building process
		array<ref SCR_BuildingPhase> buildingPhases;
		int completeBuildingTime;
		
		ref SCR_CompositionBuildingQueue buildingQueue;
		buildingQueue = m_mCurrentBuilding.Get(slotEnt);
		if (!buildingQueue)
		{
			buildingQueue = new ref SCR_CompositionBuildingQueue();
			m_mCurrentBuilding.Insert(slotEnt, buildingQueue);
		}

		buildingPhases = campaignFaction.GetCampaignSlotsComposition()[compIndex].GetBuildingPhaseResources();
		
		PlayerController pc = PlayerController.Cast(GetOwner());
		if (!pc)
			return;
		
		for (int i = 0; i < buildingPhases.Count(); i++)
		{
			completeBuildingTime += buildingPhases[i].GetBuildingTime()*1000; 
			// 1st building phase goes imidiately, no need to call CallLater
			if (i == 0)
			{
				buildingQueue.BuildPhase(slotEnt, buildingPhases[i].GetPhaseResourceName(), false, pc.GetPlayerId());
				continue;
			}
					
			// Next phase will be spawned with defined time delay + delays of previous phases.
			GetGame().GetCallqueue().CallLater(buildingQueue.BuildPhase, completeBuildingTime, false, slotEnt, buildingPhases[i].GetPhaseResourceName(), false);
			completeBuildingTime += buildingPhases[i].GetBuildingTime()*1000; 
		}

		ResourceName resName = campaignFaction.GetAvailableSlotResources()[compIndex];	
			
		// Final phase of composition
		GetGame().GetCallqueue().CallLater(buildingQueue.BuildPhase, completeBuildingTime, false, slotEnt, resName, true, pc.GetPlayerId());
	}
	
	//------------------------------------------------------------------------------------------------
	//! Spawn a trigger to force AI leave the area
	SCR_CampaignBuildingServerTrigger CreateBuildingAreaTrigger(notnull IEntity ent)
	{
		Resource res = Resource.Load(m_sBuildingAreaTrigger);
		if (!res.IsValid())
			return null;
			
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;				
		params.Transform[3] = ent.GetOrigin();
		SCR_CampaignBuildingServerTrigger trigger = SCR_CampaignBuildingServerTrigger.Cast(GetGame().SpawnEntityPrefab(res, GetGame().GetWorld(), params));
		return trigger;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Disassemble composition - Vehicle
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_DisassembleCompositionVehicle(EntityID slotID, RplId suppliesComponentID, int refundValue)
	{
		if (refundValue  == -1)
			return;
		
		SCR_SiteSlotEntity slotEnt = SCR_SiteSlotEntity.Cast(GetGame().GetWorld().FindEntityByID(slotID));
		if (!slotEnt)
			return;
		
		SCR_BaseGameModeWasteland campaign = SCR_BaseGameModeWasteland.GetInstance();
		if (!campaign)
			return;
		
		SCR_CampaignSuppliesComponent suppliesComponent = SCR_CampaignSuppliesComponent.Cast(Replication.FindItem(suppliesComponentID));
		if (!suppliesComponent)
			return;

		if ((suppliesComponent.GetSupplies() + refundValue) >= suppliesComponent.GetSuppliesMax())
			{
				suppliesComponent.AddSupplies(suppliesComponent.GetSuppliesMax() - suppliesComponent.GetSupplies());
			}
			else 
			{
				suppliesComponent.AddSupplies(refundValue);
			}
		
		// Check for tbe building queue - composition is under construction. If so, remove the next steps.
		SCR_CompositionBuildingQueue buildingQueue = new SCR_CompositionBuildingQueue();
		m_mCurrentBuilding.Find(slotEnt, buildingQueue);
		if (buildingQueue)
		{
			GetGame().GetCallqueue().Remove(buildingQueue.BuildPhase);
			m_mCurrentBuilding.Remove(slotEnt);
		}
		
		SCR_EntityHelper.DeleteEntityAndChildren(slotEnt.GetOccupant());
		campaign.RefreshBuildingPreview(slotID);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Disassemble composition - base
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_DisassembleComposition(EntityID slotID, RplId baseID, int refundValue)
	{
		if (refundValue == -1)
			return;
		
		SCR_SiteSlotEntity slotEnt = SCR_SiteSlotEntity.Cast(GetGame().GetWorld().FindEntityByID(slotID));
		if (!slotEnt)
			return;
		
		SCR_CampaignBase base = SCR_CampaignBase.Cast(Replication.FindItem(baseID));
		if (!base)
			return;
		
		SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
		if (!campaign)
			return;
		
		base.AddSupplies(refundValue);
		base.HandleMapInfo();	//Update map info for host
		SCR_EntityHelper.DeleteEntityAndChildren(slotEnt.GetOccupant());
		campaign.RefreshBuildingPreview(slotID);
	}
	
#ifdef ENABLE_BASE_DESTRUCTION
	//------------------------------------------------------------------------------------------------
	//! Repair damaged entity in composition
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_RepairComposition(int index, int repairCost, int destructibleID, RplId slotID, RplId baseID)
	{
		if (index == -1 || repairCost == -1 || destructibleID == -1)
			return;
		
		SCR_MPDestructionManager destructionManager = SCR_MPDestructionManager.GetInstance();
		if (!destructionManager)
			return;
		
		SCR_DestructionMultiPhaseComponent destructibleComp = SCR_DestructionMultiPhaseComponent.Cast(destructionManager.FindDynamicallySpawnedDestructibleByIndex(destructibleID, index));
		if (!destructibleComp)
			return;
				
		SCR_CampaignBase base = SCR_CampaignBase.Cast(Replication.FindItem(baseID));
		if (!base)
			return;
		
		SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
		if (!campaign)
			return;
			
		IEntity composition = IEntity.Cast(SCR_EntityHelper.GetMainParent(destructibleComp.GetOwner()));
		if (!composition)
			return;
		
		SCR_CampaignServiceEntityComponent serviceEntityComp = SCR_CampaignServiceEntityComponent.Cast(destructibleComp.GetOwner().FindComponent(SCR_CampaignServiceEntityComponent));
		if (!serviceEntityComp)
			return; 
		
		SCR_CampaignServiceCompositionComponent serviceCompositionComp = SCR_CampaignServiceCompositionComponent.Cast(composition.FindComponent(SCR_CampaignServiceCompositionComponent));
		if (!serviceCompositionComp)
			return;	
		
		// Check if the composition entity belong to is disabled or not. If so, increase number of spawn tickets again.
		if (!serviceCompositionComp.IsServiceOperable())
			campaign.OnStructureChanged(base, SCR_SiteSlotEntity.Cast(Replication.FindItem(slotID)), base.GetBaseService(serviceCompositionComp.GetCompositionType()), true);
		
		// Repair entity
		serviceEntityComp.RepairEntity();
		
		// Supply in base are reduced (cost of repair)
		base.AddSupplies(-repairCost);
		
		// Update map UI
		base.HandleMapInfo();
	}
#endif
	
	//------------------------------------------------------------------------------------------------
	void AddSuppliesFromContextMenu(notnull SCR_CampaignBase base, int suppliesCnt)
	{
		RplId baseID = Replication.FindId(base);
		
		if (!baseID.IsValid())
			return;
		
		Rpc(RpcAsk_AddSuppliesFromContextMenu, baseID, suppliesCnt);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_AddSuppliesFromContextMenu(RplId baseID, int suppliesCnt)
	{
		SCR_CampaignBase base = SCR_CampaignBase.Cast(Replication.FindItem(baseID));
		
		if (!base)
			return;
		
		if (base.GetSupplies() >= base.GetSuppliesMax())
			return;
		
		base.AddSupplies(Math.Min(suppliesCnt, base.GetSuppliesMax() - base.GetSupplies()));
	}
	//------------------------------------------------------------------------------------------------
	// Sends player notification to players inside Vehicle
	// parameters: entity of vehicle, int ID of player to notify about, optional second parameter for another int number
	protected void SendToVehicleOccupants(ENotification messageID, IEntity vehicleEntity, int playerID, int number = 0)
	{
		array<IEntity> occupants = {}; 
		IEntity parentVehicle = vehicleEntity.GetParent();
		
		//Gettings players from inside of vehicle. Condition allows spawning only Cargo.
		SCR_BaseCompartmentManagerComponent comp = SCR_BaseCompartmentManagerComponent.Cast(parentVehicle.FindComponent(SCR_BaseCompartmentManagerComponent));
		
		if(!parentVehicle)
			return;
		
		comp = SCR_BaseCompartmentManagerComponent.Cast(parentVehicle.FindComponent(SCR_BaseCompartmentManagerComponent));
		comp.GetOccupants(occupants);
		
		if(occupants.IsEmpty())
			return;
		
		foreach (IEntity occupant : occupants)
		{
			int occupantID = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(occupant);
			if(number != 0)
				SCR_NotificationsComponent.SendToPlayer(occupantID, messageID, playerID, number);
			else
				SCR_NotificationsComponent.SendToPlayer(occupantID, messageID, playerID);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	// Handles start of loading/unloading supplies
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_SuppliesLoadingStarted(RplId suppliesID, int playerID, int supplies, bool IsUnloading)
	{
		SCR_CampaignSuppliesComponent suppliesComponent = SCR_CampaignSuppliesComponent.Cast(Replication.FindItem(suppliesID));
		array<IEntity> occupants = {}; //new array<IEntity>;
		
		if(!suppliesComponent)
			return;
		
		//Gettings vehicle entity from cargo
		IEntity vehicleEntity = suppliesComponent.GetOwner();
		
		if(!IsUnloading)
		{
			suppliesComponent.SetSupplyLoadingPlayer(playerID);
			SendToVehicleOccupants(ENotification.SUPPLY_TRUCK_LOADING_PLAYER, vehicleEntity, playerID, supplies);
		}
		else
		{
			suppliesComponent.SetSupplyUnloadingPlayer(playerID);
			SendToVehicleOccupants(ENotification.SUPPLY_TRUCK_UNLOADING_PLAYER, vehicleEntity, playerID, supplies);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	// Handles canceling of loading/unloading supplies
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_SuppliesLoadingCanceled(RplId suppliesID, int playerID, bool IsUnloading)
	{
		SCR_CampaignSuppliesComponent suppliesComponent = SCR_CampaignSuppliesComponent.Cast(Replication.FindItem(suppliesID));
		
		if(!suppliesComponent)
			return;
		
		//Gettings vehicle entity from cargo
		IEntity vehicleEntity = suppliesComponent.GetOwner();
		
		if(!IsUnloading)
		{
			suppliesComponent.DeleteSupplyLoadingPlayer(playerID);
			SendToVehicleOccupants(ENotification.SUPPLY_TRUCK_LOADING_PLAYER_STOPPED, vehicleEntity, playerID);
		}
		else
		{
			suppliesComponent.DeleteSupplyUnloadingPlayer(playerID);
			SendToVehicleOccupants(ENotification.SUPPLY_TRUCK_UNLOADING_PLAYER_STOPPED, vehicleEntity, playerID);
		}
		
	}
	
	//------------------------------------------------------------------------------------------------
	//! Load supplies at a base
	//! \param vehicleID Vehicle entity ID
	//! \param playerID player entity ID
	//! \param baseID base entity ID
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_LoadSupplies(RplId suppliesID, int playerID, RplId baseID, int amount)
	{
		SCR_CampaignSuppliesComponent suppliesComponent = SCR_CampaignSuppliesComponent.Cast(Replication.FindItem(suppliesID));

		if (!suppliesComponent)
			return;
		
		SCR_CampaignBase base = SCR_CampaignBase.Cast(Replication.FindItem(baseID));
		
		if (!base)
			return;
		
		IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerID);
		
		if (!player)
			return;
		
		IEntity box = suppliesComponent.GetOwner();
		
		if (!box)
			return;
		
		if (amount > base.GetSupplies())
			return;
		
		if (suppliesComponent.GetSupplies() == suppliesComponent.GetSuppliesMax())
			return;
		
		Faction playerFaction = SCR_CampaignReconfigureRelayUserAction.GetPlayerFaction(player);
		
		if (!playerFaction || playerFaction != base.GetOwningFaction())
			return;
		
		SCR_CampaignSuppliesComponent baseSuppliesComponent = SCR_CampaignSuppliesComponent.Cast(base.FindComponent(SCR_CampaignSuppliesComponent));
		if (!baseSuppliesComponent)
			return;
		
		float distSq = Math.Pow(baseSuppliesComponent.GetOperationalRadius(), 2);
		vector vehPos = box.GetOrigin();
		
		if (vector.DistanceSq(vehPos, player.GetOrigin()) > 100)
			return;
		
		if (vector.DistanceSq(vehPos, base.GetOrigin()) > distSq)
		{
			SCR_CampaignServiceComponent service = base.GetBaseService(ECampaignServicePointType.SUPPLY_DEPOT);
			if (!service)
				return;
			
			if (vector.DistanceSq(vehPos, service.GetOwner().GetOrigin()) > distSq)
				return;
		}
		
		// Validity check passed, perform action
		int finalAmount = Math.Min(suppliesComponent.GetSuppliesMax() - suppliesComponent.GetSupplies(), amount);
		suppliesComponent.AddSupplies(finalAmount);
		base.AddSupplies(-finalAmount);
		suppliesComponent.SetLastLoadedAt(base);
		Rpc(RpcDo_PlayerFeedbackValueBase, ECampaignClientNotificationID.SUPPLIES_LOADED, (float)finalAmount, base.GetBaseID());
		suppliesComponent.DeleteSupplyLoadingPlayer(playerID);
		
		SendToVehicleOccupants(ENotification.SUPPLY_TRUCK_LOADING_PLAYER_FINISHED, suppliesComponent.GetOwner(), playerID);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_LoadSuppliesStandalone(RplId suppliesID, int playerID, RplId depotID, int amount)
	{
		SCR_CampaignSuppliesComponent suppliesComponent = SCR_CampaignSuppliesComponent.Cast(Replication.FindItem(suppliesID));

		if (!suppliesComponent)
			return;
		
		SCR_CampaignSuppliesComponent depot = SCR_CampaignSuppliesComponent.Cast(Replication.FindItem(depotID));
		
		if (!depot)
			return;
		
		IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerID);
		
		if (!player)
			return;
		
		IEntity box = suppliesComponent.GetOwner();
		
		if (!box)
			return;
		
		if (amount > depot.GetSupplies())
			return;
		
		if (suppliesComponent.GetSupplies() == suppliesComponent.GetSuppliesMax())
			return;
		
		float distSq = Math.Pow(depot.GetOperationalRadius(), 2);
		vector vehPos = box.GetOrigin();
		
		if (vector.DistanceSq(vehPos, depot.GetOwner().GetOrigin()) > distSq || vector.DistanceSq(vehPos, player.GetOrigin()) > 100)
			return;
		
		// Validity check passed, perform action
		int finalAmount = Math.Min(suppliesComponent.GetSuppliesMax() - suppliesComponent.GetSupplies(), amount);
		suppliesComponent.AddSupplies(finalAmount);
		Rpc(RpcDo_PlayerFeedbackValueBase, ECampaignClientNotificationID.SUPPLIES_LOADED, (float)finalAmount, -1);
		suppliesComponent.DeleteSupplyLoadingPlayer(playerID);
		SendToVehicleOccupants(ENotification.SUPPLY_TRUCK_LOADING_PLAYER_FINISHED, suppliesComponent.GetOwner(), playerID);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Unload supplies at a base
	//! \param vehicleID Vehicle entity ID
	//! \param playerID Vehicle entity ID
	//! \param baseID Vehicle entity ID
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_UnloadSupplies(RplId suppliesID, int playerID, RplId baseID, int amount)
	{
		SCR_CampaignSuppliesComponent suppliesComponent = SCR_CampaignSuppliesComponent.Cast(Replication.FindItem(suppliesID));

		if (!suppliesComponent)
			return;
		
		SCR_CampaignBase base = SCR_CampaignBase.Cast(Replication.FindItem(baseID));
		
		if (!base)
			return;
		
		IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerID);
		
		if (!player)
			return;
		
		IEntity box = suppliesComponent.GetOwner();
		
		if (!box)
			return;
		
		if (amount > suppliesComponent.GetSupplies())
			return;
		
		Faction playerFaction = SCR_CampaignReconfigureRelayUserAction.GetPlayerFaction(player);
		SCR_CampaignFaction owningFaction = base.GetOwningFaction();
		
		if (!playerFaction || playerFaction != owningFaction)
			return;
		
		SCR_CampaignSuppliesComponent baseSuppliesComponent = SCR_CampaignSuppliesComponent.Cast(base.FindComponent(SCR_CampaignSuppliesComponent));
		if (!baseSuppliesComponent)
			return;

		float distSq = Math.Pow(baseSuppliesComponent.GetOperationalRadius(), 2);
		vector vehPos = box.GetOrigin();
		
		if (vector.DistanceSq(vehPos, player.GetOrigin()) > 100)
			return;
		
		if (vector.DistanceSq(vehPos, base.GetOrigin()) > distSq)
		{
			SCR_CampaignServiceComponent service = base.GetBaseService(ECampaignServicePointType.SUPPLY_DEPOT);
			if (!service)
				return;
			
			if (vector.DistanceSq(vehPos, service.GetOwner().GetOrigin()) > distSq)
				return;
		}
		
		// Validity check passed, perform action
		int suppliesCur = base.GetSupplies();
		int suppliesMax = base.GetSuppliesMax();
		int suppliesCnt = Math.Min(amount, suppliesMax - suppliesCur);
		float rewardMultiplier = suppliesCnt / suppliesComponent.GetSuppliesMax();
		suppliesComponent.AddSupplies(-suppliesCnt);
		base.AddSupplies(suppliesCnt);
		suppliesComponent.SetLastUnloadedAt(base);
		Rpc(RpcDo_PlayerFeedbackValueBase, ECampaignClientNotificationID.SUPPLIES_UNLOADED, (float)suppliesCnt, base.GetBaseID());
		
		if (RplSession.Mode() != RplMode.Dedicated)
			base.HandleMapInfo();	//Update map info for host
		
		SCR_GameModeCampaignMP camp = SCR_GameModeCampaignMP.GetInstance();
		
		// Award XP unless the truck was just loaded in this base
		// ... or if it was both loaded and unloaded in the previous base
		// (handled in suppliesComponent)
		if (camp && suppliesComponent.AwardXP())
			camp.AwardXP(player, CampaignXPRewards.SUPPLIES_DELIVERED, rewardMultiplier);
		
		SendPlayerMessage(SCR_ERadioMsg.SUPPLIES, base.GetCallsign(), public: false);
		suppliesComponent.DeleteSupplyUnloadingPlayer(playerID);
		
		SendToVehicleOccupants(ENotification.SUPPLY_TRUCK_UNLOADING_PLAYER_FINISHED, suppliesComponent.GetOwner(), playerID);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_CaptureBaseBegin(RplId baseID, int factionIndex, int playerID)
	{
		SCR_CampaignFaction faction = SCR_CampaignFactionManager.GetInstance().GetCampaignFactionByIndex(factionIndex);
		
		if (!faction)
			return;
		
		SCR_CampaignBase base = SCR_CampaignBase.Cast(Replication.FindItem(baseID));
		
		if (!base)
			return;
		
		base.BeginCapture(faction, playerID);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_CaptureBaseEnd(RplId baseID)
	{
		SCR_CampaignBase base = SCR_CampaignBase.Cast(Replication.FindItem(baseID));
		
		if (!base)
			return;
		
		base.EndCapture();
	}
	
	//------------------------------------------------------------------------------------------------
	void SendPlayerMessage(SCR_ERadioMsg msgType, int baseCallsign = SCR_CampaignBase.INVALID_BASE_INDEX, int calledID = SCR_CampaignBase.INVALID_PLAYER_INDEX, bool public = true, int param = SCR_CampaignRadioMsg.INVALID_RADIO_MSG_PARAM, bool checkHQReached = false)
	{
		if (!m_PlayerController)
			return;
		
		SCR_BaseGameModeWasteland campaign = SCR_BaseGameModeWasteland.GetInstance();
		
		if (!campaign)
			return;
		
		SCR_CallsignManagerComponent callsignManager = SCR_CallsignManagerComponent.Cast(campaign.FindComponent(SCR_CallsignManagerComponent));
		
		if (!callsignManager)
			return;
		
		IEntity player = m_PlayerController.GetMainEntity();
		
		if (!player)
			return;
		
		int companyCallsignIndexCaller, platoonCallsignIndexCaller, squadCallsignIndexCaller, characterCallsignIndexCaller;
		
		if (!callsignManager.GetEntityCallsignIndexes(player, companyCallsignIndexCaller, platoonCallsignIndexCaller, squadCallsignIndexCaller, characterCallsignIndexCaller))
    		return;
		
		SCR_GadgetManagerComponent gadgetMan = SCR_GadgetManagerComponent.Cast(player.FindComponent(SCR_GadgetManagerComponent));
		
		if (!gadgetMan)
			return;
		
		IEntity radioEnt = gadgetMan.GetGadgetByType(EGadgetType.RADIO);
		
		if (!radioEnt)
			return;
		
		BaseRadioComponent radio = BaseRadioComponent.Cast(radioEnt.FindComponent(BaseRadioComponent));
		
		if (!radio)
			return;
		
		IEntity called = GetGame().GetPlayerManager().GetPlayerControlledEntity(calledID);
		
		SCR_CampaignRadioMsg msg = new SCR_CampaignRadioMsg;
		msg.SetRadioMsg(msgType);
		msg.SetBaseCallsign(baseCallsign);
		msg.SetCallerCallsign(companyCallsignIndexCaller, platoonCallsignIndexCaller, squadCallsignIndexCaller);
		msg.SetIsPublic(public);
		msg.SetParam(param);
		msg.SetPlayerID(m_PlayerController.GetPlayerId());
		
		int companyCallsignIndexCalled, platoonCallsignIndexCalled, squadCallsignIndexCalled, characterCallsignIndexCalled;
		
		if (called && callsignManager.GetEntityCallsignIndexes(called, companyCallsignIndexCalled, platoonCallsignIndexCalled, squadCallsignIndexCalled, characterCallsignIndexCalled))
			msg.SetCalledCallsign(companyCallsignIndexCalled, platoonCallsignIndexCalled, squadCallsignIndexCalled);
		
		Rpc(RpcDo_PlayRadioMsg, msgType, baseCallsign, CompressCallsign(companyCallsignIndexCaller, platoonCallsignIndexCaller, squadCallsignIndexCaller), CompressCallsign(companyCallsignIndexCalled, platoonCallsignIndexCalled, squadCallsignIndexCalled), param, msg.GetSeed(), 1, checkHQReached);
		
		radio.Transmit(msg);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetLastHQRadioMessageTimestamp(float time)
	{
		m_fLastHQRadioMessageTimestamp = time;
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	protected int CompressCallsign(int company, int platoon, int squad)
	{
		return (company * 10000) + (platoon * 100) + squad;
	}
	
	//------------------------------------------------------------------------------------------------
	protected void DecompressCallsign(int callsign, out int company, out int platoon, out int squad)
	{
		company = Math.Floor(callsign / 10000);
		callsign = callsign - (company * 10000);
		
		platoon = Math.Floor(callsign / 100);
		callsign = callsign - (platoon * 100);
		
		squad = callsign;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Capture a base (change its owner)
	//! \param baseID Base entity ID
	//! \param factionIndex Index of new faction to own the base
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_CaptureBase(RplId baseID, int factionIndex, int playerID)
	{
		SCR_CampaignFaction faction = SCR_CampaignFactionManager.GetInstance().GetCampaignFactionByIndex(factionIndex);
		
		if (!faction)
			return;
		
		SCR_CampaignBase base = SCR_CampaignBase.Cast(Replication.FindItem(baseID));
		
		if (!base)
			return;
		
		if (base.BeginCapture(faction, playerID))
			base.FinishCapture(playerID);
		
		if (base.GetType() == CampaignBaseType.RELAY && playerID != SCR_CampaignBase.INVALID_PLAYER_INDEX)
			SendPlayerMessage(SCR_ERadioMsg.RELAY);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Show notification about request result to the requester
	//! \param msgID Message ID (see ECampaignClientNotificationID)
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RpcDo_PlayerFeedback(int msgID)
	{
		RpcDo_PlayerFeedbackImpl(msgID);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Show notification about request result to the requester
	//! \param msgID Message ID (see ECampaignClientNotificationID)
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RpcDo_PlayerFeedbackValueBase(int msgID, float value, int baseID)
	{
		RpcDo_PlayerFeedbackImpl(msgID, value, -1, baseID);
	}

	//------------------------------------------------------------------------------------------------
	//! Show notification about request result to the requester
	//! \param msgID Message ID (see ECampaignClientNotificationID)
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RpcDo_PlayerFeedbackBase(int msgID, int baseID)
	{
		RpcDo_PlayerFeedbackImpl(msgID, 0, -1, baseID);
	}

	//------------------------------------------------------------------------------------------------
	//! Show notification about request result to the requester
	//! \param msgID Message ID (see ECampaignClientNotificationID)
	protected void RpcDo_PlayerFeedbackImpl(int msgID, float value = 0, int assetID = -1, int baseID = -1, int sourceBaseID = -1)
	{
		LocalizedString msg;
		LocalizedString msg2;
		int duration = 2;
		int prio = -1;
		string msg1param1;
		string msg2param1;
		string msg2param2;
		SCR_CampaignBase base = SCR_CampaignBaseManager.FindBaseByID(baseID);
		SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
		
		if (!campaign)
			return;
		
		switch (msgID)
		{	
			case ECampaignClientNotificationID.SUPPLIES_LOADED:
			{
				msg = "#AR-Campaign_SuppliesLoaded-UC";
				duration = 6;
				prio = SCR_ECampaignPopupPriority.SUPPLIES_HANDLED;
				msg1param1 = value.ToString();
				
				if (base)
				{
					msg2 = "#AR-Campaign_SuppliesAmountInfo-UC";
					msg2param1 = base.GetBaseName();
					msg2param2 = base.GetSupplies().ToString();
				}
				
				SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_LOADSUPPLIES);
				break;
			};
			case ECampaignClientNotificationID.SUPPLIES_UNLOADED:
			{
				msg = "#AR-Campaign_SuppliesUnloaded-UC";
				duration = 6;
				prio = SCR_ECampaignPopupPriority.SUPPLIES_HANDLED;
				msg1param1 = value.ToString();
				
				if (base)
				{
					msg2 = "#AR-Campaign_SuppliesAmountInfo-UC";
					msg2param1 = base.GetBaseName();
					msg2param2 = base.GetSupplies().ToString();
				}
				
				if (!campaign.IsTutorial())
					SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_Building_Text", "#AR-Campaign_Hint_Building_Title", 20, fieldManualEntry: EFieldManualEntryId.CONFLICT_BUILDING);
				
				SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_UNLOADSUPPLIES);
				break;
			};
			case ECampaignClientNotificationID.RESPAWN:
			{
				campaign.SetIsPlayerInRadioRange(true);
				GetGame().GetCallqueue().CallLater(campaign.ShowHint, 16500, false, SCR_ECampaignHints.SIGNAL);
				
				if (!base)
					return;

				msg = base.GetBaseNameUpperCase();
				TimeAndWeatherManagerEntity timeManager = GetGame().GetTimeAndWeatherManager();
				
				if (timeManager)
				{
					int hours;
					int minutes;
					int seconds;
					timeManager.GetHoursMinutesSeconds(hours, minutes, seconds);
					string strHours = hours.ToString();
					
					if (hours > 0 && hours < 10)
						strHours = "0" + strHours;
					
					string strMinutes = minutes.ToString();
					
					if (minutes < 10)
						strMinutes = "0" + strMinutes;
					
					msg = string.Format("%1, %2:%3", msg, strHours, strMinutes);
				}
				
				msg2 = SCR_BaseTask.TASK_HINT_TEXT;
				msg2param1 = SCR_PopUpNotification.TASKS_KEY_IMAGE_FORMAT;
				duration = 5;
				prio = SCR_ECampaignPopupPriority.RESPAWN;
				
				if (m_bFirstSpawn)
				{
					m_bFirstSpawn = false;
					duration = 120;
				}
				
				break;
			}; 			
			default: {return;};
		}
		
		SCR_PopUpNotification.GetInstance().PopupMsg(msg, duration, msg2, param1: msg1param1, text2param1: msg2param1, text2param2: msg2param2);
	}
	
	//------------------------------------------------------------------------------------------------
	void SendVehicleSpawnHint(int hintID)
	{
		Rpc(RpcDo_VehicleSpawnHint, hintID);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RpcDo_VehicleSpawnHint(int hintID)
	{
		SCR_BaseGameModeWasteland campaign = SCR_BaseGameModeWasteland.GetInstance();
		
		if (!campaign)
			return;
		
		campaign.ShowHint(hintID);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void RpcDo_OnPlayerXPChanged(int currentXP, int XPToAdd, bool volunteer, CampaignXPRewards rewardID, bool profileUsed, int skillLevel)
	{
		SCR_BaseGameModeWasteland campaign = SCR_BaseGameModeWasteland.GetInstance();
		
		if (!campaign)
			return;
		
		campaign.ShowXPInfo(currentXP, rewardID, XPToAdd, volunteer, profileUsed, skillLevel);

		if (!m_bXPInfoOnKillsHintShown)
		{
			m_bXPInfoOnKillsHintShown = true;
			GetGame().GetCallqueue().CallLater(XPInfoOnKillsHint, 30000 + Math.RandomIntInclusive(0, 30000));
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void RespawnLocationPopup(int baseID)
	{
		Rpc(RpcDo_PlayerFeedbackBase, ECampaignClientNotificationID.RESPAWN, baseID);
	}
	
	//------------------------------------------------------------------------------------------------
	void EnableShowingSpawnPosition(bool enable)
	{
			Rpc(RpcDo_PlayerEnableShowingSpawnPosition, enable);	
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void RpcDo_PlayerEnableShowingSpawnPosition(bool enable)
	{
		// Adds function to queue. Sometimes, game is not fast enough to spawn player entity
		//GetGame().GetCallqueue().CallLater(SCR_GameModeCampaignMP.GetInstance().EnablePlayerSpawnHint, 100, true, enable);
	}
	
	//------------------------------------------------------------------------------------------------
	protected void HandleRadioRespawnTimer(SCR_SpawnPoint selectedSpawnPoint)
	{
		SCR_PlayerRadioSpawnPointCampaign spawnpoint = SCR_PlayerRadioSpawnPointCampaign.Cast(selectedSpawnPoint);
		
		if (!spawnpoint)
			return;
		
		RplId id = Replication.FindId(selectedSpawnPoint);
		
		if (id == RplId.Invalid())
			return;
		
		Rpc(RpcAsk_HandleRadioRespawnTimer, id);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_HandleRadioRespawnTimer(RplId selectedSpawnPointId)
	{
		if (!m_PlayerController)
			return;
		
		SCR_PlayerRadioSpawnPointCampaign spawnpoint = SCR_PlayerRadioSpawnPointCampaign.Cast(Replication.FindItem(selectedSpawnPointId));
		
		if (!spawnpoint)
			return;
		
		IEntity operator = GetGame().GetPlayerManager().GetPlayerControlledEntity(spawnpoint.GetPlayerID());
		
		if (!operator)
			return;
		
		SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
		
		if (!campaign)
			return;
		
		SCR_CampaignFactionManager fManager = SCR_CampaignFactionManager.GetInstance();
		
		if (!fManager)
			return;
		
		SCR_TimedSpawnPointComponent timer = SCR_TimedSpawnPointComponent.Cast(campaign.FindComponent(SCR_TimedSpawnPointComponent));
		
		if (!timer)	
			return;
		
		timer.SetRespawnTime(m_PlayerController.GetPlayerId(), fManager.GetRankRadioRespawnCooldown(SCR_CharacterRankComponent.GetCharacterRank(operator)));
	}
	
	//------------------------------------------------------------------------------------------------
	void PlayRadioMsg(SCR_ERadioMsg msg, int baseCallsign, int callerCallsignCompany, int callerCallsignPlatoon, int callerCallsignSquad, int calledCallsignCompany, int calledCallsignPlatoon, int calledCallsignSquad, bool isPublic, int param, float seed, float quality, int playerID)
	{
		SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
		
		if (!campaign)
			return;
		
		SCR_CallsignManagerComponent callsignManager = SCR_CallsignManagerComponent.Cast(campaign.FindComponent(SCR_CallsignManagerComponent));
		
		if (!callsignManager)
			return;
		
		int playerCallsignCompany, playerCallsignPlatoon, playerCallsignSquad, playerCallsignCharacter;
		callsignManager.GetEntityCallsignIndexes(m_PlayerController.GetMainEntity(), playerCallsignCompany, playerCallsignPlatoon, playerCallsignSquad, playerCallsignCharacter);
		
		if (isPublic || playerID == m_PlayerController.GetPlayerId())
			Rpc(RpcDo_PlayRadioMsg, msg, baseCallsign, CompressCallsign(callerCallsignCompany, callerCallsignPlatoon, callerCallsignSquad), CompressCallsign(calledCallsignCompany, calledCallsignPlatoon, calledCallsignSquad), param, seed, quality);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void RpcDo_PlayRadioMsg(SCR_ERadioMsg msg, int baseCallsign, int callerCallsign, int calledCallsign, int param, float seed, float quality, bool checkHQReached)
	{
		SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
		
		int callerCallsignCompany, callerCallsignPlatoon, callerCallsignSquad, calledCallsignCompany, calledCallsignPlatoon, calledCallsignSquad;
		DecompressCallsign(callerCallsign, callerCallsignCompany, callerCallsignPlatoon, callerCallsignSquad);
		DecompressCallsign(calledCallsign, calledCallsignCompany, calledCallsignPlatoon, calledCallsignSquad);
		
		if (campaign)
			campaign.PlayRadioMsg(msg, baseCallsign, callerCallsignCompany, callerCallsignPlatoon, callerCallsignSquad, calledCallsignCompany, calledCallsignPlatoon, calledCallsignSquad, param, seed, quality);
		
		if (checkHQReached)
			GetGame().GetCallqueue().CallLater(CheckHQReached, 7000)
	}
		
	//------------------------------------------------------------------------------------------------
	void CheckHQReached()
	{
		if (m_fLastHQRadioMessageTimestamp < (Replication.Time() - 8000))
			SCR_HintManagerComponent.ShowCustomHint("#AR-Campaign_Hint_OutOfRadioRange_Text", "#AR-Campaign_Hint_OutOfRadioRange_Title", 20);
	}
		
	//------------------------------------------------------------------------------------------------
	// Init
	override void EOnInit(IEntity owner)
	{
		m_PlayerController = SCR_PlayerController.Cast(PlayerController.Cast(owner));
		
		if (!m_PlayerController)
		{
			Print("SCR_CampaignNetworkComponent must be attached to PlayerController!", LogLevel.ERROR);
			return;
		}
		
		m_RplComponent = RplComponent.Cast(owner.FindComponent(RplComponent));
		
		if (m_PlayerController.GetPlayerId() == SCR_PlayerController.GetLocalPlayerId())
			SCR_SelectSpawnPointSubMenu.Event_OnSpawnPointChanged.Insert(HandleRadioRespawnTimer);
	}
	
	//------------------------------------------------------------------------------------------------
	// PostInit
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);
		owner.SetFlags(EntityFlags.ACTIVE, true);
	}
		
	//------------------------------------------------------------------------------------------------
	// Constructor
	void SCR_CampaignNetworkComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
	{
	}

	//------------------------------------------------------------------------------------------------
	// Destructor
	void ~SCR_CampaignNetworkComponent()
	{
		SCR_SelectSpawnPointSubMenu.Event_OnSpawnPointChanged.Remove(HandleRadioRespawnTimer);
	}
};

