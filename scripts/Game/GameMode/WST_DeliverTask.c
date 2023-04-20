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
		SCR_UniversalInventoryStorageComponent castStorage = SCR_UniversalInventoryStorageComponent.Cast(storage);
		IEntity iStorage = castStorage.GetOwner();
		RplComponent rpl = RplComponent.Cast(iStorage.FindComponent(RplComponent));
		RplId r_id = rpl.Id();
		Print("DeliveryMission::ItemPickUp");
		//SCR_ChimeraCharacter parent = storageOwner.GetParent();
		int p_id = GetGame().GetPlayerManager().GetPlayerIdFromEntityRplId(r_id);
		
		
		
		
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
	protected override void OnAssigneeRemoved(SCR_BaseTaskExecutor oldAssignee)
	{
		
		if (oldAssignee)
			oldAssignee.AssignNewTask(null);
		
		

		
		if (m_aAssignees.Count() <= 0)
			SetState(SCR_TaskState.OPENED);
		
		UpdateMapTaskIcon();
		UpdateTaskListAssignee();
		
		Print("DeliveryMission::Removed Assignee");
		
		
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
	}
	//------------------------------------------------------------------------------------------------
	override void Finish(bool showMsg = true)
	{
		showMsg = SCR_RespawnSystemComponent.GetLocalPlayerFaction() == m_TargetFaction;
		super.Finish(showMsg);
		
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
	override void SetTitleWidgetText(notnull TextWidget textWidget, string taskText)
	{
		//string baseName;
		
		
		//textWidget.SetTextFormat(taskText, baseName);
	}

	//------------------------------------------------------------------------------------------------
	override string GetTitleText()
	{
		//string baseName;

		

		//return string.Format("%1 %2", m_sName, baseName);
	}
	
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
		
		if (!GetTaskManager().IsProxy())
		{
			
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void ~WST_DeliverTask()
	{
		if (SCR_Global.IsEditMode(this) || !GetGame().GetGameMode())
			return;
		
		if (!GetTaskManager().IsProxy())
		{
		}
	}
	
	

}
