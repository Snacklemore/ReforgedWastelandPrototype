//------------------------------------------------------------------------------------------------
[EntityEditorProps(category: "GameScripted/Tasks", description: "Campaign base task support entity.", color: "0 0 255 255")]
class WST_DeliverTaskBaseSupportEntityClass: SCR_BaseTaskSupportEntityClass
{
};

//------------------------------------------------------------------------------------------------
class WST_DeliverTaskBaseSupportEntity : SCR_BaseTaskSupportEntity
{
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RPC_SetTargetDestination(int taskID, vector destination)
	{
		if (!GetTaskManager())
			return;
		
		WST_DeliverBaseTask task = WST_DeliverBaseTask.Cast(GetTaskManager().GetTask(taskID));
		if (!task)
			return;
		
		if(!destination)
			return;
		
		task.SetTargetDestination(destination);
	}
	
	//------------------------------------------------------------------------------------------------
	void OnTaskUpdate(SCR_BaseTask task)
	{
	}
	
	//------------------------------------------------------------------------------------------------
	void SetTargetDestination(notnull WST_DeliverBaseTask task,  vector destination)
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
}