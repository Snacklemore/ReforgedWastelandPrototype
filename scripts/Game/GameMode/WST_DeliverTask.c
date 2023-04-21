//------------------------------------------------------------------------------------------------
class WST_DeliverTaskClass: SCR_EditorTaskClass
{
};


//inital task position: position of Pickup 
//second task position: position delivery
//------------------------------------------------------------------------------------------------
class WST_DeliverTask : SCR_EditorTask
{

	static const string CAMPAIGN_TASK_RECONFIGURED_TEXT = "#AR-CampaignTasks_Reconfigured-UC";
	static const string CAMPAIGN_TASK_RECONFIGURED_BY_TEXT = "#AR-CampaignTasks_ReconfiguredBy-UC";
	
	//*****************//
	//MEMBER ATTRIBUTES//
	//*****************//
	
	[Attribute(defvalue: "Task title for reconfigure.", desc: "The task title visible to the player.")]
	protected string m_sNameReconfigure;
	[Attribute(defvalue: "Task description.", desc: "The task description visible to the player.")]
	protected string m_sDescriptionReconfigure;
	bool isItemTypePickedUp = false;
	
	//**************************//
	//PROTECTED MEMBER VARIABLES//
	//**************************//
	
	protected SCR_CampaignTaskType m_eType;
	protected vector m_destination;
	
	//*********************************//
	//PROTECTED OVERRIDE MEMBER METHODS//
	//*********************************//
	
	//------------------------------------------------------------------------------------------------
	protected override bool DoneByAssignee()
	{
		/*
		SCR_BaseTaskExecutor assignee = GetAssignee();
		if (assignee && m_TargetBase)
		{
			int assigneeID = SCR_BaseTaskExecutor.GetTaskExecutorID(assignee);
			int reconfigurerID = m_TargetBase.GetReconfiguredByID();
			if (assigneeID == reconfigurerID)
				return true;
		}
		
		return false;
		*/
	}
	//------------------------------------------------------------------------------------------------
	
	void SetTargetDestination(vector v)
	{
		
			
		SetOrigin(v);
		m_destination = v;
		CreateMapUIIcon();
		UpdateMapInfo();
		SetHUDIcon();
	}
	
