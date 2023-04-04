//------------------------------------------------------------------------------------------------
modded class SCR_SelectFactionSubMenu 
{
	
	//------------------------------------------------------------------------------------------------
	protected override Faction GetSelectedFaction()
	{
		if (m_TileSelection)
		{
			SCR_FactionMenuTile tile = SCR_FactionMenuTile.Cast(m_TileSelection.GetFocusedTile());
			m_SelectedFaction = m_mAvailableFactions.Get(tile);
		}

		return m_SelectedFaction;
	}

	

	//------------------------------------------------------------------------------------------------
	protected override void UpdateFactionPlayerList()
	{
		if (!m_FactionManager)
			return;

		foreach (SCR_FactionMenuTile tile, Faction faction : m_mAvailableFactions)
		{
			if (!tile)
				continue;

			SCR_Faction scriptedFaction = SCR_Faction.Cast(faction);
			if (scriptedFaction)
				tile.SetPlayerCount(scriptedFaction.GetPlayerCount());

			Widget list = tile.GetPlayerList();
			Widget widget = list.GetChildren();
			while (widget)
			{
				Widget child = widget;
				widget = widget.GetSibling();
				child.RemoveFromHierarchy();
			}

			array<int> players = {};
			PlayerManager pm = GetGame().GetPlayerManager();
			pm.GetAllPlayers(players);

			foreach (int playerId : players)
			{
				Faction playerFaction = m_RespawnSystemComponent.GetPlayerFaction(playerId);

				if (playerFaction != faction)
				{
					Print("FACTIONSUBMENU::UpdateFactionPlayerlist()::skipped player");
					continue;
				}
					

				widget = GetGame().GetWorkspace().CreateWidgets(m_sPlayerName, list);
				if (!widget)
					continue;

				TextWidget text = TextWidget.Cast(widget.FindAnyWidget("Text"));
				if (!text)
					continue;

				string name = pm.GetPlayerName(playerId);
				text.SetText(name);

				PlatformKind platform = pm.GetPlatformKind(playerId);
				string platformName = SCR_Global.GetPlatformName(platform);

				ImageWidget img = ImageWidget.Cast(widget.FindAnyWidget("Platform"));
				if (img && !platformName.IsEmpty())
				{
					img.SetVisible(true);
					img.LoadImageFromSet(0, m_sPlatformIcons, platformName);
				}
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	protected override void UpdateUnassignedPlayerList()
	{
		if (!m_wPlayerList)
			return;

		Widget w = m_wPlayerList.GetChildren();
		while (w)
		{
			Widget child = w;
			w = w.GetSibling();
			child.RemoveFromHierarchy();
		}

		array<int> players = {};
		PlayerManager pm = GetGame().GetPlayerManager();
		pm.GetAllPlayers(players);

		foreach (int playerId : players)
		{
			if (!pm.GetPlayerController(playerId))
				continue;

			Faction playerFaction = m_RespawnSystemComponent.GetPlayerFaction(playerId);
			if (playerFaction)
				continue;

			w = GetGame().GetWorkspace().CreateWidgets(m_sPlayerName, m_wPlayerList);
			if (!w)
				continue;

			TextWidget text = TextWidget.Cast(w.FindAnyWidget("Text"));
			if (!text)
				continue;

			string name = pm.GetPlayerName(playerId);
			text.SetText(name);

			PlatformKind platform = pm.GetPlatformKind(playerId);
			string platformName = SCR_Global.GetPlatformName(platform);

			ImageWidget img = ImageWidget.Cast(w.FindAnyWidget("Platform"));
			if (img && !platformName.IsEmpty())
			{
				img.SetVisible(true);
				img.LoadImageFromSet(0, m_sPlatformIcons, platformName);
			}
		}
	}
	
	
	override void HandleOnFactionAssigned(int playerId, Faction faction)
	{
		UpdateUnassignedPlayerList();
		UpdateFactionPlayerList();
		Print("SUBMENUFACTION::HandleOnFactionAssigned::Fired");

	}
	
	
	
	//------------------------------------------------------------------------------------------------
	protected override bool ConfirmSelection()
	{
		if (GetSelectedFaction())
		{
			SetDeployAvailable();
			bool isTrue;
			Faction l_faction = GetSelectedFaction();
			isTrue = RequestFaction(l_faction);
			Print("SUBMENUFACTION::ConfirmSelection:isTrue ="+ isTrue);
			if(isTrue)
			{
				return isTrue;
			}
			
		}
		Print("SUBMENUFACTION::ConfirmSelection:False");
		return false;
	}

	
};