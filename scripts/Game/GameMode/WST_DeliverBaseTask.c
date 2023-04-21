//------------------------------------------------------------------------------------------------
class WST_DeliverBaseTaskClass: SCR_BaseTaskClass
{
};

//------------------------------------------------------------------------------------------------
//! This class is meant to be inherited.
//! Don't use this as your custom task!
class WST_DeliverBaseTask : SCR_BaseTask
{
	
	
	
	//**************************//
	//PROTECTED MEMBER VARIABLES//
	//**************************//
	
	
	protected AIWaypoint m_AssignedWaypoint = null;
	//pickup
	protected vector m_destination ;
	//protected IEntity m_deliveryItem = null;
	//protected WST_Type type = null;

	//*********************//
	//PUBLIC MEMBER METHODS//
	//*********************//
	
	//------------------------------------------------------------------------------------------------
	//! Gets the AI waypoint for this task.
	AIWaypoint GetAIWaypoint()
	{
		if (!m_AssignedWaypoint)
			SetAIWaypoint();

		return m_AssignedWaypoint;
	}
	
	//------------------------------------------------------------------------------------------------
	
	void SetDeliveryItem(IEntity item)
	{
	//		m_deliveryItem = item;
	}
	//------------------------------------------------------------------------------------------------
	//! Sets the destination that is marked as the target of this task.
	void SetTargetDestination(vector v)
	{
		
			
		SetOrigin(v);
		m_destination = v;
		CreateMapUIIcon();
		UpdateMapInfo();
		SetHUDIcon();
	}
	
	//------------------------------------------------------------------------------------------------
	
	
	
	
	
	//------------------------------------------------------------------------------------------------
	//! Returns the vector that is marked as target of this task.

	vector GetTargetDestination()
	{
		return m_destination;
	}
	
	//******************************//
	//PUBLIC OVERRIDE MEMBER METHODS//
	//******************************//
	
	
	//************************//
	//PROTECTED MEMBER METHODS//
	//************************//
	
	//------------------------------------------------------------------------------------------------
	protected void SetAIWaypoint()
	{
		if (m_AssignedWaypoint)
			return;
		
		
		//m_AssignedWaypoint = GetTargetBase().GetTaskWaypoint();
	}
	
	//------------------------------------------------------------------------------------------------
	protected override void UpdateMapInfo()
	{
		//Insert task icon into base's map UI
		//m_TargetBase cannot be null here
	}
};
