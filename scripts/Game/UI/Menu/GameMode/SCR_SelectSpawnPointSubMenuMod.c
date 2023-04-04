//------------------------------------------------------------------------------------------------
modded class SCR_SelectSpawnPointSubMenu 
{
	
	void update()
	{
		UpdateAndShowSelection();
	}

	//------------------------------------------------------------------------------------------------
	protected void InitMap()
	{
		if (m_MapEntity)
		{
			m_MapConfigSpawn = m_MapEntity.SetupMapConfig(EMapEntityMode.SPAWNSCREEN, m_sMapConfigPath, GetRootWidget());
			m_MapEntity.OpenMap(m_MapConfigSpawn);
		}
	}

	//------------------------------------------------------------------------------------------------
	override void OnMenuUpdate(SCR_SuperMenuBase parentMenu, float tDelta)
	{
		super.OnMenuUpdate(parentMenu, tDelta);
		
		if (m_MapEntity && m_MapEntity.IsOpen() && m_InputManager)
		{
			m_InputManager.ActivateContext("MapContext");
			m_InputManager.ActivateContext("DeployMenuMapContext");
		}
	}

	
	
	//------------------------------------------------------------------------------------------------
	void UntoggleQuick()
	{
		if (m_QuickDeployButton)
			m_QuickDeployButton.SetToggled(false, true, false);
	}
	
	//------------------------------------------------------------------------------------------------
	void UntoggleConfirm()
	{
		if (m_ConfirmButton)
			m_ConfirmButton.SetToggled(false, true, false);
	}

	//------------------------------------------------------------------------------------------------
	void OnMapOpen()
	{
		m_MapEntity.SetZoom(m_fZoom);
		UpdateAndShowSelection();
		if (!SelectLastSelectedSpawnPoint())
			SelectSpawnPoint();
		FocusOnSelectedSpawnPoint(false);
		UpdateTimedSpawnPoint();
		SCR_SpawnPoint sp = SCR_SpawnPoint.GetSpawnPointByRplId(m_SelectedSpawnPointId);
		Event_OnSpawnPointChanged.Invoke(sp);	
		GetGame().GetCallqueue().CallLater(SelectLater, 100, false,);
	}

	//------------------------------------------------------------------------------------------------
	void SelectLater()
	{
		// needed only for first selection because UI icons might not be initialized yet
		SCR_SpawnPoint sp = SCR_SpawnPoint.GetSpawnPointByRplId(m_SelectedSpawnPointId);
		Event_OnSpawnPointChanged.Invoke(sp);	
	}

	//------------------------------------------------------------------------------------------------
	override void OnMenuShow(SCR_SuperMenuBase parentMenu)
	{
		super.OnMenuShow(parentMenu);

		// set the map to fullscreen
		Widget parentRoot = m_ParentMenu.GetRootWidget();
		Widget blur = parentRoot.FindAnyWidget("Blur0");
		if (blur)
			blur.SetVisible(false);

		parentRoot.AddChild(m_wRoot);
		FrameSlot.SetAnchorMin(m_wRoot, 0, 0);
		FrameSlot.SetAnchorMax(m_wRoot, 1, 1);
		FrameSlot.SetOffsets(m_wRoot, 0, 0, 0, 0);
		m_wRoot.SetZOrder(-1);

		if (m_MapEntity && !m_MapEntity.IsOpen())
			InitMap();

		PlayerManager pm = GetGame().GetPlayerManager();
		SCR_RespawnComponent rc = SCR_RespawnComponent.Cast(pm.GetPlayerRespawnComponent(m_iPlayerId));
		rc.GetSpawnPointLockInvoker().Insert(LockConfirmButton);
	}

	//------------------------------------------------------------------------------------------------
	override void OnMenuHide(SCR_SuperMenuBase parentMenu)
	{
		super.OnMenuHide(parentMenu);

		Widget parentRoot = m_ParentMenu.GetRootWidget();
		Widget blur = parentRoot.FindAnyWidget("Blur0");
		if (blur)
			blur.SetVisible(true);

		if (m_MapEntity)
			m_MapEntity.CloseMap();
	}
	
	override void OnMenuClose(SCR_SuperMenuBase parentMenu)
	{
		super.OnMenuClose(parentMenu);

		if (m_MapEntity)
			m_MapEntity.CloseMap();	
	}

	
	//------------------------------------------------------------------------------------------------
	protected void OnSelectedFromMap(MapItem item)
	{
		if (!m_RespawnSystemComponent || !item)
			return;

		SCR_SpawnPoint sp = SCR_SpawnPoint.Cast(item.Entity());
		if (!sp)
		{
			IEntity child = item.Entity().GetChildren();
			while (child)
			{
				if (SCR_SpawnPoint.Cast(child))
				{
					sp = SCR_SpawnPoint.Cast(child);
					break;
				}
				child = child.GetSibling();
			}

			if (!sp)
				return;
		}

		if (sp.GetFactionKey() == m_RespawnSystemComponent.GetPlayerFaction(m_iPlayerId).GetFactionKey())
		{
			RplId id = SCR_SpawnPoint.GetSpawnPointRplId(sp);

			string spName = m_mSpawnPoints.Get(id);
			m_SelectedSpawnPointId = id;

			FocusOnSelectedSpawnPoint();
			int nameId = m_SelectionSpinBox.m_aElementNames.Find(spName);
			if (nameId != -1)
				m_SelectionSpinBox.SetCurrentItem(nameId);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void FocusOnSelectedSpawnPoint(bool smoothPan = true)
	{
		if (!m_MapEntity)
			return;

		CanvasWidget canvas = m_MapEntity.GetMapWidget();
		if (!canvas)
			return;

		SCR_SpawnPoint sp = SCR_SpawnPoint.GetSpawnPointByRplId(m_SelectedSpawnPointId);

		if (!sp)
			return;

		SCR_MapDescriptorComponent descr = GetMapDescriptorFromHierarchy(sp);
		if (!descr || !descr.Item())
			return;

		int x, y;
		vector itemPos = descr.Item().GetPos();

		m_MapEntity.WorldToScreen(itemPos[0], itemPos[2], x, y);
		if (smoothPan)
			m_MapEntity.PanSmooth(x, y);
		else
			m_MapEntity.SetPan(x, y);
	}

	//------------------------------------------------------------------------------------------------
	protected void FocusOnSpawnPointOnly()
	{
		string name = m_SelectionSpinBox.GetCurrentItem();
		m_SelectedSpawnPointId = m_mSpawnPoints.GetKeyByValue(name);
		FocusOnSelectedSpawnPoint();
		UpdateTimedSpawnPoint();
		UntoggleConfirm();
	}

	//------------------------------------------------------------------------------------------------
	protected void UpdateAndShowSelection()
	{
		if (!m_RespawnSystemComponent || !m_MapEntity || !m_MapEntity.IsOpen())
			return;

		Faction faction = m_RespawnSystemComponent.GetPlayerFaction(m_iPlayerId);
		if (!faction)
			return;

		string factionKey = faction.GetFactionKey();
		array<SCR_SpawnPoint> spawnPoints = SCR_SpawnPoint.GetSpawnPointsForFaction(factionKey);
		int cnt = spawnPoints.Count();
		SetButtonsEnabled(cnt > 1);

		for (int i = 0; i < cnt; ++i)
		{
			SCR_SpawnPoint sp = spawnPoints[i];
			//invalid spawn point not getting skipped
			RplId id = SCR_SpawnPoint.GetSpawnPointRplId(sp);
			if (!id.IsValid())
				continue;

			if (m_mSpawnPoints.Get(id))
				continue;

			string spName = m_sDefaultRespawnPointName;
			// todo: bases and mobile hqs also could have ui info?
			if ((sp.GetParent() != null ) && sp.GetParent().Type() == SCR_CampaignBase)
			{
				SCR_CampaignBase base = SCR_CampaignBase.Cast(sp.GetParent());
				spName = base.GetBaseName();

				if (base.GetType() == CampaignBaseType.MAIN)
					m_DefaultSpawnPointId = id;

			}
			else if ((sp.GetParent() != null ) && sp.GetParent().Type() == GenericEntity)
			{
				spName = m_sMobileAssembly;
			}
			else
			{
				SCR_UIInfo info = sp.GetInfo();
				if (info)
					spName = info.GetName();

				if (m_SelectionSpinBox.m_aElementNames.Find(spName) > -1)
				{
					int spId = i+1;
					spName = string.Format("%1 %2", spName, spId);
				}

				if (id == 0)
					m_DefaultSpawnPointId = id;
			}

			m_mSpawnPoints.Set(id, spName);
			if (m_SelectionSpinBox.m_aElementNames.Find(spName) == -1)
				m_SelectionSpinBox.AddItem(spName);
		}

		foreach (RplId id, string name : m_mSpawnPoints)
		{
			SCR_SpawnPoint sp = SCR_SpawnPoint.GetSpawnPointByRplId(id);
			string spName;
			SCR_UIInfo info = sp.GetInfo();
				if (info)
					spName = info.GetName();
			/*if (spName == "")
			{
				int nameId = m_SelectionSpinBox.m_aElementNames.Find(name);
				if (nameId != -1)
					m_SelectionSpinBox.RemoveItem(nameId);
				m_mSpawnPoints.Remove(id);

			}*/
			if (!sp)
				continue;
			if (sp.GetFactionKey() != factionKey)
			{
				if (sp == SCR_SpawnPoint.GetSpawnPointByRplId(SCR_SpawnPoint.s_LastUsed))
					SCR_SpawnPoint.s_LastUsed = RplId.Invalid();
				int nameId = m_SelectionSpinBox.m_aElementNames.Find(name);
				if (nameId != -1)
					m_SelectionSpinBox.RemoveItem(nameId);
				m_mSpawnPoints.Remove(id);
			}
		}

		SetDeployAvailable();
		UpdateTimedSpawnPoint();
	}

	
	
	//------------------------------------------------------------------------------------------------
	static SCR_SelectSpawnPointSubMenu GetInstance()
	{
		return s_Instance;
	}

	//------------------------------------------------------------------------------------------------
	

	//------------------------------------------------------------------------------------------------
	void SCR_SelectSpawnPointSubMenu()
	{
		if (SCR_BaseGameModeWasteland.GetInstance())
		{
			//SCR_CampaignBase.s_OnBaseOwnerChanged.Insert(UpdateAndShowSelection);
			//update spawnpoint selection on spawnpoint change
			SCR_CampaignBase.s_OnSpawnPointOwnerChanged.Insert(UpdateAndShowSelection);
			//SCR_CampaignMobileAssemblyComponent.s_OnSpawnPointOwnerChanged.Insert(UpdateAndShowSelection);
		}

		SCR_MapUIElement.Event_OnPointSelected.Insert(OnSelectedFromMap);
		SCR_MapEntity.GetOnMapOpen().Insert(OnMapOpen);

		SCR_SpawnPoint.Event_SpawnPointFactionAssigned.Insert(SetConfirmButtonToggled);
		SCR_SpawnPoint.Event_OnSpawnPointCountChanged.Insert(UpdateAndShowSelection);
		SCR_SpawnPoint.Event_SpawnPointFactionAssigned.Insert(UpdateAndShowSelection);
		SCR_SpawnPoint.Event_SpawnPointFactionAssigned.Insert(UpdateTimedSpawnPoint);
		SCR_SpawnPoint.Event_SpawnPointRemoved.Insert(RemoveSpawnPointFromList);
		
		s_Instance = this;
	}

	//------------------------------------------------------------------------------------------------
	void ~SCR_SelectSpawnPointSubMenu()
	{
		if (SCR_BaseGameModeWasteland.GetInstance())
		{
			SCR_CampaignBase.s_OnBaseOwnerChanged.Remove(UpdateAndShowSelection);
			SCR_CampaignBase.s_OnSpawnPointOwnerChanged.Remove(UpdateAndShowSelection);
			SCR_CampaignMobileAssemblyComponent.s_OnSpawnPointOwnerChanged.Remove(UpdateAndShowSelection);
		}

		SCR_MapUIElement.Event_OnPointSelected.Remove(OnSelectedFromMap);
		SCR_MapEntity.GetOnMapOpen().Remove(OnMapOpen);

		SCR_SpawnPoint.Event_SpawnPointFactionAssigned.Remove(SetConfirmButtonToggled);
		SCR_SpawnPoint.Event_OnSpawnPointCountChanged.Remove(UpdateAndShowSelection);
		SCR_SpawnPoint.Event_SpawnPointFactionAssigned.Remove(UpdateAndShowSelection);
		SCR_SpawnPoint.Event_SpawnPointFactionAssigned.Remove(UpdateTimedSpawnPoint);
		SCR_SpawnPoint.Event_SpawnPointRemoved.Remove(RemoveSpawnPointFromList);

		if (m_PrevSpawnPoint)
			m_PrevSpawnPoint.m_OnActivated.Remove(SelectPrevSpawnPoint);
		if (m_NextSpawnPoint)
			m_NextSpawnPoint.m_OnActivated.Remove(SelectNextSpawnPoint);
		if (m_ConfirmButton)
			m_ConfirmButton.m_OnToggled.Remove(UntoggleQuick);
		if (m_QuickDeployButton)
			m_QuickDeployButton.m_OnToggled.Remove(UntoggleConfirm);

		s_Instance = null;
	}
};