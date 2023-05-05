class WST_AIMissionManagerClass : GenericEntityClass
{

};
//manages AI mission objective Locations 
//-->always keeps 3 locations active 
//-->spawns the camp prefab, AIGroup and task specicfic items( to complete tasks if you acccepted one)

//FLOW:
//GameModeStart
//->Start periodic check every 30s 
// |-->Check if all three missions are Spawned (by checking AI Group members alive)
//->if not spawn missions with ai group 


class WST_AIMissionManager : GenericEntity
{

	protected static WST_AIMissionManager s_Instance = null;
	ref array<WST_AIMissionObject> m_MissionObjects;
	bool m_allMissionsSpawned = false;
	RplComponent m_Rpl;
	int activeMissionCount;
	int maxActiveMissionCount;
	void WST_AIMissionManager(IEntitySource src, IEntity parent)
	{
		#ifdef WORKBENCH
			SetFlags(EntityFlags.TRACEABLE, false);
		#else
			SetFlags(EntityFlags.NO_LINK, false);
		#endif
		if(!s_Instance)
			s_Instance = this;
		m_MissionObjects = new array<WST_AIMissionObject>();
		SetEventMask(EntityEvent.INIT);
		

	}
	
	void RegisterMissionObject(WST_AIMissionObject m_object)
	{
		m_MissionObjects.Insert(m_object);
		
	
	}
	
	
	static WST_AIMissionManager GetInstance()
	{
		return s_Instance;
	}
	void PeriodicCheck30s()
	{
		if (activeMissionCount >= maxActiveMissionCount)
			return;
		foreach(WST_AIMissionObject object : m_MissionObjects)
		{
			if (activeMissionCount >= maxActiveMissionCount)
				return;
			//check if spawned already 
			if(object.isSpawned)
				continue;
			//spawn compostion 
			SpawnMissionCompositionPrefab(object);
			SpawnAIGroup(object);
			activeMissionCount++;
		}
	}
	
	void SpawnMissionCompositionPrefab(WST_AIMissionObject object)
	{
		if(m_Rpl.IsProxy())
			return;
		if(!m_Rpl.IsMaster())
			return;
		//get resource 
		ResourceName n =object.m_missionLocationPrefab;
		//position of MissionObject 
		vector objectPos = object.GetOrigin();
		
		//spawn composition entity 
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = object.GetOrigin();
		IEntity spawned = GetGame().SpawnEntityPrefab(Resource.Load(n),GetGame().GetWorld(),params);
		object.SetSpawnedComposition(spawned);
		object.SnapToGround(spawned);
	}
	
	void SpawnAIGroup(WST_AIMissionObject object)
	{
		if(m_Rpl.IsProxy())
			return;
		if(!m_Rpl.IsMaster())
			return;
		ResourceName n = object.m_AIGroupPrefab;
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform[3] = object.GetOrigin();
		//shift 10m in z
		params.Transform[3][2] = params.Transform[3][2] +10.0;
		//snap to terrain 
		SCR_TerrainHelper.SnapToTerrain(params.Transform, GetGame().GetWorld());
		
		
		SCR_AIGroup grp = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(Resource.Load(n), GetGame().GetWorld(), params));
		object.SetSpawnedGroup(grp);
		
	}
	
	void DoActualCleanUp()
	{
	
		foreach (WST_AIMissionObject obj: m_MissionObjects)
		{
			//check which object has no AIAgents left 
			
			if(!obj.m_SpawnedGroup)
			{
				//already deleted 
				SCR_EntityHelper.DeleteEntityAndChildren(obj.m_SpawnedComposition);
				//setting to spawnable after 2min via callqueue
				TwoMinuteCoolDown(obj);
				activeMissionCount--;
				continue;
			}
				
			int alive = obj.m_SpawnedGroup.GetAgentsCount();
			if (alive == 1)
			{
				//delete composition 
				SCR_EntityHelper.DeleteEntityAndChildren(obj.m_SpawnedComposition);			
				//setting to spawnable after 2min via callqueue
				TwoMinuteCoolDown(obj);
				activeMissionCount--;
			
			}
		}
		
	}
	void TwoMinuteCoolDown(WST_AIMissionObject o)
	{
		GetGame().GetCallqueue().CallLater(cd,120000,false,o);
	}
	
	void cd(WST_AIMissionObject o)
	{
	
		o.isSpawned = false;

	}
	void CleanUpEmptyMission()
	{
		//cleanup after 2min 
		GetGame().GetCallqueue().CallLater(DoActualCleanUp,120000,false)
	
	}
	void Init()
	{
		int count = m_MissionObjects.Count();
		activeMissionCount = 0;
		maxActiveMissionCount = 3;
		Print("Registered "+ count + " objects");
		m_Rpl = RplComponent.Cast(FindComponent(RplComponent));
		//spawn composition 
		GetGame().GetCallqueue().CallLater(PeriodicCheck30s,30000,true);
	}
	
	
	override void EOnInit(IEntity owner)
	{
		GetGame().GetCallqueue().CallLater(Init,5000,false);
		
	}

	
}


class WST_AIMissionObjectClass : GenericEntityClass
{

}

[BaseContainerProps()]
class WST_AIMissionObject : GenericEntity
{
	IEntity m_SpawnedComposition;
	AIGroup m_SpawnedGroup;
	bool isSpawned = false;
	[Attribute("{DF6E27D116B7B995}Prefabs/AIMissionPrefabs/FirstMissionCompositionRPL.et")]
	ResourceName m_missionLocationPrefab;
	
	
	[Attribute("{1A729671A161C87C}Prefabs/Groups/BLUFOR/Group_FireTeamWaste_BigMoneyLoot_Mission.et")]
	ResourceName m_AIGroupPrefab;

	
	void WST_AIMissionObject(IEntitySource src, IEntity parent)
	{
		#ifdef WORKBENCH
			SetFlags(EntityFlags.TRACEABLE, false);
		#else
			SetFlags(EntityFlags.NO_LINK, false);
		#endif
		
		
		SetEventMask(EntityEvent.INIT);
		

	}
	
	void SnapToGround(IEntity entity)
	{
		//iterate over all Entities of composition 
		IEntity child = entity.GetChildren();
		while(child)
		{
			
			
			
			Print("Snapping to ground!");
			//first child of root 
			//do stuff 
			//get transform 
			vector mat[4] = {};
			child.GetTransform(mat);
			SCR_TerrainHelper.SnapToTerrain(mat, GetGame().GetWorld());
			child.SetTransform(mat);
			child.Update();
			child = child.GetSibling();
			
			
		
		}
	
	
	}
	void SetSpawnedComposition(IEntity spawned)
	{
		m_SpawnedComposition = spawned;
		isSpawned = true;
	}
	
	void SetSpawnedGroup(AIGroup grp)
	{
		m_SpawnedGroup = grp;
	}
	override void EOnInit(IEntity owner)
	{
		WST_AIMissionManager.GetInstance().RegisterMissionObject(this);
		
	}

	
}