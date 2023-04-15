class WST_GearPersistenceComponentClass : ScriptComponentClass
{


};



class  WST_GearPersistenceComponent : ScriptComponent
{
	void SaveGear()
	{
		SCR_PlayerController pc =  GetGame().GetPlayerController();
		//RplId pRplId = Replication.FindId(pc.GetControlledEntity());
		int playerId = pc.GetPlayerId();
		Rpc(RpcDo_SaveGear,playerId);
		
	}
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcDo_SaveGear(int id)
	{
		SCR_BaseGameModeWasteland mode = SCR_BaseGameModeWasteland.Cast(GetGame().GetGameMode());
		WST_GearPersistenceManager manager = mode.GetGearPersistenceManager();
		manager.Save(id);
	}

}