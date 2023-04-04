
class WST_TraderComponentClass : ScriptComponentClass
{


};



class WST_TraderComponent : ScriptComponent
{
	//resets itfself to false after getting
	private bool insertionSuccess;
	
	bool GetInsertionSuccess()
	{
		Print("WST_TraderComponent::GetInsertionSuccess");

		bool ret = insertionSuccess;
		insertionSuccess = false;
		return ret;
	
	}
	void HandleBuyAction(ResourceName n,int balance)
	{
		Rpc(RpcDo_HandleBuyAction,n);
	}
	
	void UpdateWalletValue(int value)
	{
		Rpc(RpcDo_UpdateWalletValue,value);
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcDo_UpdateWalletValue(int value)
	{
		PlayerManager p = GetGame().GetPlayerManager();
		//get RplId, this component is child of the player Character 
		RplComponent Rpl = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		
		if (!Rpl)
			return;
		RplId id = Rpl.Id();
		Print("WST_TraderComponent::RpcDoUpdateWalletValue RplID: " + id);

		
		int playerId = p.GetPlayerIdFromEntityRplId(id);
		
		PlayerController pc = p.GetPlayerController(playerId);
		IEntity ie = pc.GetControlledEntity();
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
		if(wallet)
		{
			wallet.SetValue(value);
		}
	
	}
	
	
	
	
	
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void InsertionCallback(bool success)
	{
		insertionSuccess = success;				
		Print("WST_TraderComponent::InsertionCallbackRPC :" + insertionSuccess);
	//start wallet update from here!!

	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcDo_HandleBuyAction(ResourceName n,int balance)
	{
		
		
		//spawn and insert weapon 
		
		PlayerManager p = GetGame().GetPlayerManager();
		//get RplId, this component is child of the player Character 
		RplComponent Rpl = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		
		if (!Rpl)
			return;
		RplId id = Rpl.Id();
		Print("WST_TraderComponent::RpcDo RplID: " + id);

		
		int playerId = p.GetPlayerIdFromEntityRplId(id);
		
		PlayerController pc = p.GetPlayerController(playerId);
		IEntity ie = pc.GetControlledEntity();
		InventoryStorageManagerComponent storage = InventoryStorageManagerComponent.Cast(ie.FindComponent(InventoryStorageManagerComponent));
		
		
		GenericEntity weaponEnt =  GetGame().SpawnEntityPrefab(Resource.Load(n), GetGame().GetWorld());
		bool success = storage.TryInsertItem(weaponEnt, EStoragePurpose.PURPOSE_ANY,null);
		
		//already on the server so Update wallet right away!
		if(success)
			UpdateWalletValue(balance);
		//Rpc(InsertionCallback,success);
	
	}
	
	
};
