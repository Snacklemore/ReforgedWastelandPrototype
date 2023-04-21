
//------------------------------------------------------------------------------------------------
modded class SCR_BaseTaskManager 
{
//------------------------------------------------------------------------------------------------
	//! An event called when a player disconnects
	//! Server only
	override void OnPlayerDisconnected(int id)
	{
#ifdef ENABLE_DIAG
		if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_EXECUTE_TASKS))
			return;
#endif
		
		SCR_BaseTaskExecutor executor = SCR_BaseTaskExecutor.GetTaskExecutorByID(id);
		SCR_BaseTask task = null;
		if (executor)
			task = executor.GetAssignedTask();
		
		SCR_BaseTaskSupportEntity supportEntity = SCR_BaseTaskSupportEntity.Cast(FindSupportEntity(SCR_BaseTaskSupportEntity));
		if (!supportEntity)
			return;
		if(!task)
			return;
		//Failing task with assignees on disconnect creates "ghost task"?
		supportEntity.UnassignTask(task, executor, SCR_EUnassignReason.ASSIGNEE_ABANDON);
		supportEntity.FailTask(task);
		//if (task)
			
		
		//this creates ghost task?
		//task = FindRequestedTask(executor);
		
		//if (!task)
		//	return;
		
		//supportEntity.CancelTask(task.GetTaskID());
	}

};
