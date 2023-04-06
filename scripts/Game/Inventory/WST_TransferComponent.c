class WST_TransferComponentClass : ScriptComponentClass
{


};



class WST_TransferComponent : ScriptComponent
{

	void HandleCurrencyTransfer(int value ,RplId walletId)
	{
			
		Rpc(RpcDo_HandleCurrencyTransfer,value,walletId);
			
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
		ResourceName r = "{E63CDF1BD7C8CFF6}Prefabs/Items/Equipment/Compass/Money.et";
		
		GenericEntity walletEnt =  GetGame().SpawnEntityPrefab(Resource.Load(r), GetGame().GetWorld());
		MoneyComponent mc = MoneyComponent.Cast(walletEnt.FindComponent(MoneyComponent));
		mc.SetValue(value);
		bool success = storage.TryInsertItem(walletEnt, EStoragePurpose.PURPOSE_ANY,null);
	}
}