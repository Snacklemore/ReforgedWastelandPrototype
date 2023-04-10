class WST_FlipAction : ScriptedUserAction
{
	
	
	Vehicle m_vehicle = null;
	override event void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_vehicle = Vehicle.Cast(pOwnerEntity);
	}
	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		//get WST_FlipComponent
		
		
		vector transform[4];
		pOwnerEntity.GetTransform(transform);
		
		vector angle = Math3D.MatrixToAngles(transform);
		Math3D.AnglesToMatrix(Vector(angle[0],0,0),transform);
		
		
		transform[3][1] = transform[3][1]+0.5;
		
		pOwnerEntity.SetTransform(transform);
		pOwnerEntity.GetPhysics().ApplyImpulse("0 -1 0");

	}

	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		//if(!m_vehicle)
		//	return false;
		//return !m_vehicle.IsOccupied();
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool HasLocalEffectOnlyScript()
	{
		return false;
	}
};