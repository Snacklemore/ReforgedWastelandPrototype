

modded class SCR_AIGroup
{


	protected void SpawnGroupMember(bool snapToTerrain, int index, ResourceName res, bool editMode, bool isLast)
	{
		if (!GetGame().GetAIWorld().CanAICharacterBeAdded())
		{
			if (isLast)
				Event_OnInit.Invoke(this);
			
			//Event_OnLastGroupMemberSpawned.Invoke(this);
			
			return;
		}
		BaseWorld world = GetWorld();
		AIFormationDefinition formationDefinition;
		AIFormationComponent formationComponent = AIFormationComponent.Cast(this.FindComponent(AIFormationComponent));
		if (formationComponent)
			formationDefinition = formationComponent.GetFormation();
		EntitySpawnParams spawnParams = new EntitySpawnParams;
		spawnParams.TransformMode = ETransformMode.WORLD;
		GetWorldTransform(spawnParams.Transform);
		vector pos = spawnParams.Transform[3];

		if (formationDefinition)
			pos = CoordToParent(formationDefinition.GetOffsetPosition(index));
		else
			pos = CoordToParent(Vector(index, 0, 0));

		float surfaceY = world.GetSurfaceY(pos[0], pos[2]);
		if (snapToTerrain && pos[1] < surfaceY)
		{
			pos[1] = surfaceY;
		}

		//Snap to the nearest navmesh point
		AIPathfindingComponent pathFindindingComponent = AIPathfindingComponent.Cast(this.FindComponent(AIPathfindingComponent));
		if (pathFindindingComponent && pathFindindingComponent.GetClosestPositionOnNavmesh(pos, "10 10 10", pos))
		{
			float groundHeight = world.GetSurfaceY(pos[0], pos[2]);
			if (pos[1] < groundHeight)
				pos[1] = groundHeight;
		}
		
		spawnParams.Transform[3] = pos;
		
		IEntity member = GetGame().SpawnEntityPrefab(Resource.Load(res), world, spawnParams);
		if (!member)
			return;
		
		if (editMode)
			m_aSceneGroupUnitInstances.Insert(member);

		AddAIEntityToGroup(member,index+1);
		FactionAffiliationComponent factionAffiliation = FactionAffiliationComponent.Cast(member.FindComponent(FactionAffiliationComponent));
		//AIGROUPMOD
		//create config component for money, get money component, get config component, put data in moneycomponent 
		WST_MoneyConfigComponent configComp = WST_MoneyConfigComponent.Cast(member.FindComponent(WST_MoneyConfigComponent));
		
			array<typename> components = {};
			components.Insert(MoneyComponent);
			components.Insert(RplComponent);
		InventoryStorageManagerComponent storage = InventoryStorageManagerComponent.Cast(member.FindComponent(InventoryStorageManagerComponent));
		
		IEntity walletEnity = storage.FindItemWithComponents(components, EStoragePurpose.PURPOSE_DEPOSIT);
		MoneyComponent MoneyComp;
		if(walletEnity)
		{
			MoneyComp = MoneyComponent.Cast(walletEnity.FindComponent(MoneyComponent));
		}
		if (MoneyComp && configComp)
		{
			Print("SCR_AIGroupMod::SpawnGroupMember:: Setting wallet value to configured value of "+ configComp.GetValue());
			MoneyComp.SetValue(configComp.GetValue());
		}
		
		
		if (factionAffiliation)
			factionAffiliation.SetAffiliatedFactionByKey(m_faction);
	
		if (isLast)
			Event_OnInit.Invoke(this);
	}
	

};

