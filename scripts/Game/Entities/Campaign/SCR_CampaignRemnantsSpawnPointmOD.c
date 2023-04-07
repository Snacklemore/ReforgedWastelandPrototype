

//------------------------------------------------------------------------------------------------
//! Spawn point for Remnant forces in Campaign
modded class SCR_CampaignRemnantsSpawnPoint 
{
	[Attribute("-1", UIWidgets.EditBox, "Waypoints will be placed in ascending sequence. Valid only for hierarchy children.")]
	protected int m_iWaypointIndex;
	
	[Attribute("0", UIWidgets.ComboBox, "Valid only for hierarchy parent.", "", ParamEnumArray.FromEnum(SCR_CampaignRemnantsGroupType))]
	private SCR_CampaignRemnantsGroupType m_eGroupType;
	
	[Attribute("0", UIWidgets.EditBox, "How often will the group respawn. (0 = no respawn)")]
	protected int m_iGroupRespawnPeriod;
	
	[Attribute("0", UIWidgets.CheckBox, "This entity is used as a respawn position from which Remnants will move to their target.")]
	protected bool m_bIsRespawn;
	
	
	
	
	
	
	ref array<SCR_Position> m_posArray;
	ref array<vector> m_positions;
	
	//fills m_posArray with SCR_Position objects 
	private void GetNearbyPositionObjects(float radius)
	{
		vector pos = this.GetOrigin();
		if (GetGame().GetWorld())
				GetGame().GetWorld().QueryEntitiesBySphere(pos,radius,positionPoint,TrueWhenEntityIsPositionPoint,EQueryEntitiesFlags.ALL);
	}
	
	bool positionPoint(notnull IEntity entity)
	{
		SCR_Position pos = SCR_Position.Cast(entity);
		if (!pos)
		{
			Print("SCR_CharacterTriggerEntityMod::spawnPoints()::notaspawnpointerror");
			return false;
		}
		SetPositionPoint(pos);
		return true;
			
		
	}
	bool TrueWhenEntityIsPositionPoint(IEntity entity)
	{
		SCR_Position sp = SCR_Position.Cast(entity);
		if (!sp)
		{
			return false;
		}
		return true;
									
	}
	void SetPositionPoint(SCR_Position pos)
	{
	
		m_posArray.Insert(pos);
	}
	void FillRespawns()
	{
		
		m_posArray = new array<SCR_Position>();
		GetNearbyPositionObjects(250.0);
		//if (m_posArray.Count() == 3)
		//{
			
			//query is getting inherited classes too!!! eg. SCR_SpawnPoint
			//SCR_Position pos = m_posArray.Get(0);
			foreach (SCR_Position pos : m_posArray)
			{
				//filter out unwantend SCR_SpawnPoint objects by checking Entity names 
				string name = pos.GetName();
				if (name)
				{
					Print("SCR_CampaingRemnantsSpawnPointMod::FillRespawns::PositionName: "+ name);
					if (name.Contains("SP_AI_"))
					{
					if (name.Contains("SP_AI_StPhillipe"))
						Print("Found");
						//Found SCR_Position Object!
						vector vec = pos.GetOrigin();
						m_positions.Insert(vec);
					
					}
				
				}
			}
		//}
		//use SCR_Positions from editor to fill with vector 
		//get SCR_Position Objects
								
	
	}
	
	void FillPresence(SCR_CampaignRemnantsPresence presence)
	{
		presence.FillRespawns(m_positions);
	}
		
	//------------------------------------------------------------------------------------------------
	int GetWaypointIndex()
	{
		return m_iWaypointIndex;
	}
	
	void SetGroupType(SCR_CampaignRemnantsGroupType type)
	{
		m_eGroupType = type;
	}
	
	void SetIsRespawn(bool b)
	{
		m_bIsRespawn = b;
	}
	
	void SetGroupRespawnPeriod(int n)
	{
		m_iGroupRespawnPeriod = n;
	}
	//------------------------------------------------------------------------------------------------
	SCR_CampaignRemnantsGroupType GetGroupType()
	{
		return m_eGroupType;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetGroupRespawnPeriod()
	{
		return m_iGroupRespawnPeriod;
	}
	
	//------------------------------------------------------------------------------------------------
	bool GetIsRespawn()
	{
		return m_bIsRespawn;
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		// No need to register children unless the group type is specified (indicating default base defenders near HQ)
		if (GetParent() && m_eGroupType == SCR_CampaignRemnantsGroupType.RANDOM)
			return;
		
		SCR_BaseGameModeWasteland campaign = SCR_BaseGameModeWasteland.GetInstance();
		
		if (!campaign)
			return;
		int no = Math.RandomInt(1,5);
		SetGroupType(no);
		// Gamemode will take care of the rest
		campaign.RegisterRemnantsPresence(this);
		Print("SCR_CampaignRemnantsSpawnPoint::OnInit()");
	}
	
	//------------------------------------------------------------------------------------------------
	void SCR_CampaignRemnantsSpawnPoint(IEntitySource src, IEntity parent)
	{
		#ifdef WORKBENCH
			SetFlags(EntityFlags.TRACEABLE, false);
		#else
			SetFlags(EntityFlags.NO_LINK, false);
		#endif
		m_positions = new array<vector>();
		SetEventMask(EntityEvent.INIT);
	}
};

modded class SCR_CampaignRemnantsPresence
{
	protected int m_iID;
	protected vector m_vCenter;
	protected vector m_vSpawn;
	protected AIWaypoint m_Waypoint;
	protected int m_iMembersAlive = -1;
	protected ResourceName m_sPrefab;
	protected bool m_bSpawned = false;
	protected float m_fDespawnTimer = -1;
	protected SCR_AIGroup m_Group;
	protected SCR_CampaignBase m_ParentBase;
	protected float m_fRespawnTimestamp;
	protected int m_iRespawnPeriod;
	protected ref array<vector> m_aRespawns = {};
	static const int PARENT_BASE_DISTANCE_THRESHOLD = 300;
	static const int RESPAWN_PLAYER_DISTANCE_THRESHOLD = 200;
	
	
	void FillRespawns(array<vector> vectors)
	{
		m_aRespawns = vectors;
	}
	//------------------------------------------------------------------------------------------------
	void SetID(int ID)
	{
		m_iID = ID;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetID()
	{
		return m_iID;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetCenter(vector center)
	{
		m_vCenter = center;
	}
	
	//------------------------------------------------------------------------------------------------
	vector GetCenter()
	{
		return m_vCenter;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetSpawnpoint(vector spawn)
	{
		m_vSpawn = spawn;
	}
	
	//------------------------------------------------------------------------------------------------
	vector GetSpawnpoint()
	{
		return m_vSpawn;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetWaypoint(AIWaypoint wp)
	{
		m_Waypoint = wp;
	}
	
	//------------------------------------------------------------------------------------------------
	AIWaypoint GetWaypoint()
	{
		return m_Waypoint;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetMembersAlive(int cnt)
	{
		m_iMembersAlive = cnt;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetMembersAlive()
	{
		return m_iMembersAlive;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetGroupPrefab(ResourceName prefab)
	{
		m_sPrefab = prefab;
	}
	
	//------------------------------------------------------------------------------------------------
	ResourceName GetGroupPrefab()
	{
		return m_sPrefab;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetIsSpawned(bool spawned)
	{
		m_bSpawned = spawned;
	}
	
	//------------------------------------------------------------------------------------------------
	bool GetIsSpawned()
	{
		return m_bSpawned;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetDespawnTimer(float time)
	{
		m_fDespawnTimer = time;
	}
	
	//------------------------------------------------------------------------------------------------
	float GetDespawnTimer()
	{
		return m_fDespawnTimer;
	}
	
	//------------------------------------------------------------------------------------------------
	float GetRespawnTimestamp()
	{
		return m_fRespawnTimestamp;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetRespawnTimestamp(float time)
	{
		m_fRespawnTimestamp = time;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetRespawnPeriod()
	{
		return m_iRespawnPeriod;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetRespawnPeriod(int time)
	{
		m_iRespawnPeriod = time;
	}
	
	//------------------------------------------------------------------------------------------------
	void AddRespawn(vector respawn)
	{
		m_aRespawns.Insert(respawn);
	}
	
	//------------------------------------------------------------------------------------------------
	array<vector> GetRespawns()
	{
		return m_aRespawns;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetSpawnedGroup(SCR_AIGroup group)
	{
		m_Group = group;
	}
	
	//------------------------------------------------------------------------------------------------
	SCR_AIGroup GetSpawnedGroup()
	{
		return m_Group;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetParentBase(notnull SCR_CampaignBase base)
	{
		m_ParentBase = base;
	}
	
	//------------------------------------------------------------------------------------------------
	SCR_CampaignBase GetParentBase()
	{
		return m_ParentBase;
	}
};