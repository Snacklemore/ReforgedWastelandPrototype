
modded class SCR_InventoryStorageManagerComponent : ScriptedInventoryStorageManagerComponent
{
			
	MoneyComponent m_MoneyComponent;
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
	
	void ItemRemovedInvocation(IEntity item ,BaseInventoryStorageComponent storageOwner)
	{
		auto moneyComp = MoneyComponent.Cast(item.FindComponent(MoneyComponent));
		if(moneyComp )
		{
			//set to null if last wallet
			if(m_MoneyComponent == moneyComp )
				m_MoneyComponent = null;
		}
	}
	void OnItemAddedHandler()
	{
	
		m_OnItemAddedInvoker.Insert(ItemAddedInvocation);
	}
	void OnItemRemovedHandler()
	{
		m_OnItemRemovedInvoker.Insert(ItemRemovedInvocation);
	}
	
	
	
};