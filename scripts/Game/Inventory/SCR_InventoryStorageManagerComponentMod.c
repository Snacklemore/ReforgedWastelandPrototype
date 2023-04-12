
modded class SCR_InventoryStorageManagerComponent : ScriptedInventoryStorageManagerComponent
{
			
	MoneyComponent m_MoneyComponent;
	

	// Callback when item is added (will be performed locally after server completed the Insert/Move operation)
	override protected void OnItemAdded(BaseInventoryStorageComponent storageOwner, IEntity item)
	{		
		//super.OnItemAdded(storageOwner, item);
		auto consumable = SCR_ConsumableItemComponent.Cast(item.FindComponent(SCR_ConsumableItemComponent));
		
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
		
		if ( consumable && consumable.GetConsumableType() == EConsumableType.Bandage )
			m_iHealthEquipment++;	//store count of the health components
				
		//dont invoke if deleted
		if ( m_OnItemAddedInvoker )
			m_OnItemAddedInvoker.Invoke( item, storageOwner );
		
		
		// Withdraw item from gc collection
		GarbageManager garbageManager = GetGame().GetGarbageManager();
		if (garbageManager)
		{
			if (item.FindComponent(InventoryItemComponent))
				garbageManager.Withdraw(item);
		}
		
	}
	
	// Callback when item is removed (will be performed locally after server completed the Remove/Move operation)
	override protected void OnItemRemoved(BaseInventoryStorageComponent storageOwner, IEntity item)
	{
		super.OnItemRemoved(storageOwner, item);
		
		auto consumable = SCR_ConsumableItemComponent.Cast(item.FindComponent(SCR_ConsumableItemComponent));
		auto moneyComp = MoneyComponent.Cast(item.FindComponent(MoneyComponent));
		if(moneyComp )
		{
			//set to null if last wallet
			if(m_MoneyComponent == moneyComp )
				m_MoneyComponent = null;
		}
		if ( consumable && consumable.GetConsumableType() == EConsumableType.Bandage )
			m_iHealthEquipment--;	//store count of the health components
		
		if ( m_OnItemRemovedInvoker )
			m_OnItemRemovedInvoker.Invoke( item, storageOwner );

		// Insert item into gc collection
		GarbageManager garbageManager = GetGame().GetGarbageManager();
		if (garbageManager)
		{
			if (item.FindComponent(InventoryItemComponent))
				garbageManager.Insert(item);
		}
	}
	
};