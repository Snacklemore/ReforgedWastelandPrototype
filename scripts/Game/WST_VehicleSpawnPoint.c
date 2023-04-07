class WST_VehicleSpawnPointClass : SCR_PositionClass
{

};

class WST_VehicleSpawnPoint : SCR_Position
{
	string spawnPointName;
	
	void WST_VehicleSpawnPoint(IEntitySource src, IEntity parent)
	{
		SetEventMask(EntityEvent.INIT);
	}
	override void EOnInit(IEntity owner)
	{
		//register to gamemode 
		SCR_BaseGameModeWasteland mode = SCR_BaseGameModeWasteland.Cast(GetGame().GetGameMode());
		mode.registerVehicleSpawn(this);
	
	}
	void SetSpawnPointName(string name)
	{
		spawnPointName = name;
	}
	string GetSpawnPointName()
	{
		return spawnPointName;
	}

};