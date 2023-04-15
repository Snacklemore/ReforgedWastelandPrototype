class WST_GearPersistenceManagerClass: GenericEntityClass
{
};
//session Persistence (wont survice a server restart!)
//keep manager on server side Only 
//1.new player 
//-

//2.already played before 
//-
//Rpc for client to ask for save needed
class WST_GearPersistenceManager: GenericEntity
{
	ref array<ref WST_GearObject> PlayerGearObjects;
	
	//playeridentitys
	ref array<string> connectedPlayers;
	RplComponent mRpl;
	void register()
	{
		SCR_BaseGameModeWasteland mode =  SCR_BaseGameModeWasteland.Cast(GetGame().GetGameMode());
		mode.SetGearPersistenceManager(this);
	}
	override void EOnInit(IEntity owner)
	{
		RplComponent rpl_c = RplComponent.Cast(FindComponent(RplComponent));
		if (!rpl_c)
		{
			Print("WST_GearPersistenceManager::init no RplComponent!!");
			return;
		}
		mRpl = rpl_c;
		GetGame().GetCallqueue().CallLater(UpdateConnectedPlayersList,10000,true);
		PlayerGearObjects  = new ref array<ref WST_GearObject>();

		register();
	}
	
	ref WST_GearObject GetGearObject(string UID)
	{
		foreach(ref WST_GearObject o: PlayerGearObjects)
		{
			if (o.PlayerIdentity == UID)
			{
				return o;
			}
		}
		return null;
	}
	
	//fill GearObject with currently stored/worn equipment
	void Save(int id)
	{
		ref WST_GearObject gearObject_l = new ref WST_GearObject();
		Print("WST_GearPersistenceManager::Save:: PlayerGearObjects count: " +PlayerGearObjects.Count());
		Print("WST_GearPersistenceManager::Save::RPC UID " +WST_GearPersistenceManager.GetPlayerIdentity(id));

		foreach(ref WST_GearObject o : PlayerGearObjects)
		{
			Print("WST_GearPersistenceManager::Save::UID::  " +o.PlayerIdentity);

			if(o.PlayerIdentity == WST_GearPersistenceManager.GetPlayerIdentity(id))
			{
				gearObject_l = o;
				break;
			}
		
		}
		if (gearObject_l.PlayerIdentity.IsEmpty())
		{
			Print("WST_GearPersistenceManager::no gear object created ERROR abort");
			return;

		}

		//get all items of player with UID 
		
		//get InventoryStorageManagerComponent
		
			//int GetAllItems(inout array<IEntity> items, BaseInventoryStorageComponent storage)
			//proto external int GetStorages(out notnull array<BaseInventoryStorageComponent> outStorages, EStoragePurpose purpose = EStoragePurpose.PURPOSE_ANY);
		
		PlayerManager pm = GetGame().GetPlayerManager();
		
		IEntity controller = pm.GetPlayerControlledEntity(id);
		
		SCR_InventoryStorageManagerComponent inv_mm = SCR_InventoryStorageManagerComponent.Cast(controller.FindComponent(SCR_InventoryStorageManagerComponent));
		
		array<BaseInventoryStorageComponent> availableStorages = new array<BaseInventoryStorageComponent>();
		int storageCount = inv_mm.GetStorages(availableStorages);
		Print("WST_GearPersistenceManager::Save:: storage count: "+ storageCount);
		
		int itemCount = -1;
		array<IEntity> items = new array<IEntity>();
		
		
		int count = 0;
		foreach(BaseInventoryStorageComponent storage : availableStorages)
		{
			count++;
			itemCount = inv_mm.GetAllItems(items,storage);		
			Print("WST_GearPersistenceManager::Save:: item count: "+ itemCount+" pass "+ count);
		}
		
		Print("WST_GearPersistenceManager::Save::All items count: "+ items.Count());
		foreach(IEntity item :items)
		{
			EntityPrefabData data = item.GetPrefabData();
			//Check for duplicates before adding 
			
			if (gearObject_l.itemsOnPlayer.Contains(data))
				continue;
			gearObject_l.add(data);
			Print("WST_GearPersistenceManager::Saved item. ResourceName: "+ data.GetPrefabName());

		}
		
	}
	//do not update all player at once
	//send update via RPC, and update Data 
	//update send when player opening PauseMenuUI
	
	//update connected players list 
	//call like every 10s
	void UpdateConnectedPlayersList()
	{
	 	//only on Server 
		if (!mRpl.IsMaster())
			return;
		Print("WST_GearPersistenceManager::UpdateConnectedPlayersList");
		BackendApi api = GetGame().GetBackendApi();
		Print("WST_GearPersistenceManager::UpdateConnectedPlayersList auth: "+api.IsAuthenticated());
		Print("WST_GearPersistenceManager::UpdateConnectedPlayersList auth: "+api.GetPlayerUID(1));
		
		PlayerManager pm = GetGame().GetPlayerManager();
		int connectedPlayerCount = pm.GetPlayerCount();
		
		array<int> playerIds = new array<int>(); 
		pm.GetPlayers(playerIds);
		
		int count = playerIds.Count();
		Print("WST_GearPersistenceManager::UpdateConnectedPlayersList playerCount: "+count);

		foreach (int id : playerIds)
		{
			ref WST_GearObject gearObject_l = new ref WST_GearObject();
			string identity = GetPlayerIdentity(id);
			
			
			
			gearObject_l.PlayerIdentity = identity;
			
			
			
			IEntity controlled = pm.GetPlayerControlledEntity(id);
			if(!controlled)
				continue;
			RplComponent rpl = RplComponent.Cast(controlled.FindComponent(RplComponent));
			
			//PlayerGearObjects.Insert(gearObject_l);
			InsertToPlayerGearObjects(gearObject_l);
			Print("WST_GearPersistenceManager::UpdateConnectedPlayersList::added id : "+ identity);

		}
		
	}
	
	//add if not existing
	
	void InsertToPlayerGearObjects(WST_GearObject o)
	{
		bool found = false;
		ref WST_GearObject obh;
		foreach(ref WST_GearObject ob : PlayerGearObjects)
		{
			if (ob.PlayerIdentity == o.PlayerIdentity)
			{
				found = true;
				break;
			}
		}
		if (found)
		{
			return;
		}
		else
			PlayerGearObjects.Insert(o);

	}
	static const string LOCAL_PLAYER_IDENTITY_ID = "identitySP";
	static string GetPlayerIdentity(int playerId)
	{
		if (playerId <= 0)
			return string.Empty;
		
		if (RplSession.Mode() == RplMode.None)
			return LOCAL_PLAYER_IDENTITY_ID;
		
		BackendApi api = GetGame().GetBackendApi();
		
		if (!api)
			return string.Empty;
		
		return api.GetPlayerUID(playerId);
	}
	
	
	
	void WST_GearPersistenceManager(IEntitySource src, IEntity parent)
	{
		

		SetEventMask(EntityEvent.INIT);
	}
	

}

class WST_GearObject 
{
	//array of EntityPrefabData 
	ref array<ref EntityPrefabData> itemsOnPlayer;
	string PlayerIdentity;
	//with this PlayerIdentity associated RplID 
	
	void add(EntityPrefabData e)
	{
		itemsOnPlayer.Insert(e);
	}
	
	
	
	void WST_GearObject()
	{
		itemsOnPlayer = new ref array<ref EntityPrefabData>();
		
	}

}