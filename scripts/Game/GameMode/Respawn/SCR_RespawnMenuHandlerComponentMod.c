

//------------------------------------------------------------------------------------------------
modded class SCR_RespawnMenuHandlerComponent 
{
	override void HandleOnFactionAssigned(int playerID, Faction assignedFaction)
	{
		Print("RESPAWNMENUHANDLER::HandleOnFactionAssigned::Fired");

		SCR_RespawnSuperMenu menu = SCR_RespawnSuperMenu.GetInstance();
		Print("RESPAWNMENUHANDLER::HandleOnFactionAssigned::menu = "+ menu);

		if (menu)
		{
					Print("RESPAWNMENUHANDLER::HandleOnFactionAssigned::FireSuperMenu");

			menu.HandleOnFactionAssigned(playerID, assignedFaction);
		}

		if (!m_bAllowLoadoutSelection)
			RandomizePlayerLoadout(playerID);
	}
};