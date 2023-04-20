//------------------------------------------------------------------------------------------------
[EntityEditorProps(category: "GameScripted/Tasks", description: "WST task support entity.", color: "0 0 255 255")]
class WST_DeliverTaskSupportEntityClass: SCR_EditorTaskSupportEntityClass
{
};

//------------------------------------------------------------------------------------------------
class WST_DeliverTaskSupportEntity : SCR_EditorTaskSupportEntity 
{
	protected bool m_bInitialTasksGenerated = false;
	protected ref array<IEntity> m_aDeliveryItems = {};
	ScriptInvoker s_TaskManagerOnTaskAssigned;
	
	ref array<int> m_playerIdSetItemAddedCallback = new array<int>();
	ScriptInvoker s_TaskManagerOnTaskunassigned;
	//------------------------------------------------------------------------------------------------
	//! Returns the found task if it exists.
	//TODO: Remove this method after no longer needed.
	WST_DeliverTask GetTask(Faction targetFaction, SCR_CampaignTaskType type)
	{
		if (!GetTaskManager())
			return null;
		
		array<SCR_BaseTask> tasks = {};
		int count = GetTaskManager().GetTasks(tasks);
		
		for (int i = count - 1; i >= 0; i--)
		{
			WST_DeliverTask castTask = WST_DeliverTask.Cast(tasks[i]);
			
			if (!castTask)
				continue;
			
			if (castTask.GetType() == type && castTask.GetTargetFaction() == targetFaction)
				return castTask;
		}
		return null;
	}
	
	
	//------------------------------------------------------------------------------------------------
	//! Server side
	//! Called after a new base has been captured or a mobile HQ deployed
	//! Creates capture tasks for small, major and main bases in newly gained range
	void GenerateCaptureTasks(notnull IEntity entity)
	{
		if (!GetTaskManager())
			return;
		
		
	}
	
	
	
	//------------------------------------------------------------------------------------------------
	protected void GenerateInitialTasksForAllBases()
	{
		if (!GetTaskManager() || !GetTaskManager().Initialized() || m_bInitialTasksGenerated)
			return;
		
		m_bInitialTasksGenerated = true;
		
		//On proxy we don't create any tasks, we skip the creation
		if (GetTaskManager().IsProxy())
			return;
		
		/*
		//Get bases manager
		SCR_CampaignBaseManager manager = SCR_CampaignBaseManager.GetInstance();
		
		//Get all bases
		array<SCR_CampaignBase> bases = manager.GetBases();
		
		//Create start tasks for each base
		for (int i = bases.Count() - 1; i >= 0; i--)
		{
			if (!bases[i].GetIsEnabled())
				continue;
			
			// Create tasks for this base
			if (bases[i].GetOwningFaction())
				GenerateNewTask(bases[i]);
		}
		
		//Let the game mode know, the tasks are ready
		SCR_BaseGameMode gamemode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
		if (gamemode)
			gamemode.HandleOnTasksInitialized();
		*/
	}
	
		
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RPC_SetTargetDestination(int taskID, vector destination)
	{
		Print("WST_DeliverTaskSupportEntity::Set Target Broadcast!");

		if (!GetTaskManager())
			return;
		
		WST_DeliverTask task = WST_DeliverTask.Cast(GetTaskManager().GetTask(taskID));
		if (!task)
			return;
		
		if(!destination)
			return;
		Print("WST_DeliverTaskSupportEntity::Set Target Broadcast:Setting Target");

		task.SetTargetDestination(destination);
	}
	
	//------------------------------------------------------------------------------------------------
	void OnTaskUpdate(SCR_BaseTask task)
	{
	}
	