	//------------------------------------------------------------------------------------------------
	//! Shows a message about this task being available again
	protected override void ShowAvailableTask(bool afterAssigneeRemoved = false)
	{
		/*
		SCR_ECannotAssignReasons reason;
		if (CanBeAssigned(reason))
		{
			SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
			if (campaign)
			{
				string text = TASK_AVAILABLE_TEXT + " " + GetTitle();
				string baseName;
				
				if (m_TargetBase)
					baseName = GetBaseNameWithCallsign();
				
				SCR_PopUpNotification.GetInstance().PopupMsg(text, prio: SCR_ECampaignPopupPriority.TASK_AVAILABLE, param1: baseName, text2: TASK_HINT_TEXT, text2param1: SCR_PopUpNotification.TASKS_KEY_IMAGE_FORMAT, sound: SCR_SoundEvent.TASK_CREATED);
			}
		}*/
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void ShowTaskProgress(bool showMsg = true)
	{
		/*SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
		if (campaign && showMsg)
		{
			string baseName;
			
			if (m_TargetBase)
				baseName = GetBaseNameWithCallsign();
			
			SCR_PopUpNotification.GetInstance().PopupMsg(TASK_PROGRESS_TEXT + " " + GetTitle() + " " + TASK_AMOUNT_COMPLETED_TEXT, prio: SCR_ECampaignPopupPriority.TASK_PROGRESS, param1: baseName);
		}*/
	}
	
	//******************************//
	//PUBLIC OVERRIDE MEMBER METHODS//
	//******************************//
	
	//------------------------------------------------------------------------------------------------
	override string GetIconSuffix()
	{
		//if (m_TargetBase && m_TargetBase.GetType() == CampaignBaseType.RELAY)
			//return "_Relay";
		
		return "";
	}
	
	//------------------------------------------------------------------------------------------------
	override void DoNotifyAssignment(int assigneeID)
	{
		SCR_CampaignNetworkComponent assigneeNetworkComponent = SCR_CampaignNetworkComponent.GetCampaignNetworkComponent(assigneeID);
		if (!assigneeNetworkComponent)
			return;
		
		//assigneeNetworkComponent.SendPlayerMessage(GetAssignMessage(), m_TargetBase.GetCallsign());
	}
	
	//------------------------------------------------------------------------------------------------
	//! Return the title of this task.
	override string GetTitle()
	{
		//if (m_TargetBase && m_TargetBase.GetType() == CampaignBaseType.RELAY)
			//return m_sNameReconfigure;
		
		return m_sName;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Return the description of this task.
	override string GetDescription()
	{
		//if (m_TargetBase && m_TargetBase.GetType() == CampaignBaseType.RELAY)
			//return m_sDescriptionReconfigure;
		
		return m_sDescription;
	}
	//put this into the onItemAdded Invoker of invmanager
	void OnDeliveryItemPickup(IEntity ie, BaseInventoryStorageComponent storage)
	{	//get the storage owner 
		BaseInventoryStorageComponent castStorage = BaseInventoryStorageComponent.Cast(storage);
		IEntity iStorage = castStorage.GetOwner();
		RplComponent rpl = RplComponent.Cast(iStorage.FindComponent(RplComponent));
		RplId r_id = rpl.Id();
		Print("DeliveryMission::ItemPickUp");
		//SCR_ChimeraCharacter parent = storageOwner.GetParent();
		int p_id = GetGame().GetPlayerManager().GetPlayerIdFromEntityRplId(r_id);
		WeaponComponent wpc = WeaponComponent.Cast(ie.FindComponent(WeaponComponent));
		if (wpc)
		{
			isItemTypePickedUp = true;
			Print("DeliveryMission::DeliveryItemTypePickedUp");

		}
		
		
		
		Print("DeliveryMission::PlayerID: " + p_id);

		int taskID = GetTaskID();
		
		Print("DeliveryMission::TaskID: " + taskID);

		
	}
	
	
	override void RemoveAssignee(SCR_BaseTaskExecutor assignee, SCR_EUnassignReason reason)
	{
		if (!m_aAssignees || !assignee)
			return;
		
		int index = m_aAssignees.Find(assignee);
		
		if (index == -1)
			return;
		
		m_aAssignees.Remove(index);
		if (assignee == SCR_BaseTaskExecutor.GetLocalExecutor())
			ShowPopUpNotification("#AR-Tasks_UnassignPopup");

		OnAssigneeRemoved(assignee);
		
		if (m_bIndividualTask)
		{
			switch (reason)
			{
				case SCR_EUnassignReason.ASSIGNEE_TIMEOUT:
					// We set timeout for the assignee, since we don't want them to be immediately able to assign the task to themselves again
					m_fAssigneeTimeoutTimestamp = GetTaskManager().GetTimestamp() + DEFAULT_ASSIGNEE_TIMEOUT_TIME;
					m_TimedOutAssignee = assignee;
					break;
				case SCR_EUnassignReason.ASSIGNEE_DISCONNECT:
				{
					GetTaskManager().AddAssigneeAbandoned(assignee);
					break;
				}
					
				case SCR_EUnassignReason.ASSIGNEE_ABANDON:
					break;
				case SCR_EUnassignReason.GM_REASSIGN:
					break;
			}
			
			ShowAvailableTask(true);
		}
		
		SCR_BaseTaskManager.s_OnTaskUnassigned.Invoke(this);
		
		RegisterTaskUpdate(SCR_ETaskEventMask.TASK_ASSIGNEE_CHANGED);
	}
	
	
	//called on server and clients(broadcast)
	protected override void OnAssigneeAdded(SCR_BaseTaskExecutor newAssignee)
	{
		if (newAssignee)
			newAssignee.AssignNewTask(this);
		
		UpdateMapTaskIcon();
		UpdateTaskListAssignee();		
		
		
		Print("WST_DeliverTask::Added Assignee");

		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
		if(!Replication.IsServer())
			return;
		
		Print("WST_DeliverTask::OnAssigneeAdded");
		
		//attach onItemAddedInvoker to StorageManagerComponent
		array<SCR_BaseTaskExecutor> assignees = {};
		GetAssignees(assignees);
		if(assignees.IsEmpty())
			return;
		
		
		
		int assignees_count = assignees.Count();
		Print("WST_DeliverTask::Assignees Count: " + assignees_count);

		int playerID = SCR_BaseTaskExecutor.GetTaskExecutorID(assignees[0]); 
		//check for playerID in m_playerIdSetItemAddedCallback
		
		
		PlayerManager pm = GetGame().GetPlayerManager();
		PlayerController pc = pm.GetPlayerController(playerID);
		
		IEntity controlled = pc.GetControlledEntity();
		if (controlled)
			Print("WST_DeliverTask::OnAssignedHook controlled");

		SCR_InventoryStorageManagerComponent imc = SCR_InventoryStorageManagerComponent.Cast(controlled.FindComponent(SCR_InventoryStorageManagerComponent));
		if (imc)
			Print("WST_DeliverTask::OnAssignedHook IMC found");
		SCR_CharacterInventoryStorageComponent cis = SCR_CharacterInventoryStorageComponent.Cast(controlled.FindComponent(SCR_CharacterInventoryStorageComponent));
		if (cis)
			Print("WST_DeliverTask::OnAssignedHook CIS found");

		SetInvokerCallBack(imc);
		
		
		BaseInventoryStorageComponent weaponStorage = cis.GetWeaponStorage();
		array<IEntity> outarray = {};
		weaponStorage.GetAll(outarray);
		Print("WST_DeliverTask::WeaponStorageCount"+ outarray.Count());

		if (!outarray.IsEmpty())
			isItemTypePickedUp = true;

		
	}
	protected override void OnAssigneeRemoved(SCR_BaseTaskExecutor oldAssignee)
	{
		
		if (oldAssignee)
			oldAssignee.AssignNewTask(null);
		
		

		
		if (m_aAssignees.Count() <= 0)
			SetState(SCR_TaskState.OPENED);
		
		UpdateMapTaskIcon();
		UpdateTaskListAssignee();
		
		Print("DeliveryMission::Removed Assignee");
		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
		if(!Replication.IsServer())
			return;
		//remove on item added invoke (used to detect items for delivery mission)
		IEntity controlled = oldAssignee.GetControlledEntity();
		
		if (controlled)
					Print("DeliveryMission::Found controlled, proceeding to remove Invoke");
		else
			return;

		SCR_InventoryStorageManagerComponent imc = SCR_InventoryStorageManagerComponent.Cast(controlled.FindComponent(SCR_InventoryStorageManagerComponent));
		if(imc)
					Print("DeliveryMission::Found IMC, proceeding to remove Invoke");

		imc.m_OnItemAddedInvoker.Remove(OnDeliveryItemPickup);
		
		
		
		
		Print("DeliveryMission::Probably removed Invoke...huh");
	}
	void SetInvokerCallBack(SCR_InventoryStorageManagerComponent mc)
	{
		mc.m_OnItemAddedInvoker.Insert(OnDeliveryItemPickup);
		//mc.m_OnItemRemovedInvoker.Insert(OnDeliveryItemRemove);
	}
	//server
	void CheckPeriodic()
	{
		
		SCR_BaseTaskSupportEntity supportEntity = SCR_BaseTaskSupportEntity.Cast(GetTaskManager().FindSupportEntity(SCR_BaseTaskSupportEntity));
		if (!supportEntity)
			return;
		
		array<SCR_BaseTaskExecutor> assignees = new array<SCR_BaseTaskExecutor>();
		GetAssignees(assignees);
		vector playerPos;
		IEntity controlledEntity;
		for (int x = assignees.Count() - 1; x >= 0; x--)
		{

			int id = SCR_BaseTaskExecutor.GetTaskExecutorID(assignees[x]);
			
			
			
			controlledEntity = SCR_PossessingManagerComponent.GetPlayerMainEntity(id);
			if (!controlledEntity)
				continue;
			playerPos = controlledEntity.GetOrigin();
			
		}
		
		
		
		float dist = vector.DistanceSq(m_destination,playerPos);
		if (dist < 256.0)
		{
			//check if item is present 
			//this value is set once and then cached on item pickup to save performance
			if(isItemTypePickedUp)
			{
				
				//get wallet and add cash
				
				//crashing as soon as there is an attachment on the weapon on dedicated server
				SCR_BaseTaskManager tm = GetTaskManager();
				WST_DeliverTaskSupportEntity support = tm.FindSupportEntity(WST_DeliverTaskSupportEntity);
				if (support)
						Print("DeliverTask::Support Entity Found");

				SCR_InventoryStorageManagerComponent imc = SCR_InventoryStorageManagerComponent.Cast(controlledEntity.FindComponent(SCR_InventoryStorageManagerComponent));
				SCR_CharacterInventoryStorageComponent cis = SCR_CharacterInventoryStorageComponent.Cast(controlledEntity.FindComponent(SCR_CharacterInventoryStorageComponent));
				BaseInventoryStorageComponent weaponStorage = cis.GetWeaponStorage();
				array<IEntity> outarray = {};
				weaponStorage.GetAll(outarray);
				IEntity weaponEquiped;

				if (!outarray.IsEmpty())
				{
					Print("DeliverTask::weaponsArrayCount: "+ outarray.Count());
					//Deleting weapon that is currently held is a CTD!!
					array<Managed> slots = {};
					foreach(IEntity e: outarray)
					{
						if (cis.GetItemType(e) == EWeaponType.WT_RIFLE)
						{
							weaponEquiped = e;
							Print("DeliverTask::weapon Found!");

							
							
						}
					
					}
					
					
					IEntity currentItem = cis.GetCurrentItem();
					if(currentItem)
					{
						//something held in hands, check if weapon 
						WeaponComponent wc = WeaponComponent.Cast(currentItem.FindComponent(WeaponComponent));
						if (wc)
						{
							//we are holding a weapon 
						//check if weapon is currently held 
							if(weaponEquiped)
							{
								if(currentItem == weaponEquiped)
								{
									support.PopUpNotification(this,"Cant turn in weapon when held in hands!");
									//ShowPopUpNotification();
									Print("DeliverTask::Cant delete if held!");
									return;

									
								}
							}
							
									
						}
					}

				}
				WST_WalletPredicate predicate = new WST_WalletPredicate();
				IEntity wallet = imc.FindItem(predicate);
				
				if (wallet)
				{
					MoneyComponent mc = MoneyComponent.Cast(wallet.FindComponent(MoneyComponent));
					if (mc)
					{
						
									
						if(weaponEquiped)
						{
							//dark magic so it doesnt crash, dont ask plz
							while(cis.IsLocked())
							{
								int i;
							}
							RplComponent.DeleteRplEntity(weaponEquiped,false);						

						}else
						{
							//no delivery item return without finish
							return;
						}
						int newValue;
						newValue = 5000 + mc.GetValue();
						mc.SetValue(newValue);
						supportEntity.FinishTask(this);
						SCR_BaseTaskManager.s_OnPeriodicalCheck2Second.Remove(CheckPeriodic);

						return;
					}

				}
				
				//OR if no wallet in inventory spawn wallet with 5000 at m_destination
				EntitySpawnParams param = new EntitySpawnParams();
				param.Transform[3] = m_destination;
				ResourceName n = "{50E496D7030957C1}Prefabs/Props/Commercial/cash/cash.et";
				wallet = GetGame().SpawnEntityPrefab(Resource.Load(n),GetGame().GetWorld(),param);
				MoneyComponent mc = MoneyComponent.Cast(wallet.FindComponent(MoneyComponent));
				mc.SetValue(5000);
				SCR_BaseTaskManager.s_OnPeriodicalCheck2Second.Remove(CheckPeriodic);

				supportEntity.FinishTask(this);

			}
		
		}
			
	}
	//------------------------------------------------------------------------------------------------
	//server and client per broadcast
	override void Finish(bool showMsg = true)
	{
		showMsg = SCR_RespawnSystemComponent.GetLocalPlayerFaction() == m_TargetFaction;
		super.Finish(showMsg);
		
		
		
		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''//
		if(!Replication.IsServer())
			return;
		//get assignee (IsIndividual task so first assignee is the only assignee)
		SCR_BaseTaskExecutor assignee = GetAssignee();
		int id = SCR_BaseTaskExecutor.GetTaskExecutorID(assignee);
		PlayerManager pm = GetGame().GetPlayerManager();
		
		
		IEntity controlled = pm.GetPlayerControlledEntity(id);
		
		if (controlled)
				Print("DeliveryMission::Found controlled on Finish!");
		else
			return;

		SCR_InventoryStorageManagerComponent imc = SCR_InventoryStorageManagerComponent.Cast(controlled.FindComponent(SCR_InventoryStorageManagerComponent));
		if(imc)
				Print("DeliveryMission::Found IMC, on Finish!");

		imc.m_OnItemAddedInvoker.Remove(OnDeliveryItemPickup);
		
		
		/*
		SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
		
		// Reward XP for reconfiguring a relay
		if (!GetTaskManager().IsProxy() && GetType() == SCR_CampaignTaskType.CAPTURE && m_TargetBase.GetType() == CampaignBaseType.RELAY)
		{
			int playerID = m_TargetBase.GetReconfiguredByID();
			PlayerController player = GetGame().GetPlayerManager().GetPlayerController(playerID);
			campaign.AwardXP(player, CampaignXPRewards.RELAY_RECONFIGURED, 1.0, DoneByAssignee());
		};
		
		string baseName;
		
		if (m_TargetBase)
			baseName = GetBaseNameWithCallsign();
		
		if (campaign && showMsg)
		{
			// TODO make this nicer
			if (m_bIndividualTask)
			{
				if (IsAssignedToLocalPlayer())
				{
					if (DoneByAssignee())
					{
						SCR_PopUpNotification.GetInstance().PopupMsg(TASK_COMPLETED_TEXT + " " + GetTitle(), prio: SCR_ECampaignPopupPriority.TASK_DONE, param1: baseName, sound: SCR_SoundEvent.TASK_SUCCEED, text2: SCR_BaseTask.TASK_HINT_TEXT, text2param1: SCR_PopUpNotification.TASKS_KEY_IMAGE_FORMAT);
					}
					else
					{
						SCR_PopUpNotification.GetInstance().PopupMsg(TASK_FAILED_TEXT + " " + GetTitle(), prio: SCR_ECampaignPopupPriority.TASK_DONE, param1: baseName, sound: SCR_SoundEvent.TASK_FAILED);
					}
				}
				else
				{
					if (m_aAssignees && m_aAssignees.Count() > 0)
					{
						string text;
						string par1;
						string par2;
						string par3;
						string par4;
						string par5;
						GetFinishTextData(text, par1, par2, par3, par4, par5);
						SCR_PopUpNotification.GetInstance().PopupMsg(text, prio: SCR_ECampaignPopupPriority.TASK_DONE, param1: par1, param2: par2, param3: par3, param4: par4, sound: SCR_SoundEvent.TASK_SUCCEED);
					}
					else
						SCR_PopUpNotification.GetInstance().PopupMsg(TASK_COMPLETED_TEXT + " " + GetTitle(), prio: SCR_ECampaignPopupPriority.TASK_DONE, param1: baseName, sound: SCR_SoundEvent.TASK_SUCCEED, text2: SCR_BaseTask.TASK_HINT_TEXT, text2param1: SCR_PopUpNotification.TASKS_KEY_IMAGE_FORMAT);
				}
			}
			else
				SCR_PopUpNotification.GetInstance().PopupMsg(TASK_COMPLETED_TEXT + " " + GetTitle(), prio: SCR_ECampaignPopupPriority.TASK_DONE, param1: baseName, sound: SCR_SoundEvent.TASK_SUCCEED, text2: SCR_BaseTask.TASK_HINT_TEXT, text2param1: SCR_PopUpNotification.TASKS_KEY_IMAGE_FORMAT);
		}*/
	}
	
	//------------------------------------------------------------------------------------------------
	//! Fails the task.
	override void Fail(bool showMsg = true)
	{
		showMsg = SCR_RespawnSystemComponent.GetLocalPlayerFaction() == m_TargetFaction;
		super.Fail(showMsg);
		
		/*SCR_GameModeCampaignMP campaign = SCR_GameModeCampaignMP.GetInstance();
		if (campaign && showMsg)
		{
			string baseName;
		
			if (m_TargetBase)
				baseName = GetBaseNameWithCallsign();
			
			SCR_PopUpNotification.GetInstance().PopupMsg(TASK_FAILED_TEXT + " " + GetTitle(), prio: SCR_ECampaignPopupPriority.TASK_DONE, param1: baseName, sound: SCR_SoundEvent.TASK_FAILED);
		}*/
	}
	 void PopUpNotification(string subtitle,int taskExecutorId)
	{
		
		SCR_PlayerController pc = GetGame().GetPlayerController();
		int playerID = -1;
		if (pc)
			playerID = pc.GetPlayerId();
		
		
		
		if (playerID == taskExecutorId)
		{
			SCR_PopUpNotification.GetInstance().PopupMsg(GetTitleText(), text2: subtitle);

		}
		
	}
	//------------------------------------------------------------------------------------------------
	//! Returns a text informing about the task being finished
	override string GetFinishText()
	{
		/*
		// TODO Add other types of tasks here once needed...
		string text = "";
		
		switch (m_eType)
		{
			case SCR_CampaignTaskType.CAPTURE:
			{
				if (m_TargetBase)
					if (m_bIndividualTask && DoneByAssignee())
						text = string.Format(CAMPAIGN_TASK_RECONFIGURED_BY_TEXT, GetBaseNameWithCallsign(), GetAllAssigneeNamesString());
					else
						text = string.Format(CAMPAIGN_TASK_RECONFIGURED_TEXT, GetBaseNameWithCallsign());
				
				break;
			};
		}
			
		return text;
		*/
	}

	//------------------------------------------------------------------------------------------------
	//! Returns text data informing about the task being finished
	void GetFinishTextData(out string text = "", out string param1 = "", out string param2 = "", out string param3 = "", out string param4 = "", out string param5 = "")
	{
		/*
		switch (m_eType)
		{
			case SCR_CampaignTaskType.CAPTURE:
			{
				if (m_TargetBase)
					if (m_bIndividualTask && DoneByAssignee())
					{
						text = CAMPAIGN_TASK_RECONFIGURED_BY_TEXT;
						param1 = GetBaseNameWithCallsign();
						param2 = GetAllAssigneeNamesString();
					}
					else
					{
						text = CAMPAIGN_TASK_RECONFIGURED_TEXT;
						param1 = GetBaseNameWithCallsign();
					}

				break;
			};
		}
		*/
	}
	
	//------------------------------------------------------------------------------------------------
	override string GetMapDescriptorText()
	{
		return GetTaskListTaskText();
	}
	
	//*********************//
	//PUBLIC MEMBER METHODS//
	//*********************//
	
	//------------------------------------------------------------------------------------------------
	

	//------------------------------------------------------------------------------------------------
	
	//------------------------------------------------------------------------------------------------
	override void SetDescriptionWidgetText(notnull TextWidget textWidget, string taskText)
	{
		//string baseName;
		
		
		
		//textWidget.SetTextFormat(taskText, baseName);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns the type of this task.
	SCR_CampaignTaskType GetType()
	{
		return m_eType;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Sets the type of this task.
	void SetType(SCR_CampaignTaskType type)
	{
		m_eType = type;
		
		// Assign proper waypoint unless it's a relay 
		
	}
	
	/*
	//------------------------------------------------------------------------------------------------
	override void Deserialize(ScriptBitReader reader)
	{
		super.Deserialize(reader);
		
		SCR_CampaignTaskType type;
		reader.ReadIntRange(type, 0, SCR_CampaignTaskType.LAST-1);
		SetType(type);
	}
	
	//------------------------------------------------------------------------------------------------
	override void Serialize(ScriptBitWriter writer)
	{
		super.Serialize(writer);
		writer.WriteIntRange(GetType(), 0, SCR_CampaignTaskType.LAST-1);
		//serialize faction!!
	}
	*/
	
	
	//------------------------------------------------------------------------------------------------
	void WST_DeliverTask(IEntitySource src, IEntity parent)
	{
		if (SCR_Global.IsEditMode(this))
			return;
		
		if (!GetTaskManager().IsProxy() )
			SCR_BaseTaskManager.s_OnPeriodicalCheck2Second.Insert(CheckPeriodic);
	}
	
	//------------------------------------------------------------------------------------------------
	void ~WST_DeliverTask()
	{
		if (SCR_Global.IsEditMode(this) || !GetGame().GetGameMode())
			return;
		
		if (!GetTaskManager().IsProxy())
			SCR_BaseTaskManager.s_OnPeriodicalCheck2Second.Remove(CheckPeriodic);

		
	}
	
	

}
