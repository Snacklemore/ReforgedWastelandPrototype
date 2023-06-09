
class SCRWST_AISpawnManagerClass: GenericEntityClass
{
};

class SCRWST_AISpawnManager: GenericEntity
{
	
	ref array<SCR_SpawnPoint> m_aSpawnPoints;
	ref set<SCR_EditableEntityComponent> entities;
	ref array<SCR_EditableEntityComponent> commentEntities;
	ref array<SCR_EditableEntityComponent> citys;
	ref array<SCR_EditableEntityComponent> hills;
	
	
	override void EOnInit(IEntity owner)
	{
		RplComponent rpl = RplComponent.Cast(FindComponent(RplComponent));
		//if(rpl.IsProxy())
			//return;
		Print("SCRWST_AISpawnManagerClass::EOnInit()");
		SCR_EditableEntityCore core = SCR_EditableEntityCore.Cast(SCR_EditableEntityCore.GetInstance(SCR_EditableEntityCore));
	       if (!core)
	           return;
		entities = new set<SCR_EditableEntityComponent>();
		commentEntities = new array<SCR_EditableEntityComponent>();
		 citys = new array<SCR_EditableEntityComponent>();
		hills = new array<SCR_EditableEntityComponent>();
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
			string EntityName = parent.GetName();
			
			//filter out name with a capital "C" at the beginning. Those are Citys.
			//format city:
			//-> C_LocationChotain
			//format Hills 
			//-> H_HumboldtHill
			if(EntityName.Contains("C_"))
			{	
				//City 
				Print("SCRWST_AISpawnManagerClass::EOnInit()::Adding city, " + EntityName);
				citys.Insert(ent);

				
			
			}else if (EntityName.Contains("H_"))
			{
				//Hill 
				Print("SCRWST_AISpawnManagerClass::EOnInit()::Adding Hill " + EntityName);
				hills.Insert(ent);

				
			
			}
			
			Print("SCRWST_AISpawnManagerClass::EOnInit()::LocationName" + LocationName);
			
			
			
			
			
			
		}
		
		foreach (SCR_EditableEntityComponent city : citys)
		{
			ResourceName l_prefabString = "{6AE3C21D9B733704}Prefabs/MP/Campaign/Bases/CampaignRemnantsSpawnPointEntityMod.et";
			Resource resource = Resource.Load(l_prefabString);
			if (!resource.IsValid())
				return;
			
			
		
			EntitySpawnParams params = EntitySpawnParams();
			params.TransformMode = ETransformMode.WORLD;				
			params.Transform[3] = city.GetOwner().GetOrigin();
			SCR_CampaignRemnantsSpawnPoint SpawnPoint = SCR_CampaignRemnantsSpawnPoint.Cast(GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params));
			SpawnPoint.SetGroupType(SCR_CampaignRemnantsGroupType.MG);
			SpawnPoint.SetIsRespawn(true);
			SpawnPoint.SetGroupRespawnPeriod(1);
			SCR_MapDescriptorComponent mds = SCR_MapDescriptorComponent.Cast(SpawnPoint.FindComponent(SCR_MapDescriptorComponent));
			string ident = city.GetDisplayName();
			SpawnPoint.setident(ident);
			Print("SCRWST_AISpawnManagerClass::EOnInit()::Spawned CampaignRemnantsSpawnPointEntity at Origin of ");
			if(SpawnPoint)
			spawnPointSet = true;
		}
		
		foreach (SCR_EditableEntityComponent hill : hills)
		{
			ResourceName l_prefabString = "{6AE3C21D9B733704}Prefabs/MP/Campaign/Bases/CampaignRemnantsSpawnPointEntityMod.et";
			Resource resource = Resource.Load(l_prefabString);
			if (!resource.IsValid())
				return;
			
			
		
			EntitySpawnParams params = EntitySpawnParams();
			params.TransformMode = ETransformMode.WORLD;				
			params.Transform[3] = hill.GetOwner().GetOrigin();
			SCR_CampaignRemnantsSpawnPoint SpawnPoint = SCR_CampaignRemnantsSpawnPoint.Cast(GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params));
			SpawnPoint.SetGroupType(SCR_CampaignRemnantsGroupType.MG);
			
			string ident = hill.GetDisplayName();
			SpawnPoint.setident(ident);
			
			Print("SCRWST_AISpawnManagerClass::EOnInit()::Spawned CampaignRemnantsSpawnPointEntity at Origin of");
			if(SpawnPoint)
			spawnPointSet = true;
		}
		
		
		
		//register to WasteBaseGameMode
		
		SCR_BaseGameModeWasteland mode = SCR_BaseGameModeWasteland.Cast(GetGame().GetGameMode());
		mode.registerAISpawnManager(this);
		
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