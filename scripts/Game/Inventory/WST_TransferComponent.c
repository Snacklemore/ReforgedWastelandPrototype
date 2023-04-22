class WST_TransferComponentClass : ScriptComponentClass
{


};



class WST_TransferComponent : ScriptComponent
{

	void HandleCurrencyTransfer(int value ,RplId walletId,int playerId)
	{
			
		Rpc(RpcDo_HandleCurrencyTransfer,value,walletId,playerId);
		
			
	}
	//hijacked for wallet value set on spawn
	void MoneyConfigSetDelayed()
	{	
		IEntity ie = GetOwner();
		RplComponent rpl = RplComponent.Cast(ie.FindComponent(RplComponent));
		
		if (rpl.IsProxy())
			return;
		int playerId = -1;
		Print("WST_TransferComponent::OnInit" );
		WST_MoneyConfigComponent mcc = WST_MoneyConfigComponent.Cast(GetOwner().FindComponent(WST_MoneyConfigComponent));
		if (!mcc)
			return;
		if(mcc.alreadySet)
			return;
		
		//PlayerManager pm = GetGame().GetPlayerManager();
		//RplId rpl_id = rpl.Id();
		
		
		
		
		InventoryStorageManagerComponent storage = InventoryStorageManagerComponent.Cast(ie.FindComponent(InventoryStorageManagerComponent));

		array<typename> components = {};
		components.Insert(MoneyComponent);
		components.Insert(RplComponent);
		IEntity walletEnity = storage.FindItemWithComponents(components, EStoragePurpose.PURPOSE_DEPOSIT);

		MoneyComponent wallet;
		if(walletEnity)
		{
			wallet = MoneyComponent.Cast(walletEnity.FindComponent(MoneyComponent));
		}
		if(!wallet)
			return;
		Rpc(Rpc_OnBroadcastValueUpdatedOwner,mcc.GetValue());
		wallet.SetValue(mcc.GetValue());
	}
	override event void OnPostInit(IEntity owner)
	{
		GetGame().GetCallqueue().CallLater(MoneyConfigSetDelayed,50,false);
		hideMapDescriptorComponentOnProxy();
		
	}
	
	//hijacked to hide map descriptor if entity is ProximityTriggerComponent
	
	void hideMapDescriptorComponentOnProxy()
	{
		
		PlayerController controller = GetGame().GetPlayerController();
		IEntity controlled
		if(controller)
			controlled =  controller.GetControlledEntity();
		SCR_MapDescriptorComponent d
		if(controlled)
			d = SCR_MapDescriptorComponent.Cast(controlled.FindComponent(SCR_MapDescriptorComponent));
		Print("Hiding Descriptor in hijacked function on proxy");
		if(!d)
			return;
		MapItem item =  d.Item();
		if(item)
			item.SetVisible(true);	
	
			
			
		
	}
	
	
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void Rpc_OnBroadcastValueUpdatedOwner(int value)//value is current balance
	{
		// this method will only run on proxies if authority's RpcConditionMethod returns true
		/*Print("TransferBroadcastOwner::OnBroadcastValueUpdated::: " );
		PlayerController pc = GetGame().GetPlayerController();
		if (!pc)
			return;
		int playeridlocal = pc.GetPlayerId();
		
		if(playerId != playeridlocal)
			return;
		IEntity controlled = pc.GetControlledEntity();
		if(!controlled)
			return;
		RplComponent rpl = RplComponent.Cast(controlled.FindComponent(RplComponent));
		
		if(!rpl)
			return;*/
		SCR_HintManagerComponent.ShowCustomHint("Wallet update. New Balance: "+ value ,"Wallet Info",5.0,false,EFieldManualEntryId.NONE,false);
		
		


	}
	void CreateWallet(int value, RplId playerId)
	{
		
		Rpc(RpcDo_CreateWallet,value,playerId);
	}
	
	
	//dont call hint here (call where this Rpc is called)!
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcDo_HandleCurrencyTransfer(int value ,RplId walletId,int playerId)
	{
		
		Print("WST_TransferComponent::RpcDoHandleCurrencyTransfer wallet RplID: " + walletId + "and value to transfer: " + value );
		RplComponent rpl = RplComponent.Cast(Replication.FindItem(walletId));
		IEntity ie = rpl.GetEntity();
		
		
		MoneyComponent comp = MoneyComponent.Cast(ie.FindComponent(MoneyComponent));
		
		Print("WST_TransferComponent::RpcDoHandleCurrencyTransfer wallet RplID: " + walletId + " has value:  " + value );
		Rpc(Rpc_OnBroadcastValueUpdatedOwner,value);
		comp.SetValue(value);
		
		

		
	
	}
	
	//call hint when wallet created (on client) and when money WST_TransferCurrencyWindow
	//dont call hint here (call where this Rpc is called)!

	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcDo_CreateWallet(int value,RplId playerId)
	{
		//spawn and insert wallet 
		
		PlayerManager p = GetGame().GetPlayerManager();
		//player rpl component
		RplComponent Rpl  = Replication.FindItem(playerId);
		
		
		if (!Rpl)
			return;
		RplId id = Rpl.Id();
		Print("WST_TransferComponent::RpcDo_CreateWallet Player RplID: " + id);
		Print("WST_TransferComponent::RpcDo_CreateWallet with value: "+ value);

		
		int iplayerId = p.GetPlayerIdFromEntityRplId(id);
		
		PlayerController pc = p.GetPlayerController(iplayerId);
		if(!pc)
			return;
		IEntity ie = pc.GetControlledEntity();
		InventoryStorageManagerComponent storage = InventoryStorageManagerComponent.Cast(ie.FindComponent(InventoryStorageManagerComponent));
		ResourceName r = "{50E496D7030957C1}Prefabs/Props/Commercial/cash/cash.et";
		
		GenericEntity walletEnt =  GetGame().SpawnEntityPrefab(Resource.Load(r), GetGame().GetWorld());
		MoneyComponent mc = MoneyComponent.Cast(walletEnt.FindComponent(MoneyComponent));
		Rpc(Rpc_OnBroadcastValueUpdatedOwner,value);
		mc.SetValue(value);
		bool success = storage.TryInsertItem(walletEnt, EStoragePurpose.PURPOSE_ANY,null);
	}
}