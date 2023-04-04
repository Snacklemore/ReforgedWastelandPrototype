
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
	
		return 1;
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























