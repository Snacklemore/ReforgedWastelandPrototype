

//------------------------------------------------------------------------------------------------
modded class SCR_RespawnMenuHandlerComponent 
{
	override void HandleOnFactionAssigned(int playerID, Faction assignedFaction)
	{

		SCR_RespawnSuperMenu menu = SCR_RespawnSuperMenu.GetInstance();

		if (menu)
		{

			menu.HandleOnFactionAssigned(playerID, assignedFaction);
		}

		if (!m_bAllowLoadoutSelection)
			RandomizePlayerLoadout(playerID);
	}
};