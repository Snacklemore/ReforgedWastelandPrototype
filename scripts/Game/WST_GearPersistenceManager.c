class WST_GearPersistenceManagerClass: GenericEntityClass
{
};
///////TODO:
//-DeleteAllItems not deleting all items 

//-only scan first storage for items (contains all items already probably)

//-clear the itemsOnPlayer array before saving new items!
//-save currency value on player

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
		array<IEntity> items2 = new array<IEntity>();

		array<IEntity> items3 = new array<IEntity>();

		array<IEntity> items4 = new array<IEntity>();

		array<IEntity> items5 = new array<IEntity>();

		array<IEntity> items6 = new array<IEntity>();
		array<IEntity> items7 = new array<IEntity>();
		array<IEntity> items8 = new array<IEntity>();
		array<IEntity> items9 = new array<IEntity>();
		array<IEntity> items10 = new array<IEntity>();

		array<IEntity> items = new array<IEntity>();
		array<IEntity> itemsWithWeapons = new array<IEntity>();
		array<IEntity> itemWeapons= new array<IEntity>();
		

		PlayerManager pm = GetGame().GetPlayerManager();
		
		IEntity controller = pm.GetPlayerControlledEntity(id);
		
		SCR_InventoryStorageManagerComponent inv_mm = SCR_InventoryStorageManagerComponent.Cast(controller.FindComponent(SCR_InventoryStorageManagerComponent));
		
		array<BaseInventoryStorageComponent> availableStorages = new array<BaseInventoryStorageComponent>();
		//amount of storages dependant on character equipment 
		int storageCount = inv_mm.GetStorages(availableStorages);
		Print("WST_GearPersistenceManager::Save:: storage count: "+ storageCount);
		SCR_InventoryStorageManagerComponent mmc = inv_mm;
		BaseInventoryStorageComponent b_characterStorage;
		BaseInventoryStorageComponent b_EquipedWeaponStorageStorage;
		foreach(BaseInventoryStorageComponent bStorage : availableStorages)
		{
			EquipedWeaponStorageComponent ws = EquipedWeaponStorageComponent.Cast(bStorage);
			if (ws)
				b_EquipedWeaponStorageStorage = ws;
			SCR_CharacterInventoryStorageComponent cs = SCR_CharacterInventoryStorageComponent.Cast(bStorage);
			if (cs)
				b_characterStorage = cs;	
		}
		
		
		
		mmc.GetAllItems(items,availableStorages[0]);
		mmc.GetAllItems(items2,availableStorages[1]);
		mmc.GetAllItems(items3,availableStorages[2]);
		mmc.GetAllItems(items4,availableStorages[3]);
		mmc.GetAllItems(items5,availableStorages[4]);
		mmc.GetAllItems(items6,availableStorages[5]);
		mmc.GetAllItems(items7,availableStorages[6]);
		mmc.GetAllItems(items8,availableStorages[7]);
		mmc.GetAllItems(items9,availableStorages[8]);
		mmc.GetAllItems(items10,availableStorages[9]);

		Print("GameModeWasteland::item data start");
		foreach(IEntity it : items)
		{
			Print("GameModeWasteland::item data" +it.GetPrefabData().GetPrefabName());
		}
		
		Print("GameModeWasteland::item data start2");
		foreach(IEntity it : items2)
		{
			Print("GameModeWasteland::item data2" +it.GetPrefabData().GetPrefabName());
		}
		
		Print("GameModeWasteland::item data start3");
		foreach(IEntity it : items3)
		{
			Print("GameModeWasteland::item data3" +it.GetPrefabData().GetPrefabName());
		}		
		Print("GameModeWasteland::item data start4");
		foreach(IEntity it : items4)
		{
			Print("GameModeWasteland::item data4" +it.GetPrefabData().GetPrefabName());
		}		
		Print("GameModeWasteland::item data start5");
		foreach(IEntity it : items5)
		{
			Print("GameModeWasteland::item data5" +it.GetPrefabData().GetPrefabName());
		}		
		Print("GameModeWasteland::item data start6");
		foreach(IEntity it : items6)
		{
			Print("GameModeWasteland::item data6" +it.GetPrefabData().GetPrefabName());
		}		
		Print("GameModeWasteland::item data start7");
		foreach(IEntity it : items7)
		{
			Print("GameModeWasteland::item data7" +it.GetPrefabData().GetPrefabName());
		}		
		Print("GameModeWasteland::item data start8");
		foreach(IEntity it : items8)
		{
			Print("GameModeWasteland::item data8" +it.GetPrefabData().GetPrefabName());
		}		
		Print("GameModeWasteland::item data start9");
		foreach(IEntity it : items9)
		{
			Print("GameModeWasteland::item data9" +it.GetPrefabData().GetPrefabName());
		}		
		Print("GameModeWasteland::item data start10");
		foreach(IEntity it : items10)
		{
			Print("GameModeWasteland::item data10" +it.GetPrefabData().GetPrefabName());
		}		
		int itemCount = -1;
		
		
		items.Clear();
		int count = 0;
		
		//characterinventory
		itemCount = inv_mm.GetAllItems(items,b_characterStorage);	
		int max = items.Count();
		
			/*for (int i = 0;i < max ;i++)
			{
				IEntity e = items.Get(i);
				InventoryMagazineComponent mag = InventoryMagazineComponent.Cast(e.FindComponent(InventoryMagazineComponent));
				if (mag)
				{
					items.Remove(items.Find(e));
					max--;
				}
			}*/
		
		
		
		//weaponsattachmentsstorage
		itemCount = inv_mm.GetAllItems(items,b_EquipedWeaponStorageStorage);
			//itemCount = inv_mm.GetItems(itemsWithWeapons);
			
		
			
			/*
			foreach(IEntity e : itemsWithWeapons)
			{
				WeaponComponent wpc = WeaponComponent.Cast(e.FindComponent(WeaponComponent));
				if(wpc)
				{
					itemWeapons.Insert(e);
				}
				//InventoryMagazineComponent mgc = InventoryMagazineComponent.Cast(e.FindComponent(InventoryMagazineComponent));
				//if(mgc)
			//	{
					//itemWeapons.Insert(e);
				//}
			}
			foreach(IEntity e : itemWeapons)
			{
				Print("WST_GearPersistenceManager::ItemWeapons: "+ e.GetPrefabData().GetPrefabName());
				items.Insert(e);
			}
			
			*/
		
		
		Print("WST_GearPersistenceManager::Save::All items count: "+ items.Count());
		//clear prefab data 
		gearObject_l.itemsOnPlayer.Clear();
		
		foreach(IEntity item :items)
		{
			if(!item)
				continue;
			EntityPrefabData data = item.GetPrefabData();
			
			gearObject_l.add(data);
			Print("WST_GearPersistenceManager::Saved item. ResourceName: "+ data.GetPrefabName());

		}
		
	}
	
	bool isWeaponAttachment(IEntity e)
	{
		InventoryItemComponent itemComp = InventoryItemComponent.Cast(e.FindComponent(InventoryItemComponent));

		if (!itemComp)
			return false;

		ItemAttributeCollection itemAttributes = itemComp.GetAttributes();

		if (!itemAttributes)
			return false;

		WeaponAttachmentAttributes itemAttribute = WeaponAttachmentAttributes.Cast(itemAttributes.FindAttribute(WeaponAttachmentAttributes));

		if (!itemAttribute)
			return false;
		else 
			return true;
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
	int walletValue;
	//with this PlayerIdentity associated RplID 
	
	void add(EntityPrefabData e)
	{
		itemsOnPlayer.Insert(e);
	}
	
	
	
	void WST_GearObject()
	{
		itemsOnPlayer = new ref array<ref EntityPrefabData>();
		walletValue = -1;
		
	}

}