	//------------------------------------------------------------------------------------------------
	void SetTargetDestination(notnull WST_DeliverTask task,  vector destination)
	{
		if (!GetTaskManager())
			return;
		
		int taskID;
		
		taskID = task.GetTaskID();
		
		
		Rpc(RPC_SetTargetDestination, taskID, destination);
		RPC_SetTargetDestination(taskID, destination);
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		if (!SCR_BaseTaskManager.s_OnTaskUpdate)
			return;
		
		SCR_BaseTaskManager.s_OnTaskUpdate.Insert(OnTaskUpdate);
	}
	void OnAssignedHook(SCR_BaseTask task = null)
	{
		Print("WST_DeliverTaskSupportEntity::OnAssignedHook");
		if(!task)
			return;
		//attach onItemAddedInvoker to StorageManagerComponent
		array<SCR_BaseTaskExecutor> assignees = {};
		task.GetAssignees(assignees);
		if(assignees.IsEmpty())
			return;
		
		
		
		int assignees_count = assignees.Count();
		Print("WST_DeliverTaskSupportEntity::Assignees Count: " + assignees_count);

		int playerID = SCR_BaseTaskExecutor.GetTaskExecutorID(assignees[0]); 
		//check for playerID in m_playerIdSetItemAddedCallback
		
		
		
		Print("WST_DeliverTaskSupportEntity::Invoker already set!");

		
		
		
		Print("WST_DeliverTaskSupportEntity::Invoker not set add now then add to this array >>> m_playerIdSetItemAddedCallback !");
		PlayerManager pm = GetGame().GetPlayerManager();
		PlayerController pc = pm.GetPlayerController(playerID);
		
		IEntity controlled = pc.GetControlledEntity();
		if (controlled)
			Print("WST_DeliverTaskSupportEntity::OnAssignedHook controlled");

		SCR_InventoryStorageManagerComponent imc = SCR_InventoryStorageManagerComponent.Cast(controlled.FindComponent(SCR_InventoryStorageManagerComponent));
		if (imc)
			Print("WST_DeliverTaskSupportEntity::OnAssignedHook IMC found");
		WST_DeliverTask castTask = WST_DeliverTask.Cast(task);
		castTask.SetInvokerCallBack(imc);
		
		
	
		
	
	}
	
	
	
	
	void OnUnassignedHook(SCR_BaseTask task = null)
	{
		
		int count = task.GetAssigneeCount();
		Print("WST_DeliverTaskSupportEntity:OnUnassignedHook:Assignees Count: ");

	
	}
	//------------------------------------------------------------------------------------------------
	//! Creates a new campaign task.
	WST_DeliverTask CreateNewDeliverTask(Faction faction, vector deliveryDestination,IEntity itemToDeliver ,WST_Type type  )
	{
		if (!GetTaskManager())
			return null;
		s_TaskManagerOnTaskAssigned.Insert(OnAssignedHook);
		s_TaskManagerOnTaskunassigned.Insert(OnUnassignedHook);

		
		
		
		
		Print("WST_DeliverTaskSupportEntity::Creating new Task!");
		SCR_BaseTask bTask = CreateTask();
		if (!bTask)
			return null;
		Print("WST_DeliverTaskSupportEntity::BaseTask created!");

///CHECK PREFAB FOR CORRECT COMPOSITION
		WST_DeliverTask task = WST_DeliverTask.Cast(bTask);

		if (!task)
			return null;
		Print("WST_DeliverTaskSupportEntity::New DeliverTask created!");

		SetTargetFaction(task, faction);
		 // Replicated internally
		//SetTaskType(task, type); // Replicated internally
		
		if (itemToDeliver)
		{
			//task to deliver IEntity 
			//move task marker to IEntity (which need to spawn first!)
			SetTargetDestination(task, Vector(0,0,0));
		}else if(type == WST_Type.WST_WEAPON)
		{
			//task to deliver any item of specified type
			//move task marker to deliveryDestination 
			SetTargetDestination(task, deliveryDestination);
			task.SetIndividual(true); // Replicated internally
			Print("WST_DeliverTaskSupportEntity::CreateNewTask:: Origin set!!");
			//SCR_BaseTaskManager.s_OnTaskAssigned called when task is assigned 
			//use to add to onItemAdded in IMC 
		
		
		}
	
		
		return task;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetTaskType(notnull WST_DeliverTask task, SCR_CampaignTaskType type)
	{
		if (!GetTaskManager())
			return;
		
		int taskID = task.GetTaskID();
		
		Rpc(RPC_SetTaskType, taskID, type);
		RPC_SetTaskType(taskID, type);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RPC_SetTaskType(int taskID, SCR_CampaignTaskType type)
	{
		if (!GetTaskManager())
			return;
		
		WST_DeliverTask task = WST_DeliverTask.Cast(GetTaskManager().GetTask(taskID));
		if (!task)
			return;
		
		task.SetType(type);
	}
	
	//------------------------------------------------------------------------------------------------
	override void Initialize()
	{
		s_TaskManagerOnTaskAssigned = GetTaskManager().s_OnTaskAssigned;
		s_TaskManagerOnTaskunassigned = GetTaskManager().s_OnTaskUnassigned;
	}
	
	//------------------------------------------------------------------------------------------------
	void WST_DeliverTaskSupportEntity(IEntitySource src, IEntity parent)
	{
		//Register to script invokers
		//SCR_GameModeCampaignMP.s_OnBaseCaptured.Insert(OnBaseCaptured);
		//SCR_CampaignBaseManager.s_OnAllBasesInitialized.Insert(OnAllBasesInitialized);
		//SCR_GameModeCampaignMP.s_OnSignalChanged.Insert(OnBaseCaptured);
	}
	
	//------------------------------------------------------------------------------------------------
	void ~WST_DeliverTaskSupportEntity()
	{
		//Unregister from script invokers
		SCR_BaseTaskManager.s_OnTaskAssigned.Remove(OnAssignedHook);
		//SCR_GameModeCampaignMP.s_OnBaseCaptured.Remove(OnBaseCaptured);
		//SCR_CampaignBaseManager.s_OnAllBasesInitialized.Remove(OnAllBasesInitialized);
		//SCR_GameModeCampaignMP.s_OnSignalChanged.Remove(OnBaseCaptured);
	}
};