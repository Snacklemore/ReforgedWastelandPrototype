class TriggerSpawnManagerClass : GenericEntityClass{};


class TriggerSpawnManager : GenericEntity
{
	ref array<SCR_SpawnPoint> m_spawnPoints;
	
	//list of all triggers to enable spawning of allied faction members
	ref array<SCR_CharacterTriggerEntity> m_aTriggerSpawner;
	ref array<SCR_SpawnPoint> m_aSpawnedSpawnPoints;

	
	override void EOnInit(IEntity owner)
	{
		
		//m_aTriggerSpawner = SCR_CharacterTriggerEntity.Cast(GetGame().GetWorld().FindEntityByName("Trigger1"));
		
		//get all entities?
		//->register Trigger at SpawnManager
		SCR_BaseGameModeWasteland gameMode =  SCR_BaseGameModeWasteland.Cast(GetGame().GetGameMode());

		if ( gameMode.IsMaster() && !gameMode.IsProxy() )
		{
			Print("TriggerSpawnerManager::initOnServer!");

			GetGame().GetCallqueue().CallLater(delay,2000,false);

		}

	}
	
	void delay()
	{
		SCR_BaseGameModeWasteland gameMode =  SCR_BaseGameModeWasteland.Cast(GetGame().GetGameMode());

		if ( gameMode.IsMaster() && !gameMode.IsProxy() )
		{
		
			foreach(SCR_CharacterTriggerEntity trigger: m_aTriggerSpawner)
			{
				Print("TriggerSpawnerManager::init");
				vector mat[4];
				trigger.GetTransform(mat);
				SpawnSpawnPoint(mat);
				//break;
			}
		}
	}
	
	bool SpawnSpawnPoint(vector mat[4] )
	{
		
		
		Resource resource = Resource.Load("{5392C4D80E4B18F8}Prefabs/MP/Spawning/SpawnPoint_FFA.et");
		if (!resource.IsValid())
			return false;
		
		
		EntitySpawnParams spawnParams = new EntitySpawnParams();
		
		spawnParams.TransformMode = ETransformMode.WORLD;
		//this.GetTransform(spawnParams.Transform);
		spawnParams.Transform = mat;
		IEntity ent = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(),spawnParams);
		SCR_SpawnPoint sp = SCR_SpawnPoint.Cast(ent);
		m_aSpawnedSpawnPoints.Insert(sp);
		sp.SetFactionKey("A");
		sp.Event_OnSpawnPointCountChanged.Invoke();
		
		
		sp.LinkInfo(SCR_UIInfo.CreateInfo("Trigger Spawn"));
		if (!ent)
			return false;
		return true;
	}
	bool register(SCR_CharacterTriggerEntity trigger)
	{
		m_aTriggerSpawner.Insert(trigger);
		return true;
	}
	void TriggerSpawnManager(IEntitySource src, IEntity parent)
	{
		m_aTriggerSpawner = new array<SCR_CharacterTriggerEntity>();
		m_aSpawnedSpawnPoints = new array<SCR_SpawnPoint>();
		#ifdef WORKBENCH
			SetFlags(EntityFlags.TRACEABLE, false);
		#else
			SetFlags(EntityFlags.NO_LINK, false);
		#endif
		
		SetEventMask(EntityEvent.INIT);
	}
	
};