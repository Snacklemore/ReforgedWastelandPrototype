class WST_TransferComponentClass : ScriptComponentClass
{


};



class WST_TransferComponent : ScriptComponent
{

	void HandleCurrencyTransfer(int value ,RplId walletId)
	{
			
		Rpc(RpcDo_HandleCurrencyTransfer,value,walletId);
			
	}
	//hijacked for wallet value set on spawn
	void MoneyConfigSetDelayed()
	{
		Print("WST_TransferComponent::OnInit" );
		WST_MoneyConfigComponent mcc = WST_MoneyConfigComponent.Cast(GetOwner().FindComponent(WST_MoneyConfigComponent));
		if (!mcc)
			return;
		if(mcc.alreadySet)
			return;
		IEntity ie = GetOwner();
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
		MapItem item =  d.Item();
		if(item)
			item.SetVisible(true);	
	
			
			
		
	}
	
	void CreateWallet(int value, RplId playerId)
	{
		Rpc(RpcDo_CreateWallet,value,playerId);
	}
	
	
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcDo_HandleCurrencyTransfer(int value ,RplId walletId)
	{
		
		Print("WST_TransferComponent::RpcDoHandleCurrencyTransfer wallet RplID: " + walletId + "and value to transfer: " + value );
		RplComponent rpl = RplComponent.Cast(Replication.FindItem(walletId));
		IEntity ie = rpl.GetEntity();
		
		
		MoneyComponent comp = MoneyComponent.Cast(ie.FindComponent(MoneyComponent));
		
		Print("WST_TransferComponent::RpcDoHandleCurrencyTransfer wallet RplID: " + walletId + " has value:  " + value );
		
		comp.SetValue(value);
		
		

		
	
	}
	
	
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
		mc.SetValue(value);
		bool success = storage.TryInsertItem(walletEnt, EStoragePurpose.PURPOSE_ANY,null);
	}
}