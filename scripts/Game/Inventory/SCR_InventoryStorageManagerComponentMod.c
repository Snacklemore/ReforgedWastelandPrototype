
modded class SCR_InventoryStorageManagerComponent : ScriptedInventoryStorageManagerComponent
{
			
	MoneyComponent m_MoneyComponent;
	//serverside
	void ItemAddedInvocation(IEntity item ,BaseInventoryStorageComponent storageOwner )
	{
		auto moneyComp = MoneyComponent.Cast(item.FindComponent(MoneyComponent));
		
			//1. check for existing wallet
			//2. get value of existing Wallet 
			//3. get value of added wallet 
			//4. delete existing Wallet 
			//5. add value of existing wallet to added Wallet 
		if(moneyComp)
		{
			//when not null there is a wallet already 
			//-->save value of item and delete 
			if (m_MoneyComponent)
			{
				Print("SCR_InventoryStorageManagerComponentMod::OnItemAdded::existing wallet detected with value: " + m_MoneyComponent.GetValue());
				IEntity enti = IEntity.Cast(m_MoneyComponent.GetOwner());
				//save value of added wallet
				int val = moneyComp.GetValue();
				Print("SCR_InventoryStorageManagerComponentMod::OnItemAdded::adding wallet value " + val);

				
				//add both wallet values 
				int sum = val + m_MoneyComponent.GetValue();
				Print("SCR_InventoryStorageManagerComponentMod::OnItemAdded::cmputed wallet sum " + sum);

				//set MoneyComponent new value (and broadcast/sync with server?)
				moneyComp.SetValue(sum);
				

				//delete the cached item
				Print("SCR_InventoryStorageManagerComponentMod::OnItemAdded::deleting wallet  " + m_MoneyComponent.GetRplId() +" with value: "+ m_MoneyComponent.GetValue() );
				m_MoneyComponent.Unregister(true);
		
				//AskServerToDeleteEntity(enti);
				
				
				
				

			}else
			{
				
				Print("SCR_InventoryStorageManagerComponentMod::OnItemAdded::moneyWallet detected with value : "+ moneyComp.GetValue());
			}
			m_MoneyComponent = moneyComp;
			
			
			
		}
	}
	//serverside
	void ItemRemovedInvocation(IEntity item ,BaseInventoryStorageComponent storageOwner)
	{
		auto moneyComp = MoneyComponent.Cast(item.FindComponent(MoneyComponent));
		if(moneyComp )
		{
			//set to null if last wallet
			if(m_MoneyComponent == moneyComp )
				m_MoneyComponent = null;
		}
		
		
		//check if storage box is removed from Inventory 
		ResourceName stashbox_resource = "{86F5F52A6FABBF7C}Prefabs/save_stash_boxPrefab.et";
		EntityPrefabData stashbox_prefab_data = item.GetPrefabData();
		ResourceName item_resource = stashbox_prefab_data.GetPrefabName();
		
		
		
		
		
		//
		/*
		slot.GetWorldTransform(spawnParams.Transform);
		SCR_BasePreviewEntity previewEntity = SCR_PrefabPreviewEntity.SpawnPreviewFromPrefab(Resource.Load(slotData.GetResourceName()), m_PreviewEntityPrefab, slot.GetWorld(), spawnParams, m_PreviewMaterial);
		if (!previewEntity)
			return;*/
		//remove box from gc(for now, might just add lifetime on entity when used)
		
		
		if (stashbox_resource == item_resource)
		{
			GarbageManager garbageManager = GetGame().GetGarbageManager();
			if (garbageManager)
			{
				RplComponent rpl = RplComponent.Cast(item.FindComponent(RplComponent));
				
				if (item.FindComponent(InventoryItemComponent))
				{
					RplId id = rpl.Id();
					if(rpl)
						
						
					garbageManager.Withdraw(item);
					Print("InventoryStorageManagerComponent::OnItemRemoveWithdraw::");

				}
				
				
			}
		}
		
		
	}
	
	
	
	void ItemRemovedInvocationLocal(IEntity item ,BaseInventoryStorageComponent storageOwner)
	{
		
		/*Print("InventoryStorageManagerComponent::LocalRemovedCall");
		ResourceName stashbox_resource = "{86F5F52A6FABBF7C}Prefabs/save_stash_boxPrefab.et";
		EntityPrefabData stashbox_prefab_data = item.GetPrefabData();
		ResourceName item_resource = stashbox_prefab_data.GetPrefabName();
		WST_SaveStashBoxPlacementComponent placementComponent;
		EntitySpawnParams spawnParams = new EntitySpawnParams();
		//get owner of storage 
		IEntity owner = storageOwner.GetOwner();
		IEntity player = GetGame().GetPlayerController().GetControlledEntity();
		placementComponent = WST_SaveStashBoxPlacementComponent.Cast(player.FindComponent(WST_SaveStashBoxPlacementComponent));
		
		//get position 
		vector pos = owner.GetOrigin();
		//set pos for preview 
		spawnParams.Transform[3] = pos;
		//spawns non interactible preview entity of stashbox
		//	static SCR_BasePreviewEntity SpawnPreviewFromPrefab(Resource prefabResource, ResourceName previewPrefab, BaseWorld world = null, EntitySpawnParams spawnParams = null, ResourceName material = ResourceName.Empty, EPreviewEntityFlag flags = 0)
		SCR_BasePreviewEntity previewEntity
		if (stashbox_resource == item_resource)
		{
					previewEntity = SCR_PrefabPreviewEntity.SpawnPreviewFromPrefab(Resource.Load("{6293F5A91DD7001D}Prefabs/preview_save_stash.et"), "{6293F5A91DD7001D}Prefabs/preview_save_stash.et",GetGame().GetWorld(), spawnParams,"{A926DA1095830E3E}Assets/Editor/PlacingPreview/Preview_04.emat");
					placementComponent.SetPreviewEntity(item,previewEntity);
					
					GetGame().GetCallqueue().CallLater(Insert,100,false,item);
		}
		//Create SaveStashBoxComponent, attach to box, call Rpc on that component to hide */ //need a different approach on this. go study their bulding system!!
		
	}
	
	
	void Insert(IEntity item)
	{
		//rpc this to all proxys
		//InsertItem(item);
		item.ClearFlags(EntityFlags.VISIBLE,true);
		item.ClearFlags(EntityFlags.TRACEABLE,true);

		item.SetFlags(EntityFlags.DISABLED,true);
		item.Update();
	}
	void ItemAddedInvocationLocal(IEntity item ,BaseInventoryStorageComponent storageOwner)
	{
		
		Print("InventoryStorageManagerComponent::LocalAddedCall");

		
	}
	void OnItemAddedHandler()
	{
	
		m_OnItemAddedInvoker.Insert(ItemAddedInvocation);
	}
	void OnItemRemovedHandler()
	{
		m_OnItemRemovedInvoker.Insert(ItemRemovedInvocation);
	}
	
	void OnItemRemovedLocalHandler()
	{
		m_OnItemRemovedInvoker.Insert(ItemRemovedInvocationLocal);
	}
	
	void OnItemAddedLocalHandler()
	{
		m_OnItemAddedInvoker.Insert(ItemAddedInvocationLocal);
	}
};