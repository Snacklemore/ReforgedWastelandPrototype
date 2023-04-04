
modded class SCR_RespawnSuperMenu 
{
//------------------------------------------------------------------------------------------------

	
	//------------------------------------------------------------------------------------------------
	override void UpdateTabs()
	{
		Print("RESPAWNSUPERMENU::UpdateTabs::funcRoot");

		SCR_GroupsManagerComponent groupsManager = SCR_GroupsManagerComponent.GetInstance();
		int selectedTab = m_TabViewComponent.GetShownTab();
		SCR_RespawnBriefingComponent briefingComponent = SCR_RespawnBriefingComponent.GetInstance();
		SCR_Faction playerFaction = SCR_Faction.Cast(m_RespawnSystemComponent.GetPlayerFaction(SCR_PlayerController.GetLocalPlayerId()));
		bool isFactionAssigned = (
			playerFaction != null
			&& playerFaction.IsPlayable()
		);
				Print("RESPAWNSUPERMENU::UpdateTabs::playerFaction = " + playerFaction);

		bool isLoadoutAssigned = (m_RespawnSystemComponent.GetPlayerLoadout(SCR_PlayerController.GetLocalPlayerId()) != null);
		bool isSpawnPointAssigned = (m_RespawnSystemComponent.GetPlayerSpawnPoint(SCR_PlayerController.GetLocalPlayerId()) != null);
		bool isGroupConfirmed = true;
		if (groupsManager)
			isGroupConfirmed = groupsManager.GetConfirmedByPlayer();
		Print("RESPAWNSUPERMENU::UpdateTabs::isFactionAssigned = " + isFactionAssigned);

		// enable individual submenu tabs based on gamemode settings:
		m_TabViewComponent.SetTabVisible(EDeployScreenType.BRIEFING, briefingComponent && briefingComponent.GetInfo()); 

	 	m_TabViewComponent.EnableTab(EDeployScreenType.FACTION,
	 		(m_RespawnMenuHandler.GetAllowFactionSelection()
	 		&& (m_RespawnMenuHandler.GetAllowFactionChange() || !isFactionAssigned))
	 	);

		m_TabViewComponent.EnableTab(EDeployScreenType.GROUP, isFactionAssigned && groupsManager);

		m_TabViewComponent.EnableTab(EDeployScreenType.LOADOUT,
			(isFactionAssigned
			&& m_RespawnMenuHandler.GetAllowLoadoutSelection()
			&& isGroupConfirmed)
		);

		bool showMap = (
			m_RespawnMenuHandler.GetAllowSpawnPointSelection()
			&& isFactionAssigned
			&& (isLoadoutAssigned || !m_RespawnMenuHandler.GetAllowLoadoutSelection())
			&& isGroupConfirmed
		);
		Print("RESPAWNSUPERMENU::UpdateTabs::showMap = " + showMap);

		m_TabViewComponent.EnableTab(EDeployScreenType.MAP, showMap);

		int nextTab = m_TabViewComponent.GetNextValidItem(false);
		Print("RESPAWNSUPERMENU::UpdateTabs::nextTab = " + nextTab);

		if (m_TabViewComponent.IsTabEnabled(nextTab))
			selectedTab = nextTab;

		// switch to the first enabled tab
		if (showMap)
			selectedTab = EDeployScreenType.MAP;
		else if (!m_TabViewComponent.IsTabEnabled(selectedTab))
			selectedTab = m_TabViewComponent.GetNextValidItem(false);

		if (selectedTab > -1) // todo(hajekmar): should this be handled in ShowTab() instead?
			m_TabViewComponent.ShowTab(selectedTab, true, false);
	}

	//------------------------------------------------------------------------------------------------
	override void UpdateCurrentTab()
	{
		m_TabViewComponent.ShowTab(m_TabViewComponent.GetShownTab());
	}

	//------------------------------------------------------------------------------------------------
	override void HandleOnFactionAssigned(int playerId, Faction faction)
	{
		//nothing getting called
		Print("RESPAWNSUPERMENU::HandleOnFactionAssigned::funcRoot");

		if (playerId == SCR_PlayerController.GetLocalPlayerId())
		{
			if (!faction)
			{
				m_TabViewComponent.ShowTab(EDeployScreenType.FACTION);
				Print("RESPAWNSUPERMENU::HandleOnFactionAssigned::showTabDeployScreen");

			}
			Print("RESPAWNSUPERMENU::HandleOnFactionAssigned::UpdateTabs()");

			UpdateTabs();
		}

		SCR_SelectFactionSubMenu menu = SCR_SelectFactionSubMenu.GetInstance();
		if (menu)
		{
			menu.HandleOnFactionAssigned(playerId, faction);
		}
	}

	
};
 