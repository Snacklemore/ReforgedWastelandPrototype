
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
	
	void CloseAllMenus()
	{
		Rpc(RpcDo_CloseAllMenus);
	
	}
	void HandleVehicleBuyAction(ResourceName n,int balance,RplId shopId)
	{
		Rpc(RpcDo_HandleVehicleBuyAction,n,balance,shopId);
	}
	void HandleBuyAction(ResourceName n,int balance)
	{
		Rpc(RpcDo_HandleBuyAction,n,balance);
	}
	
	void UpdateWalletValue(int value)
	{
		Rpc(RpcDo_UpdateWalletValue,value);
	}

	
	
	
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void RpcDo_CloseAllMenus()
	{
		GetGame().GetMenuManager().CloseAllMenus();
	
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
	
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcDo_HandleVehicleBuyAction(ResourceName n,int balance,RplId shopId)
	{
		
		
		//spawn and insert weapon 
		
		//get mode 
		SCR_BaseGameModeWasteland mode = SCR_BaseGameModeWasteland.Cast( GetGame().GetGameMode());
		
		//get vehiclespawnpoint array
		//array<WST_VehicleSpawnPoint> vs_a = mode.m_vehicleSpawnPoints;
		//get WST_VehicleShopPointInfoComponent 
		RplComponent shop = Replication.FindItem(shopId);
		IEntity owner = shop.GetEntity();
		WST_VehicleShopPointInfoComponent spawn = WST_VehicleShopPointInfoComponent.Cast(owner.FindComponent(WST_VehicleShopPointInfoComponent));
		vector mat[4] = {};
		spawn.m_Position.GetModelTransform(mat);
		
		
		
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		spawn.m_Position.GetWorldTransform(params.Transform);
		params.Transform[3] = spawn.GetOwner().CoordToParent(params.Transform[3]);
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
		
		
		
		
		
		
		
		Vehicle v = Vehicle.Cast(GetGame().SpawnEntityPrefab(Resource.Load(n), GetGame().GetWorld(), params));
		bool isInserted = GetGame().GetGarbageManager().Insert(v);
		//already on the server so Update wallet right away!
		if(v)
			UpdateWalletValue(balance);
		//Rpc(InsertionCallback,success);
	
	}
	
	void LoadBoughtSupplies(RplId suppliesId,int suppliesAmount,int pricePerSupplyPoint)
	{
		Rpc(RpcDo_LoadBoughtSupplies,suppliesId,suppliesAmount,pricePerSupplyPoint);
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]	
	void RpcDo_SupplyBuyInfoHint(string msg)
	{
		SCR_HintManagerComponent.ShowCustomHint(msg,5.0,false,EFieldManualEntryId.NONE,false);

	}

	[RplRpc(RplChannel.Reliable, RplRcver.Server)]	
	void RpcDo_LoadBoughtSupplies(RplId suppliesId,int suppliesAmount,int pricePerSupplyPoint)
	{
	
		SCR_CampaignSuppliesComponent suppliesComponent = SCR_CampaignSuppliesComponent.Cast(Replication.FindItem(suppliesId));
		
		if (!suppliesComponent)
			return;
		
		int overAllCost = suppliesAmount * pricePerSupplyPoint;
		
		PlayerManager p = GetGame().GetPlayerManager();
		//get RplId, this component is child of the player Character 
		RplComponent Rpl = RplComponent.Cast(GetOwner().FindComponent(RplComponent));
		
		if (!Rpl)
			return;
		RplId id = Rpl.Id();
		Print("WST_TraderComponent::RpcDoLoadBoughtValue Player RplID: " + id);

		
		int playerId = p.GetPlayerIdFromEntityRplId(id);
		
		PlayerController pc = p.GetPlayerController(playerId);
		IEntity ie = pc.GetControlledEntity();
		InventoryStorageManagerComponent storage = InventoryStorageManagerComponent.Cast(ie.FindComponent(InventoryStorageManagerComponent));
		
		
		array<typename> components = {};
		components.Insert(MoneyComponent);
		components.Insert(RplComponent);
		IEntity walletEnity = storage.FindItemWithComponents(components, EStoragePurpose.PURPOSE_DEPOSIT);
		int walletBalance = -1;
		MoneyComponent wallet;
		if(walletEnity)
		{
			wallet = MoneyComponent.Cast(walletEnity.FindComponent(MoneyComponent));

		}else 
		{
			Print("no cash");
			string msg = "No Wallet!";
			Rpc(RpcDo_SupplyBuyInfoHint,msg);
			return;
		}
		if(wallet)
		{
			walletBalance = wallet.GetValue();
			
		}
		if(wallet.GetValue() < 0)
			return;
		
		if (overAllCost > walletBalance)
		{
			Print("no cash");
			string msg = "No Cash!";
			Rpc(RpcDo_SupplyBuyInfoHint,msg);
			return;
		}
			
		
		int newWalletBalance = walletBalance - overAllCost;
		int maxSupplies = suppliesComponent.GetSuppliesMax();
		if (suppliesComponent.GetSupplies() == maxSupplies)
		{
			Print("already full");
			string msg = "Nothing bought, full already!";
			Rpc(RpcDo_SupplyBuyInfoHint,msg);
			return;
		}
		
		
		RpcDo_UpdateWalletValue(newWalletBalance);
		suppliesComponent.AddSupplies(suppliesAmount);
		string msg = "Bought " +suppliesAmount+ " of supplies for " + overAllCost +"$";
		Rpc(RpcDo_SupplyBuyInfoHint,msg);
		
	}
	
};
