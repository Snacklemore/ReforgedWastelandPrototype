
class SCRWST_AISpawnManagerClass: GenericEntityClass
{
};

class SCRWST_AISpawnManager: GenericEntity
{
	
	ref array<SCR_SpawnPoint> m_aSpawnPoints;
	ref set<SCR_EditableEntityComponent> entities;
	ref array<SCR_EditableEntityComponent> commentEntities;
	
	override void EOnInit(IEntity owner)
	{
		Print("SCRWST_AISpawnManagerClass::EOnInit()");
		
		SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
	       if (!core)
	           return;
		entities = new set<SCR_EditableEntityComponent>();
		commentEntities = new array<SCR_EditableEntityComponent>();
		core.GetAllEntities(entities,true,true);
		//Print("SCRWST_AISpawnManager::EOnInit::AllSCR_EditableEntityComponent"+ entities);
		
		//filter out EEditableEntityType.COMMENT
		foreach (SCR_EditableEntityComponent ent : entities)
		{
			if (ent.GetEntityType() == 6)
				commentEntities.Insert(ent);
			//Print("SCRWST_AISpawnManagerClass::EOnInit()::EntityType = " +ent.GetEntityType() );
			
			
		}
		bool spawnPointSet = false;
		foreach (SCR_EditableEntityComponent ent : commentEntities)
		{
		
			GenericEntity parent = ent.GetOwner();
			SCR_MapDescriptorComponent mapDescr = SCR_MapDescriptorComponent.Cast(parent.FindComponent(SCR_MapDescriptorComponent));
			string LocationName;
			if (!mapDescr)
				return;
			MapItem item = mapDescr.Item();
			LocationName = item.GetDisplayName();
			Print("SCRWST_AISpawnManagerClass::EOnInit()::LocationName" + LocationName);
			
			
			
			ResourceName l_prefabString = "{DE7ABA5055B00185}Prefabs/MP/Campaign/Bases/CampaignRemnantsSpawnPointEntity.et";
			Resource resource = Resource.Load(l_prefabString);
			if (!resource.IsValid())
				return;
			
			
			
			EntitySpawnParams params = EntitySpawnParams();
			params.TransformMode = ETransformMode.WORLD;				
			params.Transform[3] = ent.GetOwner().GetOrigin();
			SCR_CampaignRemnantsSpawnPoint SpawnPoint = SCR_CampaignRemnantsSpawnPoint.Cast(GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params));
			SpawnPoint.SetGroupType(SCR_CampaignRemnantsGroupType.MG);
			
			Print("SCRWST_AISpawnManagerClass::EOnInit()::Spawned CampaignRemnantsSpawnPointEntity at Origin of " + LocationName);
			if(SpawnPoint)
				spawnPointSet = true;
			
			
		}
		
		
		
		
		Print("SCRWST_AISpawnManagerClass::EOnInit()::End" );

	}
	
	
void SCRWST_AISpawnManager(IEntitySource src, IEntity parent)
	{
		#ifdef WORKBENCH
			SetFlags(EntityFlags.TRACEABLE, false);
		#else
			SetFlags(EntityFlags.NO_LINK, false);
		#endif
		
		SetEventMask(EntityEvent.INIT);
	}
	
};