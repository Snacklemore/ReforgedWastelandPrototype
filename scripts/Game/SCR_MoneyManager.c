
class SCR_MoneyManagerClass: GenericEntityClass
{
};


//Register Manager in GameMode 
//----->implement GameMode Global getter for MoneyManager Singleton 
//---->get MoneyManager inside MoneyComponent 

class SCR_MoneyManager: GenericEntity
{
	
	ref array<GenericEntity> wallets;
	RplComponent m_rpl;
	
	override void EOnInit(IEntity owner)
	{
		m_rpl = RplComponent.Cast(FindComponent(RplComponent));
		if(!m_rpl.IsMaster())
			return;
		wallets = new array<GenericEntity>();
		
		
		SCR_BaseGameModeWasteland mode = SCR_BaseGameModeWasteland.Cast(GetGame().GetGameMode());
		Print("SCR_MoneyManager::OnInit::RegisterManager");
		mode.SetMoneyManager(this);
		GetGame().GetCallqueue().CallLater(Cleanup,20000,true);
		
	}
	void Cleanup()
	{
		if(!m_rpl.IsMaster())
			return;
		int counter = 0;
		int removedNullpointer = 0;
		Print("SCR_MoneyManager::Cleanup Called!");

		foreach (GenericEntity ent : wallets)
		{
			//too late for null check?
			if(ent)
				continue;
			if(!ent)
			{
				wallets.Remove(counter);
				removedNullpointer++;
				
			}
			
			
			counter++;
		}
		Print("SCR_MoneyManager::Cleanup finished, removed nullptrs: "+  removedNullpointer);

	}
	void Unregister(MoneyComponent comp)
	{
		if(!m_rpl.IsMaster())
			return;
		RplId id = comp.GetRplId();
		Print("SCR_MoneyManager::Unregister::amount of registered wallets: "+  wallets.Count());

		foreach (GenericEntity ent : wallets)
		{
			if(!ent)
				return;
			MoneyComponent moneyComp = ent.FindComponent(MoneyComponent);
			if (!moneyComp)
				continue;			
			if (moneyComp.GetRplId() == id)
			{
				wallets.RemoveItem(ent);
				return;
			}
				
		}	
		
		
		return;
	
	}
	MoneyComponent GetWallet(RplId id)
	{
		Print("SCR_MoneyManager::GetWallet::amount of registered wallets: "+  wallets.Count());

		if(!m_rpl.IsMaster())
			return null;
		foreach (GenericEntity ent : wallets)
		{
			MoneyComponent moneyComp = ent.FindComponent(MoneyComponent);
			if (!moneyComp)
				return null;			
			if (moneyComp.GetRplId() == id)
				return moneyComp;
		}	
		
		return null;	
	}
	//serveronly
	int GetWalletValue(RplId id)
	{
		if(!m_rpl.IsMaster())
			return -1;
		foreach (GenericEntity ent : wallets)
		{
			MoneyComponent moneyComp = ent.FindComponent(MoneyComponent);
			if (!moneyComp)
				return -1;
			
			if (moneyComp.GetRplId() == id)
				return moneyComp.GetValue();
		}	
	
		return -1;
	}
	
	bool SetWalletValue(RplId id,int val)
	{
		if(!m_rpl.IsMaster())
			return false;
		foreach (GenericEntity ent : wallets)
		{
			MoneyComponent moneyComp = ent.FindComponent(MoneyComponent);
			if (!moneyComp)
				return false;
			
			if (moneyComp.GetRplId() == id)
				moneyComp.SetValue(val);
		}	
	
		return 1;
	}
	
	
	bool register(MoneyComponent comp)
	{
		wallets.Insert(comp.GetOwner());
		Print("SCR_MoneyManager::register::inserted walletEntity w/ val: " + comp.GetValue());
		Print("SCR_MoneyManager::register::amount of registered wallets: "+  wallets.Count());

		return true;
	}
	
	void SCR_MoneyManager(IEntitySource src, IEntity parent)
	{
		#ifdef WORKBENCH
			SetFlags(EntityFlags.TRACEABLE, false);
		#else
			SetFlags(EntityFlags.NO_LINK, false);
		#endif
		//if(!m_rpl.IsMaster())
		//	return;
		SetEventMask(EntityEvent.INIT);
	}
	
};























