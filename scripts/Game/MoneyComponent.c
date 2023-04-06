[EntityEditorProps(category: "Game/System", description: "Component attached to compositions.")]
class MoneyComponentClass : ScriptComponentClass
{
};


class floatext
{
	void floatext()
	{
		if(!this)
		{
			
		}
	}
	
	float val;
}
//------------------------------------------------------------------------------------------------
class MoneyComponent : ScriptComponent
{
	protected RplId m_RplCompId;
	
	bool valueChanged;
	
	[RplProp(onRplName: "OnBroadcastValueUpdated", condition: RplCondition.Custom, customConditionName: "RpcConditionMethod")]
	int m_iValue;
	
	SCR_MoneyManager m_MoneyManager; 
	override void OnPostInit(IEntity owner)
	{
		
	
	}
	
	
	void OnBroadcastValueUpdated()
	{
		// this method will only run on proxies if authority's RpcConditionMethod returns true
		Print("MoneyComponent::OnBroadcastValueUpdated::: " );

	}
	
	void Unregister(bool shouldDelete)
	{
		if (shouldDelete)
		{
			m_MoneyManager.Unregister(this);
			Rpc(RpcAsk_SelfDelete);
			return;
		}

		
		
		
	}
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcAsk_SelfDelete()
	{
		RplComponent rplComp = RplComponent.Cast(Replication.FindItem(m_RplCompId));
		if (!rplComp)
		            return;
		IEntity entity = rplComp.GetEntity();
		if (!entity)
			        return;
		Print("MoneyComponent::RpcAsk_SelfDelete::: " );
		
		RplComponent.DeleteRplEntity(entity, false);	
	}

	bool RpcConditionMethod()
	{
		if (valueChanged)
		{
			valueChanged = false;
			return true;
		
		}
		return false;
	}
	
	override void EOnInit(IEntity owner)
	{
		//set RplId
		RplComponent rpl =  RplComponent.Cast(owner.FindComponent(RplComponent));

		
		if (!rpl.IsMaster() && rpl.IsProxy())
			return;
		GetGame().GetCallqueue().CallLater(SetRplID,1000,false);
		SCR_BaseGameModeWasteland mode;
		mode = SCR_BaseGameModeWasteland.Cast(GetGame().GetGameMode());
		if(mode)
		{
			m_MoneyManager = mode.GetMoneyManager();
			m_MoneyManager.register(this);
		}
		
	
	}
	
	RplId GetRplId()
	{
		return m_RplCompId;
	}
	
	
	int GetValueForWallet()
	{
		//request last value stored on server
		
	}
	
	
	
	
	void MoneyComponent()
	{
		
		SetEventMask(GetOwner(),EntityEvent.INIT);
		m_iValue = 50;
		valueChanged = false;
	}
	
	bool add()
	{
	
	}
	
	int GetValue()
	{
		return m_iValue;
	}
	
	bool SetValue(int val)
	{
		m_iValue = val;
		Replication.BumpMe();
		
		if(m_iValue <= 0)
		{
			 Unregister(true);
		}
		valueChanged = true;
		return true;
	}
	bool subtract()
	{
	
	}
	void SetRplID()
	{
		
		m_RplCompId = RplId.Invalid();
		RplComponent rpl =  RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		if (!rpl.IsMaster() && rpl.IsProxy())
			return;
		if (rpl)
		{
			m_RplCompId = rpl.Id();
					Print("MoneyComponent::SetRplID::RplID of : " + rpl.Id());

		}
	}
	
	//------------------------------------------------------------------------------------------------
	override bool RplSave(ScriptBitWriter writer)
	{
		if (!m_RplCompId)
		{
			writer.WriteBool(0);
			return true;
		}
	
		RplComponent rplComp = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
	
		writer.WriteBool(1);
		writer.WriteRplId(rplComp.Id());
		Print("MoneyComponent::RplSave::RplID of : " + rplComp.Id());

		writer.WriteInt(m_iValue);
		Print("MoneyComponent::RplSave::m_iValue of : " + m_iValue);

		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool RplLoad(ScriptBitReader reader)
	{
		bool rplExist;
		reader.ReadBool(rplExist);
		if (!rplExist)
			return true;
		
		reader.ReadRplId(m_RplCompId);
		RplComponent rplComp = RplComponent.Cast(Replication.FindItem(m_RplCompId));
		if (!rplComp)
		{
			return false;
		}
		
		
		m_RplCompId = rplComp.Id();
		Print("MoneyComponent::RplLoad::RplID of : " + rplComp.Id());

		reader.ReadInt(m_iValue);
		Print("MoneyComponent::RplLoad::m_iValue of : " + m_iValue);
		return true;
	}
};