
modded class SCR_InventoryMenuUI 
{	
	
	#ifndef DISABLE_INVENTORY
	static protected ResourceName							m_ItemPreviewManagerPrefab = "{9F18C476AB860F3B}Prefabs/World/Game/ItemPreviewManager.et";
	protected SCR_InventoryStorageManagerComponent			m_InventoryManager 		= null;
	protected SCR_CharacterInventoryStorageComponent		m_StorageManager 		= null;
	protected ChimeraCharacter								m_Player;
	protected Widget										m_wContainer;
	protected Widget										m_wStorageList, m_wGadgetStorage, m_wLootStorage, m_wOpenedStorage, m_wWeaponSlots;
	protected Widget  										m_wAttachmentStorage;
	protected Widget										m_widget;
	
	protected SCR_InventoryInspectionUI						m_InspectionScreen				= null;
	
	protected SCR_InventorySlotUI							m_pFocusedSlotUI				= null;
	protected SCR_InventorySlotUI							m_pSelectedSlotUI				= null;
	protected SCR_InventorySlotUI							m_pPrevSelectedSlotUI			= null;

	protected SCR_InventorySlotStorageUI					m_pFocusedSlotStorageUI			= null;
	protected SCR_InventorySlotStorageUI					m_pSelectedSlotStorageUI		= null;
	protected SCR_InventorySlotStorageUI					m_pSelectedSlotStoragePrevUI	= null;

	protected SCR_InventoryStorageBaseUI					m_pStorageBaseUI				= null;
	protected SCR_InventoryStoragesListUI					m_pStorageListUI				= null;
	protected SCR_InventoryStorageGadgetsUI					m_pGadgetsBaseUI				= null;
	protected SCR_InventoryStorageBaseUI					m_pStorageLootUI				= null;
	protected SCR_InventoryWeaponSlotsUI					m_pWeaponStorage				= null;
	protected EquipedWeaponStorageComponent					m_pWeaponStorageComp			= null;
	protected ref array<SCR_InventoryOpenedStorageUI> 		m_aOpenedStoragesUI				= {};
	protected SCR_InventoryAttachmentStorageUI				m_pAttachmentStorageUI			= null;

	protected SCR_InventoryItemInfoUI						m_pItemInfo						= null;
	protected SCR_InventoryStorageBaseUI					m_pActiveStorageUI				= null;
	protected SCR_InventoryStorageBaseUI					m_pActiveHoveredStorageUI		= null;
	protected SCR_InventoryStorageBaseUI					m_pPrevActiveStorageUI			= null;
	protected ref array<SCR_InventorySlotUI>				m_aShownStorages				= new ref array<SCR_InventorySlotUI>();	//used for storing pointers on storages which are displayed on screen ( for the refreshing purposes )
	protected GridLayoutWidget								m_wStoragesContainer;
	protected const int										STORAGE_AREA_COLUMNS			= 2;
	protected int											m_iStorageListCounter			= 0;
	protected int											m_iVicinityDiscoveryRadius		= 0;
	//protected static bool									m_bColdStart = true;			// uncomment to enable the expand / collapse feature of storages

	protected ref SCR_InventorySlotUI						m_pInspectedSlot				= null;
	protected ref SCR_InventoryGearInspectionPointUI		m_pGearInspectionPointUI		= null;
	protected Widget										m_wAttachmentContainer			= null;
	
	protected ProgressBarWidget							m_wProgressBarWeight;
	protected ProgressBarWidget							m_wProgressBarWeightItem;

	const string 											BACKPACK_STORAGE_LAYOUT= "{06E9285D68D190EF}UI/layouts/Menus/Inventory/InventoryContainerGrid.layout";
	const string											WEAPON_STORAGE_LAYOUT	= "{7B28D87B8A1ADD41}UI/layouts/Menus/Inventory/InventoryWeaponSlots.layout";
	const string 											GADGETS_STORAGE_LAYOUT 	= "{265189B87ED5CD10}UI/layouts/Menus/Inventory/InventoryGadgetsPanel.layout";
	const string 											STORAGES_LIST_LAYOUT 	= "{FC579324F5E4B3A3}UI/layouts/Menus/Inventory/InventoryCharacterGrid.layout";
	const string 											ITEM_INFO			 	= "{AE8B7B0A97BB0BA8}UI/layouts/Menus/Inventory/InventoryItemInfo.layout";
	const string											ACTION_LAYOUT 			= "{81BB7785A3987196}UI/layouts/Menus/Inventory/InventoryActionNew.layout";

	const ResourceName										HITZONE_CONTAINER_LAYOUT= "{36DB099B4CDF8FC2}UI/layouts/Menus/Inventory/Medical/HitZonePointContainer.layout";

	protected EStateMenuStorage								m_EStateMenuStorage = EStateMenuStorage.STATE_IDLE; // is this useful for anything?
	protected EStateMenuItem								m_EStateMenuItem = EStateMenuItem.STATE_IDLE;
	protected string 										m_sFSMStatesNames[10]={"init", "idle", "item selected", "storage selected", "STATE_STORAGE_OPENED", "storage unfolded", "move started", "move finished", "closing", "STATE_UNKNOWN" };
	protected SCR_NavigationBarUI							m_pNavigationBar			= null;
	protected SCR_NavigationButtonComponent					m_CloseButton;

	//variables dedicated to move an item from storage to storage
	protected IEntity 											m_pItem;
	protected BaseInventoryStorageComponent						m_pDisplayedStorage, m_pLastCurrentNavStorage;
	protected BaseInventoryStorageComponent						m_pStorageFrom, m_pStorageTo;	//last known storages from the last move operation
//	protected SCR_InventorySlotUI	 							m_pStorageUIFrom;
	protected SCR_InventorySlotStorageUI						m_pStorageUITo;	//last known storagesUI from the last move operation
	protected SCR_CharacterVicinityComponent 					m_pVicinity;
	ItemPreviewWidget 											m_wPlayerRender, m_wPlayerRenderSmall;
	PreviewRenderAttributes										m_PlayerRenderAttributes;
	protected ButtonWidget										m_wButtonShowAll;
	protected SCR_InventoryCharacterWidgetHelper 				m_pCharacterWidgetHelper;
	protected ItemPreviewManagerEntity 							m_pPreviewManager;
	protected bool												m_bDraggingEnabled;
	protected FrameWidget										m_wDragDropContainer;
	protected SCR_SlotUIComponent 								m_pDragDropFrameSlotUI;
	protected RenderTargetWidget								m_pDragDropPreviewImage;
	protected ref array<SCR_InventoryStorageBaseUI>  			m_aStorages = {};
	protected TextWidget										m_wTotalWeightText;
	protected bool												m_bLocked = false;	//helper variable

	//Item/Weapon Switching
	protected SCR_InventoryStorageQuickSlotsUI			m_pQuickSlotStorage;
	protected Widget									m_wQuickSlotStorage;
	protected ref SCR_InvCallBack						m_pCallBack = new SCR_InvCallBack();
	const int											WEAPON_SLOTS_COUNT = 4;

	protected bool												m_bIsUsingGamepad;
	protected float 											m_fX, m_fY;	//debug;
	protected bool												m_bShowIt = true;
	protected int												m_iMouseX, m_iMouseY
	const int													DRAG_THRESHOLD 			= 5;

	//other character's information
	protected SCR_CharacterControllerComponent					m_CharController;
	protected Widget											m_wCharFeatureBleeding;
	protected Widget											m_wCharFeatureWounded;
	//protected ProgressBarWidget								m_wInfoStamina;	// Preparation for showing the stamina level in inventory
	protected ref array<HitZone> 								m_aBleedingHitZones = {};

	protected bool 												m_bWasJustTraversing;
	protected bool 												m_bStorageSwitchMode;
	protected SCR_InventorySlotUI 								m_pItemToAssign;

	protected Widget											m_wAttachmentPointsContainer;
	protected ref array<SCR_InventoryHitZonePointContainerUI> 	m_aHitZonePoints = {};

	protected SCR_InventoryHitZonePointUI 						m_pBleedingHandlerGlobal;
	protected SCR_InventoryHitZonePointUI 						m_pDamageHandlerGlobal;

	protected SCR_CharacterDamageManagerComponent				m_CharDamageManager;
	protected const int											CHARACTER_HITZONES_COUNT = 7;

	protected SCR_InventorySpinBoxComponent 					m_AttachmentSpinBox;

	//------------------------------------------------------------------------------------------------
	protected void InitializeCharacterHitZones()
	{
		if (m_AttachmentSpinBox)
			m_AttachmentSpinBox.ClearAll();

		Widget bleeding = m_widget.FindAnyWidget("BleedingGlobal");
		Widget damage = m_widget.FindAnyWidget("DamageGlobal");

		m_CharController = SCR_CharacterControllerComponent.Cast(m_Player.GetCharacterController());
		m_CharDamageManager = SCR_CharacterDamageManagerComponent.Cast(m_Player.GetDamageManager());
		if (bleeding)
			m_pBleedingHandlerGlobal = SCR_InventoryHitZonePointUI.Cast(bleeding.FindHandler(SCR_InventoryHitZonePointUI));

		if (damage)
			m_pDamageHandlerGlobal = SCR_InventoryHitZonePointUI.Cast(damage.FindHandler(SCR_InventoryHitZonePointUI));
		
		if (!m_wAttachmentPointsContainer)
			return;

		SCR_InventoryHitZoneUI hitZoneHandler;

		for (int i = 0; i < CHARACTER_HITZONES_COUNT; ++i)
		{
			Widget w = GetGame().GetWorkspace().CreateWidgets(HITZONE_CONTAINER_LAYOUT, m_wAttachmentPointsContainer);
			SCR_InventoryHitZonePointContainerUI point = SCR_InventoryHitZonePointContainerUI.Cast(w.FindHandler(SCR_InventoryHitZonePointContainerUI));
			point.InitializeHitZoneUI(m_pStorageListUI.GetStorage(), this, i + 1, m_Player);
			m_aHitZonePoints.Insert(point);
			string hzName = point.GetHitZoneName();
		}
		
		OnDamageStateChanged();
		GetGame().GetCallqueue().CallLater(UpdateCharacterPreview, 50, false);
	}

	//------------------------------------------------------------------------------------------------
	protected void HideCharacterHitZones()
	{
		if (m_AttachmentSpinBox)
			m_AttachmentSpinBox.ClearAll();
		if (m_pBleedingHandlerGlobal)
			m_pBleedingHandlerGlobal.GetRootWidget().SetVisible(false);
		if (m_pDamageHandlerGlobal)
			m_pDamageHandlerGlobal.GetRootWidget().SetVisible(false);

		foreach (SCR_InventoryHitZonePointContainerUI point : m_aHitZonePoints)
		{
			point.GetRootWidget().RemoveFromHierarchy();
		}
		
		m_aHitZonePoints.Clear();
	}

	//------------------------------------------------------------------------------------------------
	// !
	protected void InitializeCharacterInformation()
	{
		SCR_CharacterBloodHitZone charBloodHZ = SCR_CharacterBloodHitZone.Cast(m_CharDamageManager.GetBloodHitZone());
		charBloodHZ.GetOnDamageStateChanged().Insert(OnDamageStateChanged);
		m_CharDamageManager.GetOnDamageOverTimeRemoved().Insert(OnDamageStateChanged);
		m_CharDamageManager.GetOnDamageStateChanged().Insert(OnDamageStateChanged);
		OnDamageStateChanged();
	}
	
	//------------------------------------------------------------------------------------------------
	void AddItemToAttachmentSelection(string item, Managed data = null)
	{
		if (m_AttachmentSpinBox.FindItem(item) < 0)
			m_AttachmentSpinBox.AddItem(item, data);

		SetAttachmentSpinBoxActive(m_bIsUsingGamepad);
	}

	//------------------------------------------------------------------------------------------------
	void RemoveItemFromAttachmentSelection(string item)
	{
		int index = m_AttachmentSpinBox.FindItem(item);
		if (index > -1)
			m_AttachmentSpinBox.RemoveItem(index);
		
		SetAttachmentSpinBoxActive(m_bIsUsingGamepad);
	}
	
	//------------------------------------------------------------------------------------------------
	// !
	void OnDamageStateChanged()
	{
		SCR_CharacterBloodHitZone charBloodHZ = SCR_CharacterBloodHitZone.Cast(m_CharDamageManager.GetBloodHitZone());

		bool bleedingVisible = m_CharDamageManager.IsDamagedOverTime(EDamageType.BLEEDING);

		float health = charBloodHZ.GetHealthScaled();
		float treshold = charBloodHZ.GetDamageStateThreshold(ECharacterBloodState.UNDAMAGED) - charBloodHZ.GetDamageStateThreshold(ECharacterBloodState.UNCONSCIOUS);

		if (m_pBleedingHandlerGlobal)
		{
			m_pBleedingHandlerGlobal.GetRootWidget().SetVisible(bleedingVisible);
			m_pBleedingHandlerGlobal.UpdateRegen();
		}

		health = m_CharDamageManager.GetHealthScaled();
		int state = health / 0.25;
		
		if (m_pDamageHandlerGlobal)
		{
			m_pDamageHandlerGlobal.GetRootWidget().SetVisible(health < 1);
			m_pDamageHandlerGlobal.UpdateRegen(state);
		}
	}

	//------------------------------------------------------------------------------------------------
	// !
	void RegisterUIStorage( SCR_InventorySlotUI pStorageUI )
	{
		if ( m_aShownStorages.Find( pStorageUI ) > -1 )
			return;

		if ( SCR_InventorySlotStorageUI.Cast( pStorageUI ) || SCR_InventorySlotWeaponUI.Cast( pStorageUI ) )
			m_aShownStorages.Insert( pStorageUI );
	}

	//------------------------------------------------------------------------------------------------
	// !
	void UnregisterUIStorage( SCR_InventorySlotUI pStorageUI )
	{
		if ( pStorageUI )
			m_aShownStorages.RemoveItem( pStorageUI );
	}

	//------------------------------------------------------------------------------------------------
	void SetActiveStorage( SCR_InventoryStorageBaseUI storageUI )
	{
		m_pPrevActiveStorageUI = m_pActiveStorageUI;
		m_pActiveStorageUI = storageUI;
		if( m_pActiveStorageUI )
		{
			m_pFocusedSlotUI = m_pActiveStorageUI.GetFocusedSlot();
		}
	}

	//------------------------------------------------------------------------------------------------
	//---Get total weight from all storages (weapon, gadget, deposit)
	protected float GetTotalWeight()
	{
		if (!m_InventoryManager)
			return 0;
		
		return m_InventoryManager.GetTotalWeightOfAllStorages();
	}

	//------------------------------------------------------------------------------------------------
	SCR_InventoryStorageManagerComponent GetInventoryStorageManager()
	{
		return m_InventoryManager;
	}

	//------------------------------------------------------------------------------------------------
	//! Actualy opened UI storage in character's storage
	SCR_InventoryStorageBaseUI GetActualStorageInCharacterStorageUI() { return m_pStorageBaseUI; }
	SCR_InventoryStorageBaseUI GetStorageList() { return m_pStorageListUI; }
	SCR_InventoryStorageBaseUI GetLootStorage() { return m_pStorageLootUI; }

	//------------------------------------------------------------------------------------------------
	//! Returns whether interaction with inventory items is possible
	bool GetCanInteract()
	{
		if (GetInspectionScreen())
			return false;

		return true;
	}

	//------------------------------------------------------------------------------------------------
	protected void RefreshPlayerWidget()
	{
		if (!m_pPreviewManager)
			return;
		if (m_wPlayerRender)
			m_pPreviewManager.SetPreviewItem(m_wPlayerRender, m_Player, m_PlayerRenderAttributes);
		if (m_wPlayerRenderSmall)
			m_pPreviewManager.SetPreviewItem(m_wPlayerRenderSmall, m_Player, m_PlayerRenderAttributes);
	}

	//------------------------------------------------------------------------------------------------
	override void OnMenuUpdate(float tDelta)
	{
		super.OnMenuUpdate(tDelta);

		if (m_InspectionScreen)
		{
			m_InspectionScreen.UpdateView(tDelta);
			return;
		}

		if (m_pCharacterWidgetHelper && m_pPreviewManager && m_PlayerRenderAttributes && m_pCharacterWidgetHelper.Update(tDelta, m_PlayerRenderAttributes))
		{
			UpdatePreview();
		}
	}

	//------------------------------------------------------------------------------------------------
	void UpdatePreview()
	{
		if (m_pInspectedSlot)
			UpdateGearInspectionPreview();
		else
			UpdateCharacterPreview();
	}
	
	//------------------------------------------------------------------------------------------------
	protected void UpdateCharacterPreview()
	{
		m_pPreviewManager.SetPreviewItem(m_wPlayerRender, m_Player, m_PlayerRenderAttributes);
		for (int i = 0; i < m_aHitZonePoints.Count(); ++i)
		{
			SCR_InventoryHitZonePointContainerUI hp = m_aHitZonePoints[i];
			if (!hp)
				continue;
			TNodeId id = hp.GetBoneIndex();

			vector transform[4];
			Math3D.MatrixIdentity4(transform);
			vector screenPos;
			if (m_wPlayerRender.TryGetItemNodePositionInWidgetSpace(id, transform, screenPos))
				FrameSlot.SetPos(hp.GetRootWidget(), screenPos[0], screenPos[1]);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void UpdateGearInspectionPreview()
	{	
		if (!m_pInspectedSlot)
		{
			InspectItem(null);
			return;
		}
		
		InventoryItemComponent itemComp = m_pInspectedSlot.GetInventoryItemComponent();
		
		if (!itemComp)
		{
			InspectItem(null);
			return;
		}
		
		m_pPreviewManager.SetPreviewItem(m_wPlayerRender, itemComp.GetOwner(), m_PlayerRenderAttributes, true);
		
		if (m_wPlayerRender && m_pGearInspectionPointUI)
			m_pGearInspectionPointUI.UpdatePreviewSlotWidgets(m_wPlayerRender);
	}

	//------------------------------------------------------------------------------------------------
	override void OnMenuOpen()
	{
		if( !Init() )
		{
			Action_CloseInventory();
			return;
		}

		//TODO: decide what storage should be opened as default on the start

		//ShowStorage( ESlotID.SLOT_BACKPACK );
		//ShowStorage( ESlotID.SLOT_WEAPONS_STORAGE );
		//ShowGadgetStorage();
		ShowStoragesList();
		ShowAllStoragesInList();
		ShowVicinity();
		m_pPreviewManager = GetGame().GetItemPreviewManager();
		if (m_pPreviewManager)
		{
			m_wPlayerRender = ItemPreviewWidget.Cast( m_widget.FindAnyWidget( "playerRender" ) );
			auto collection = m_StorageManager.GetAttributes();
			if (collection)
				m_PlayerRenderAttributes = PreviewRenderAttributes.Cast(collection.FindAttribute(SCR_CharacterInventoryPreviewAttributes));
			SizeLayoutWidget wPlayerRenderSmallRoot = SizeLayoutWidget.Cast( m_widget.FindAnyWidget( "playerRenderSmallRoot" ) );
			if ( wPlayerRenderSmallRoot )
				m_wPlayerRenderSmall = ItemPreviewWidget.Cast( wPlayerRenderSmallRoot.FindAnyWidget( "item" ) );
			RefreshPlayerWidget();
			m_pCharacterWidgetHelper = SCR_InventoryCharacterWidgetHelper(m_wPlayerRender, GetGame().GetWorkspace() );
		}

		Widget wrap = m_widget.FindAnyWidget( "WrapLayoutShow" );
		m_wButtonShowAll = ButtonWidget.Cast( wrap.FindAnyWidget( "ItemButton" ) );
		if( m_wButtonShowAll )
		{
			m_wButtonShowAll.AddHandler( new SCR_InventoryButton( EInventoryButton.BUTTON_SHOW_DEFAULT, this ) );
		}

		if( m_pNavigationBar )
		{
			m_pNavigationBar.m_OnAction.Insert(OnAction);

			m_CloseButton = SCR_NavigationButtonComponent.GetNavigationButtonComponent("Back", m_widget);
			if (m_CloseButton)
				m_CloseButton.m_OnActivated.Insert(Action_TryCloseInventory);
		}
		GetGame().GetInputManager().AddActionListener("Inventory_Drag", EActionTrigger.DOWN, Action_DragDown);
		GetGame().GetInputManager().AddActionListener("Inventory", EActionTrigger.DOWN, Action_CloseInventory);
		InitAttachmentSpinBox();
		OnInputDeviceIsGamepad(!GetGame().GetInputManager().IsUsingMouseAndKeyboard());
		GetGame().OnInputDeviceIsGamepadInvoker().Insert(OnInputDeviceIsGamepad);		
		
		SetAttachmentSpinBoxActive(m_bIsUsingGamepad);
		
		ResetHighlightsOnAvailableStorages();
		SetOpenStorage();
		UpdateTotalWeightText();
		
		InitializeCharacterHitZones();
		UpdateCharacterPreview();
		
		SetStorageSwitchMode(m_bIsUsingGamepad);
	}

	void InitAttachmentSpinBox()
	{
		if (m_AttachmentSpinBox)
		{
			m_AttachmentSpinBox.ClearAll();
			return;
		}

		Widget w = m_widget.FindAnyWidget("AttachmentSpinBox");
		if (!w)
		{
			Print("Cannot find AttachmentSpinBox widget in hierarchy!", LogLevel.ERROR);
			return;
		}

		m_AttachmentSpinBox = SCR_InventorySpinBoxComponent.Cast(w.FindHandler(SCR_InventorySpinBoxComponent));
	}


	//------------------------------------------------------------------------------------------------
	protected void SetStorageSwitchMode(bool enabled)
	{
		m_bStorageSwitchMode = enabled;
		foreach (SCR_InventoryStorageBaseUI storage : m_aStorages)
		{
			if (storage)
				storage.ActivateStorageButton(m_bStorageSwitchMode);
		}

		m_pStorageLootUI.ActivateStorageButton(m_bStorageSwitchMode);
		m_pStorageListUI.ActivateStorageButton(m_bStorageSwitchMode);
		m_pQuickSlotStorage.ActivateStorageButton(m_bStorageSwitchMode);

		foreach (SCR_InventoryOpenedStorageUI storage : m_aOpenedStoragesUI)
		{
			storage.ActivateStorageButton(m_bStorageSwitchMode);
		}

		if (m_bStorageSwitchMode)
		{
			Widget btnToFocus = m_pStorageLootUI.GetButtonWidget();
			if (m_pActiveStorageUI)
				btnToFocus = m_pActiveStorageUI.GetButtonWidget();
			GetGame().GetWorkspace().SetFocusedWidget(btnToFocus);
		}
		else
		{
			if (m_pActiveStorageUI)
			{
				m_pActiveStorageUI.ShowContainerBorder(false);
				FocusOnSlotInStorage(m_pActiveStorageUI);
			}
		}

		NavigationBarUpdate();
	}

	//------------------------------------------------------------------------------------------------
	protected void ToggleStorageSwitchMode()
	{
		SetStorageSwitchMode(!m_bStorageSwitchMode);
	}

	protected void SetAttachmentSpinBoxActive(bool enable)
	{
		if (!m_AttachmentSpinBox)
			return;

		int itemCount = m_AttachmentSpinBox.GetNumItems();
		bool shouldBeActive = (enable && (itemCount > 0));
		m_AttachmentSpinBox.SetEnabled(shouldBeActive);
		m_AttachmentSpinBox.GetRootWidget().SetVisible(shouldBeActive);

		Widget leftArea = m_widget.FindAnyWidget("StorageLootSlot");
		Widget rightArea = m_widget.FindAnyWidget("StoragesListSlot");
		Widget openStorages = m_widget.FindAnyWidget("OpenedStorages");

		if (shouldBeActive)
		{
			openStorages.SetNavigation(WidgetNavigationDirection.RIGHT, WidgetNavigationRuleType.EXPLICIT, "AttachmentSpinBox");
			leftArea.SetNavigation(WidgetNavigationDirection.RIGHT, WidgetNavigationRuleType.EXPLICIT, "AttachmentSpinBox");
			rightArea.SetNavigation(WidgetNavigationDirection.LEFT, WidgetNavigationRuleType.EXPLICIT, "AttachmentSpinBox");
			m_AttachmentSpinBox.GetRootWidget().SetNavigation(WidgetNavigationDirection.RIGHT, WidgetNavigationRuleType.EXPLICIT, "StoragesListSlot");
		}
		else
		{
			openStorages.SetNavigation(WidgetNavigationDirection.RIGHT, WidgetNavigationRuleType.EXPLICIT, "StoragesListSlot");
			leftArea.SetNavigation(WidgetNavigationDirection.RIGHT, WidgetNavigationRuleType.EXPLICIT, "StoragesListSlot");
			rightArea.SetNavigation(WidgetNavigationDirection.LEFT, WidgetNavigationRuleType.EXPLICIT, "StorageLootSlot");
		}
	}

	//------------------------------------------------------------------------------------------------
	bool FocusOnSlotInStorage(SCR_InventoryStorageBaseUI storage, int id = 0, bool focus = true)
	{
		if (!storage || id < 0)
			return false;

		array<SCR_InventorySlotUI> slots = {};
		storage.GetSlots(slots);

		if (slots.IsEmpty() || !slots[id])
			return false;

		if (focus)
		{
			Widget focusedWidget = slots[id].GetButtonWidget();
			GetGame().GetWorkspace().SetFocusedWidget(slots[id].GetButtonWidget());
		}

		return true;
	}

	//------------------------------------------------------------------------------------------------
	//! returns true if the init procedure was success
	protected bool Init()
	{
		m_widget = GetRootWidget();

		if ( m_widget == null )
			return false;

		//Get player
		PlayerController playerController = GetGame().GetPlayerController();
		if ( playerController != null )
		{
			m_Player = ChimeraCharacter.Cast(playerController.GetControlledEntity());
			if ( m_Player != null )
			{
				if (!GetGame().GetItemPreviewManager())
				{
					Resource rsc = Resource.Load(m_ItemPreviewManagerPrefab);
					if (rsc.IsValid())
						GetGame().SpawnEntityPrefabLocal(rsc);
				}

				m_CharController = SCR_CharacterControllerComponent.Cast(m_Player.GetCharacterController());
				if (m_CharController)
				{
					m_CharController.m_OnPlayerDeath.Insert(Action_CloseInventory);
					if (m_CharController.IsDead() || m_CharController.IsUnconscious())
					{
						return false;
					}
				}

				//Inventory Manager
				m_InventoryManager = SCR_InventoryStorageManagerComponent.Cast( m_Player.FindComponent( SCR_InventoryStorageManagerComponent ) );
				m_StorageManager = SCR_CharacterInventoryStorageComponent.Cast( m_Player.FindComponent( SCR_CharacterInventoryStorageComponent ) );
				if( !m_StorageManager )
					return false;
				m_pWeaponStorageComp = EquipedWeaponStorageComponent.Cast(m_StorageManager.GetWeaponStorage());
				if( !m_pWeaponStorageComp )
					return false;
				if ( m_InventoryManager )
				{
					m_InventoryManager.m_OnItemAddedInvoker.Insert(OnItemAddedListener);
					m_InventoryManager.m_OnItemRemovedInvoker.Insert(OnItemRemovedListener);
				}

				m_wProgressBarWeight = ProgressBarWidget.Cast( m_widget.FindAnyWidget( "ProgressBarWeight" ) );
				if( m_wProgressBarWeight )
				{
					m_wProgressBarWeight.SetMin( 0.0 );
					m_wProgressBarWeight.SetMax( m_StorageManager.GetMaxLoad() );
					m_wProgressBarWeight.SetCurrent( m_InventoryManager.GetTotalWeightOfAllStorages() );
				};

				m_wProgressBarWeightItem = ProgressBarWidget.Cast( m_widget.FindAnyWidget( "ProgressBarWeightItem" ) );
				if( m_wProgressBarWeightItem )
				{
					m_wProgressBarWeightItem.SetMin( 0.0 );
					m_wProgressBarWeightItem.SetMax( m_StorageManager.GetMaxLoad() );
					m_wProgressBarWeightItem.SetCurrent( 0.0 );
				};

				Widget wNaviBar = m_widget.FindAnyWidget( "Footer" );
				if( wNaviBar )
					m_pNavigationBar = SCR_NavigationBarUI.Cast( wNaviBar.FindHandler( SCR_NavigationBarUI ) );
				m_pVicinity = SCR_CharacterVicinityComponent.Cast(m_Player.FindComponent(SCR_CharacterVicinityComponent));
				if (m_pVicinity)
				{
					m_pVicinity.OnVicinityUpdateInvoker.Insert(RefreshLootUIListener);
					m_iVicinityDiscoveryRadius = m_pVicinity.GetDiscoveryRadius();
				}

				m_wStoragesContainer = GridLayoutWidget.Cast( m_widget.FindAnyWidget( "StorageGrid" ) );
				m_wTotalWeightText = TextWidget.Cast( m_widget.FindAnyWidget("TotalWeightText") );
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}


		ShowQuickSlotStorage();

		int quickSlotCount = m_StorageManager.GetQuickSlotItems().Count();
		for (int slotId = 1; slotId <= quickSlotCount; ++slotId)
		{
			GetGame().GetInputManager().AddActionListener("InventoryQuickSlot" + slotId, EActionTrigger.PRESSED, Action_SelectQuickSlot);
		}
		
		m_wDragDropContainer = FrameWidget.Cast( m_widget.FindAnyWidget( "DragDropContainer" ) );
		if ( m_wDragDropContainer )
		{
			m_pDragDropFrameSlotUI = SCR_SlotUIComponent.Cast( m_wDragDropContainer.FindHandler( SCR_SlotUIComponent ) );
			m_pDragDropPreviewImage = RenderTargetWidget.Cast( m_wDragDropContainer.FindAnyWidget( "item" ) );
		}

		m_wAttachmentPointsContainer = m_widget.FindAnyWidget("AttachmentPoints");

		return true;
	}

	//------------------------------------------------------------------------------------------------
	void OnAction( SCR_NavigationButtonComponent comp, string action, SCR_InventoryStorageBaseUI pParentStorage = null, int traverseStorageIndex = -1 )
	{
		switch (action)
		{
			case "Inventory":
			{
				Action_CloseInventory();
			}
			break;

			case "Inventory_UnassignFromQuickSlot":
			{
				Action_QuickSlotUnassign();
			} break;

			case "Inventory_AssignToQuickSlot":
			{
				Action_QuickSlotAssign();
			} break;

			case "Inventory_Drop":
			{
				Action_MoveBetween();
			} break;

			case "Inventory_Equip":
			{
				Action_EquipItem();
			} break;

			case "Inventory_Select":
			{
				if (m_pAttachmentStorageUI && m_AttachmentSpinBox.IsFocused())
				{
					FocusOnSlotInStorage(m_pAttachmentStorageUI);
					return;
				}

				if (m_bStorageSwitchMode && m_bIsUsingGamepad)
				{
					SetStorageSwitchMode(false);
					return;
				}
				if (m_pFocusedSlotUI && !m_pFocusedSlotUI.IsSlotSelected())
					Action_SelectItem();
				else
					Action_DeselectItem();
			} break;

			case "Inventory_Deselect":
			{
				Action_DeselectItem();
			} break;

			case "Inventory_OpenStorage":
			{
				Action_UnfoldItem();
			} break;

			case "Inventory_OpenNewStorage":
			{
				Action_OpenContainer();
			} break;

			case "Inventory_Back":
			{
				Action_StepBack( pParentStorage, traverseStorageIndex );
			} break;

			case "Inventory_StepBack":
			{
				Action_StepBack( pParentStorage, traverseStorageIndex );
			} break;

			case "Inventory_Inspect":
			{
				Action_Inspect();
			} break;

			case "Inventory_Use":
			{
				Action_UseItem();
			} break;

			case "Inventory_Move":
			{
				if (m_bStorageSwitchMode)
				{
					Action_Drop();
					return;
				}
				Action_MoveItemToStorage();
			} break;

			case "Inventory_Swap":
			{
				if (m_bStorageSwitchMode)
				{
					ToggleStorageSwitchMode();
					FocusOnSlotInStorage(m_pActiveStorageUI);
					return;
				}
				Action_SwapItems(m_pSelectedSlotUI, m_pFocusedSlotUI);
			} break;
		}
	}

	//------------------------------------------------------------------------------------------------
	// ! Default view of the character's inventory
	void ShowDefault()
	{
		ShowEquipedWeaponStorage();
		if ( !m_pSelectedSlotStorageUI )
		{
			SimpleFSM( EMenuAction.ACTION_BACK );
			return;
		}
		m_pSelectedSlotStorageUI.SetSelected( false );

		if ( m_pSelectedSlotUI )
		{
			m_pSelectedSlotUI.SetSelected( false );
			m_pSelectedSlotUI = null;
		}
		if ( m_pSelectedSlotStorageUI )
			m_pSelectedSlotStorageUI = null;

		FilterOutStorages( false );
	}

	//------------------------------------------------------------------------------------------------
	protected int CloseOpenedStorage()
	{
		if ( !m_wContainer )
			return -1;	//any storage opened
		return CloseStorage( m_wContainer );
	}

	//------------------------------------------------------------------------------------------------
	protected int CloseStorage( SCR_InventoryStorageBaseUI pStorageUI )
	{
		//if (!pStorageUI)
			//return -1;
		Widget w = pStorageUI.GetRootWidget();
		if ( !w )
			return -1;
		return CloseStorage( w );
	}

	//------------------------------------------------------------------------------------------------
	protected int CloseStorage( notnull Widget w )
	{
		auto storageUIHandler = SCR_InventoryStorageBaseUI.Cast( w.FindHandler( SCR_InventoryStorageBaseUI ) );
		if ( !storageUIHandler )
			return -1;	//some storage opened, but it does not have any handler ( wrong )

		int iLastShownPage = storageUIHandler.GetLastShownPage();
		m_pLastCurrentNavStorage = storageUIHandler.GetCurrentNavigationStorage();
		w.RemoveHandler( storageUIHandler );	//remove the handler from the widget
		w.RemoveFromHierarchy();
		return iLastShownPage;
	}

	//------------------------------------------------------------------------------------------------
	// ! Hides/Unhides ( not closes ) the storage
	protected void ToggleStorageContainerVisibility( notnull SCR_InventorySlotUI pSlot )
	{
		BaseInventoryStorageComponent pStorage = pSlot.GetAsStorage();
		if ( !pStorage )
			return;
		SCR_InventoryStorageBaseUI pStorageUI = GetStorageUIByBaseStorageComponent( pStorage );
		if ( !pStorageUI )
			return;
		pStorageUI.ToggleShow();
	}

	//------------------------------------------------------------------------------------------------
	// ! Shows the content of the 1st available storage
	void ShowStorage()
	{
		if( !m_StorageManager )
			return;

		array<SCR_UniversalInventoryStorageComponent> pStorages = new array<SCR_UniversalInventoryStorageComponent>();
		m_StorageManager.GetStorages( pStorages );

		if( pStorages.Count() == 0 )
			return;
		SCR_UniversalInventoryStorageComponent pStorage = pStorages.Get( 0 );
		ShowStorage( pStorage );
	}

	//------------------------------------------------------------------------------------------------
	//!
	void ShowStorage( BaseInventoryStorageComponent storage, LoadoutAreaType area = null )
	{
		if ( !m_wStoragesContainer )
			return;
		m_wContainer =  GetGame().GetWorkspace().CreateWidgets( BACKPACK_STORAGE_LAYOUT, m_wStoragesContainer );
		int iRow = Math.Floor( m_iStorageListCounter / STORAGE_AREA_COLUMNS );
		int iCol = m_iStorageListCounter % STORAGE_AREA_COLUMNS;
		m_iStorageListCounter++;

		m_wStoragesContainer.SetRowFillWeight( iRow, 0 );
		m_wStoragesContainer.SetColumnFillWeight( iCol, 0 );

		GridSlot.SetColumn( m_wContainer, iCol );
		GridSlot.SetRow( m_wContainer, iRow );
		GridSlot.SetColumnSpan( m_wContainer, 1 );
		GridSlot.SetRowSpan( m_wContainer, 1 );

		if ( !m_wContainer )
			return;
		if ( storage.Type() == ClothNodeStorageComponent )
		{
			m_wContainer.AddHandler( new SCR_InventoryStorageLBSUI( storage, area, this, 0, null) );
			m_pStorageBaseUI = SCR_InventoryStorageBaseUI.Cast( m_wContainer.FindHandler( SCR_InventoryStorageLBSUI ) );
		}
		else if ( storage.Type() == EquipedWeaponStorageComponent )
		{
			m_wContainer.AddHandler( new SCR_InventoryStorageWeaponsUI( m_StorageManager.GetWeaponStorage(), area, this ) );
			m_pStorageBaseUI = SCR_InventoryStorageWeaponsUI.Cast( m_wContainer.FindHandler( SCR_InventoryStorageWeaponsUI ) );
		}
		else
		{
			m_wContainer.AddHandler( new SCR_InventoryStorageBackpackUI( storage, area, this, 0, null ) );
			m_pStorageBaseUI = SCR_InventoryStorageBaseUI.Cast( m_wContainer.FindHandler( SCR_InventoryStorageBaseUI ) );
		}

		m_aStorages.Insert( m_pStorageBaseUI );
	}

	//------------------------------------------------------------------------------------------------
	void ShowVicinity(bool compact = false) // if true, vicinity will have only 4 rows instead of 6
	{
		if (!m_pVicinity)
		{
			Print("No vicnity component on character!", LogLevel.DEBUG);
			return;
		}

		if ( m_wLootStorage )
		{
			m_wLootStorage.RemoveHandler( m_wLootStorage.FindHandler( SCR_InventoryStorageLootUI ) );	//remove the handler from the widget
			m_wLootStorage.RemoveFromHierarchy();
		}

		Widget parent = m_widget.FindAnyWidget( "StorageLootSlot" );
		m_wLootStorage =  GetGame().GetWorkspace().CreateWidgets( BACKPACK_STORAGE_LAYOUT, parent );
		if ( !m_wLootStorage )
			return;

		if (compact)
			m_wLootStorage.AddHandler( new SCR_InventoryStorageLootUI( null, null, this, 0, null, m_Player, 4, 6 ) );
		else
			m_wLootStorage.AddHandler( new SCR_InventoryStorageLootUI( null, null, this, 0, null, m_Player ) );
		m_pStorageLootUI = SCR_InventoryStorageBaseUI.Cast( m_wLootStorage.FindHandler( SCR_InventoryStorageLootUI ) );
	}

	//------------------------------------------------------------------------------------------------
	void ShowAttachmentStorage(InventorySearchPredicate searchPredicate, bool closeOnly = false)
	{
		if (m_wAttachmentStorage)
		{
			m_wAttachmentStorage.RemoveHandler(m_wAttachmentStorage.FindHandler(SCR_InventoryAttachmentStorageUI));
			m_wAttachmentStorage.RemoveFromHierarchy();
			if (closeOnly)
				return;
		}

		Widget parent = m_widget.FindAnyWidget("AttachmentStorage");
		if (!parent)
			return;

		m_wAttachmentStorage = GetGame().GetWorkspace().CreateWidgets(BACKPACK_STORAGE_LAYOUT, parent);
		if (!m_wAttachmentStorage)
			return;

		SCR_InventoryAttachmentStorageUI handler = new SCR_InventoryAttachmentStorageUI(m_pStorageListUI.GetStorage(), null, this, 0, null, searchPredicate);
		m_wAttachmentStorage.AddHandler(handler);
		m_pAttachmentStorageUI = handler;
	}

	//------------------------------------------------------------------------------------------------
	void CloseAttachmentStorage()
	{
		if (m_wAttachmentStorage)
		{
			m_wAttachmentStorage.RemoveHandler(m_wAttachmentStorage.FindHandler(SCR_InventoryAttachmentStorageUI));
			m_wAttachmentStorage.RemoveFromHierarchy();
		}
	}

	//------------------------------------------------------------------------------------------------
	SCR_InventoryAttachmentStorageUI GetAttachmentStorageUI()
	{
		return m_pAttachmentStorageUI;
	}

	//------------------------------------------------------------------------------------------------
	void RemoveAttachmentStorage(SCR_InventoryAttachmentStorageUI attStorage)
	{
		CloseStorage(attStorage);
	}

	//------------------------------------------------------------------------------------------------
	// shows opened storage in a new container
	void OpenStorageAsContainer(BaseInventoryStorageComponent storage)
	{
		foreach (SCR_InventoryOpenedStorageUI openedStorage : m_aOpenedStoragesUI)
		{
			if (openedStorage.GetStorage() == storage)
			{
				// if storage is already open, close it instead
				RemoveOpenStorage(openedStorage);
				return;
			}
		}

		Widget parent = m_widget.FindAnyWidget("OpenedStorages");
		Widget newStorage = GetGame().GetWorkspace().CreateWidgets(BACKPACK_STORAGE_LAYOUT, parent);
		SCR_InventoryOpenedStorageUI handler = new SCR_InventoryOpenedStorageUI(storage, null, this, 0, {storage});
		newStorage.AddHandler(handler);
		ButtonWidget closeBtn = handler.ActivateCloseStorageButton();

		ScrollLayoutWidget scroll = ScrollLayoutWidget.Cast(m_widget.FindAnyWidget("LeftAreaScroll"));
		if (scroll)
			scroll.SetSliderPos(0, 0);

		m_aOpenedStoragesUI.Insert(handler);
		ShowVicinity(true);
	}

	//------------------------------------------------------------------------------------------------
	void RemoveOpenStorage(SCR_InventoryOpenedStorageUI openedStorage)
	{
		CloseStorage(openedStorage);
		m_aOpenedStoragesUI.RemoveItem(openedStorage);
		if (m_aOpenedStoragesUI.IsEmpty())
		{
			ShowVicinity();
		}
	}

	//------------------------------------------------------------------------------------------------
	void RefreshLootUIListener()
	{
		if (!m_pVicinity || !m_pStorageLootUI)
			return;
		
		m_pStorageLootUI.Refresh();
		if (m_aOpenedStoragesUI.IsEmpty())
			return;
		
		vector playerOrigin = m_Player.GetOrigin();
		vector entityBoundsMaxs, entityBoundsMins;
		SCR_InventoryOpenedStorageUI storageUI
		for (int index = m_aOpenedStoragesUI.Count() - 1; index >= 0; index--)
		{
			storageUI = m_aOpenedStoragesUI.Get(index);
			if (!storageUI)
				continue;
			
			storageUI.GetStorage().GetOwner().GetWorldBounds(entityBoundsMins, entityBoundsMaxs);
			if (!Math3D.IntersectionSphereAABB(playerOrigin, m_iVicinityDiscoveryRadius, entityBoundsMins, entityBoundsMaxs))
				RemoveOpenStorage(storageUI);
		}
	}

	//------------------------------------------------------------------------------------------------
	void ShowStoragesList()
	{
		if( m_wStorageList )
		{
			m_wStorageList.RemoveHandler( m_wStorageList.FindHandler( SCR_InventoryStoragesListUI ) );	//remove the handler from the widget
			m_wStorageList.RemoveFromHierarchy();
		}

		Widget parent = m_widget.FindAnyWidget( "StoragesListSlot" );
		m_wStorageList =  GetGame().GetWorkspace().CreateWidgets( STORAGES_LIST_LAYOUT, parent );

		if( !m_wStorageList )
			return;

		m_wStorageList.AddHandler( new SCR_InventoryStoragesListUI( m_StorageManager, null, this ) );
		m_pStorageListUI = SCR_InventoryStoragesListUI.Cast( m_wStorageList.FindHandler( SCR_InventoryStoragesListUI ) );
	}

	//------------------------------------------------------------------------------------------------
	void ShowAllStoragesInList()
	{
		m_iStorageListCounter = 0;
		array<SCR_InventorySlotUI> aSlotsUI = {};
		m_pStorageListUI.GetSlots( aSlotsUI );
		if ( !m_aStorages.IsEmpty() )
		{
			foreach ( SCR_InventoryStorageBaseUI pStorage : m_aStorages )
			{
				if ( !pStorage )
					continue;
				CloseStorage( pStorage );
			}
		}
		SortSlotsByLoadoutArea( aSlotsUI );
		m_aStorages.Resize( aSlotsUI.Count() );
		
		ShowEquipedWeaponStorage();
		
		foreach ( SCR_InventorySlotUI pSlot : aSlotsUI )
		{
			if ( !pSlot )
				continue;

			BaseInventoryStorageComponent pStorage = pSlot.GetAsStorage();
			if ( pStorage )
			{
				ShowStorage( pStorage, pSlot.GetLoadoutArea() );

				/* Enable to have the expand / collapse feature
				if ( m_StorageManager.GetIsStorageShown( pStorage ) )
					m_pStorageBaseUI.Show( true );
				else
					m_pStorageBaseUI.Show( false );
				*/
			}
		}
	}

	protected SCR_InventoryStorageBaseUI GetStorageUIFromVicinity(BaseInventoryStorageComponent storage)
	{
		array<SCR_InventorySlotUI> slots = {};
		m_pStorageLootUI.GetSlots(slots);
		foreach (SCR_InventorySlotUI slot : slots)
		{
			if (slot && slot.GetStorageComponent() == storage)
				return slot.GetStorageUI();
		}

		return null;
	}
	
	//------------------------------------------------------------------------------------------------
	protected SCR_InventoryStorageBaseUI GetStorageUIByBaseStorageComponent( BaseInventoryStorageComponent pStorage )
	{
		foreach ( SCR_InventoryStorageBaseUI pStorageUI : m_aStorages )
			if ( pStorageUI && pStorageUI.GetStorage() == pStorage )
				return pStorageUI;

		return null;
	}

	//------------------------------------------------------------------------------------------------
	protected void SortSlotsByLoadoutArea( out array<SCR_InventorySlotUI> aSlots )
	{
		array<SCR_InventorySlotUI> tmpSlots = {};
		tmpSlots.Copy( aSlots );
		aSlots.Clear();

		foreach ( SCR_InventorySlotUI pSlotUI : tmpSlots )
		{
			if ( !SCR_InventorySlotStorageUI.Cast( pSlotUI ) || pSlotUI.GetLoadoutArea() == null )
				continue;
			aSlots.Insert(pSlotUI);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void ShowGadgetStorage()
	{
		if( m_wGadgetStorage && m_wContainer )
		{
			m_wContainer.RemoveHandler( m_wGadgetStorage.FindHandler( SCR_InventoryStorageGadgetsUI ) );	//remove the handler from the widget
			m_wGadgetStorage.RemoveFromHierarchy();
		}

		Widget parent = m_widget.FindAnyWidget( "GadgetsGridSlot" );
		m_wGadgetStorage =  GetGame().GetWorkspace().CreateWidgets( GADGETS_STORAGE_LAYOUT, parent );

		if( !m_wGadgetStorage )
			return;

		//m_wGadgetStorage.AddHandler( new SCR_InventoryStorageGadgetsUI( m_StorageManager.GetGadgetsStorage(), ELoadoutArea.ELA_None, this ) );
		m_pGadgetsBaseUI = SCR_InventoryStorageGadgetsUI.Cast( m_wGadgetStorage.FindHandler( SCR_InventoryStorageGadgetsUI ) );
	}

	//------------------------------------------------------------------------------------------------
	protected void ShowEquipedWeaponStorage()
 	{
		
		if ( m_wWeaponSlots )
		{
			m_wWeaponSlots.RemoveHandler( m_wWeaponSlots.FindHandler( SCR_InventoryWeaponSlotsUI ) );	//remove the handler from the widget
			m_wWeaponSlots.RemoveFromHierarchy();
		}

		Widget parent = m_widget.FindAnyWidget( "WeaponSlots" );
		m_wWeaponSlots = GetGame().GetWorkspace().CreateWidgets( WEAPON_STORAGE_LAYOUT, parent );

		if ( !m_wWeaponSlots )
			return;

		m_wWeaponSlots.AddHandler( new SCR_InventoryWeaponSlotsUI( m_StorageManager.GetWeaponStorage(), null, this ) );
		m_pStorageBaseUI = SCR_InventoryWeaponSlotsUI.Cast( m_wWeaponSlots.FindHandler( SCR_InventoryWeaponSlotsUI ) );
		m_pWeaponStorage = SCR_InventoryWeaponSlotsUI.Cast( m_pStorageBaseUI );
		m_pWeaponStorageComp = EquipedWeaponStorageComponent.Cast( m_pWeaponStorage.GetStorage() );
 	}

	//------------------------------------------------------------------------------------------------
	void ShowItemInfo( string sName = "", string sDescr = "", float sWeight = 0.0, SCR_InventoryUIInfo uiInfo = null )
	{
		if ( !m_pItemInfo )
		{
			//Widget parent = m_widget.FindAnyWidget( "SoldierInfo" );
			Widget infoWidget = GetGame().GetWorkspace().CreateWidgets(ITEM_INFO, m_widget);
			if ( !infoWidget )
				return;

			infoWidget.AddHandler( new SCR_InventoryItemInfoUI() );
			m_pItemInfo = SCR_InventoryItemInfoUI.Cast( infoWidget.FindHandler( SCR_InventoryItemInfoUI ) );
		}

		if( !m_pItemInfo )
			return;

		Widget w = WidgetManager.GetWidgetUnderCursor();
		if (!w)
		{
			w = m_pFocusedSlotUI.GetButtonWidget();
		}

		m_pItemInfo.Show( 0.6, w, m_bIsUsingGamepad );
		m_pItemInfo.SetName( sName );
		m_pItemInfo.SetDescription( sDescr );
		m_pItemInfo.SetWeight( sWeight );
		if (uiInfo && uiInfo.IsIconVisible())
			m_pItemInfo.SetIcon(uiInfo.GetIconPath(), uiInfo.GetIconColor());

		int iMouseX, iMouseY;

		float x, y;
		w.GetScreenPos(x, y);

		float width, height;
		w.GetScreenSize(width, height);

		float screenSizeX, screenSizeY;
		GetGame().GetWorkspace().GetScreenSize(screenSizeX, screenSizeY);

		float infoWidth, infoHeight;
		m_pItemInfo.GetInfoWidget().GetScreenSize(infoWidth, infoHeight);

		iMouseX = x;
		iMouseY = y + height;
		if (x + infoWidth > screenSizeX)
			iMouseX = screenSizeX - infoWidth - width / 2; // offset info if it would go outside of the screen

		m_pItemInfo.Move( GetGame().GetWorkspace().DPIUnscale( iMouseX ), GetGame().GetWorkspace().DPIUnscale( iMouseY ) );
	}

	//------------------------------------------------------------------------------------------------
	void HideItemInfo()
	{
		if ( !m_pItemInfo )
			return;
		m_pItemInfo.Hide();
	}

	//------------------------------------------------------------------------------------------------
	void SetSlotFocused( SCR_InventorySlotUI pFocusedSlot, SCR_InventoryStorageBaseUI pFromStorageUI, bool bFocused )
	{
		if( bFocused )
		{
			InputManager pInputManager = GetGame().GetInputManager();
			if ( !( pInputManager && pInputManager.IsUsingMouseAndKeyboard() ) )
			{
				if ( m_pActiveStorageUI != pFromStorageUI )
				{
					if ( m_pActiveStorageUI )
					{
						SCR_InventorySlotUI pLastFocusedSlot = pFromStorageUI.GetLastFocusedSlot();
						pFromStorageUI.SetSlotFocused( pLastFocusedSlot );
						if ( pLastFocusedSlot )
						{
							m_bLocked = true;
							GetGame().GetWorkspace().SetFocusedWidget( pLastFocusedSlot.GetButtonWidget(), true );
							m_bLocked = false;
						}
					}

					m_pActiveStorageUI = pFromStorageUI;
				}
			}

			m_pFocusedSlotUI = pFocusedSlot;
			SetFocusedSlotEffects();
		}
		else
		{
			if ( !m_bLocked )
			{
				if ( m_pActiveStorageUI )
					m_pActiveStorageUI.SetLastFocusedSlot( pFocusedSlot	 );

				HideItemInfo();
				m_pFocusedSlotUI = null;
				NavigationBarUpdate();
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void SetFocusedSlotEffects()
	{
		if( !m_pFocusedSlotUI )
		{
			if( m_wProgressBarWeightItem )
				m_wProgressBarWeightItem.SetCurrent( 0.0 );
			return;
		}

		//show info about the item
		InventoryItemComponent invItemComp = m_pFocusedSlotUI.GetInventoryItemComponent();
		if ( !invItemComp )
			return;
		auto attribs = SCR_ItemAttributeCollection.Cast( invItemComp.GetAttributes() );

		if ( !attribs )
			return;
		UIInfo itemInfo = attribs.GetUIInfo();
		IEntity ie = invItemComp.GetOwner();
		MoneyComponent mc = MoneyComponent.Cast(ie.FindComponent(MoneyComponent));
		int value
		if(mc)
			value = mc.GetValue();
		if ( !itemInfo )
			HideItemInfo();
		else
		{
			if(mc)
				ShowItemInfo( itemInfo.GetName(), value.ToString(), invItemComp.GetTotalWeight(), SCR_InventoryUIInfo.Cast(itemInfo) );
			else
				ShowItemInfo( itemInfo.GetName(), itemInfo.GetDescription(), invItemComp.GetTotalWeight(), SCR_InventoryUIInfo.Cast(itemInfo) );
		}
		
		
		//show the weight on the progressbar
		//TODO: overlap or add on the end, depending on if the item is already in the storage or is going to be added
		if( m_wProgressBarWeightItem )
		{
			float weight = invItemComp.GetTotalWeight();
			m_wProgressBarWeightItem.SetCurrent( weight );
		};

		NavigationBarUpdate();
	}

	//------------------------------------------------------------------------------------------------
	//!
	protected EDropContainer IsFocusedItemInsideDropContainer()
	{
		if ( !m_pActiveHoveredStorageUI )
			return EDropContainer.NOCONTAINER;
		if ( m_pActiveHoveredStorageUI.GetFocusedSlot() == m_pFocusedSlotUI )
			return EDropContainer.ISINSIDE;

		return EDropContainer.ISNOTINSIDE;
	}

	//------------------------------------------------------------------------------------------------
	protected bool CanSwapItems(SCR_InventorySlotUI slot1, SCR_InventorySlotUI slot2)
	{
		if (!slot1 || !slot2)
			return false;

		return m_InventoryManager.CanSwapItemStorages(
			slot1.GetInventoryItemComponent().GetOwner(),
			slot2.GetInventoryItemComponent().GetOwner()
		);
	}

	//------------------------------------------------------------------------------------------------
	protected bool CanMoveItem(SCR_InventorySlotUI slot1, SCR_InventorySlotUI slot2)
	{
		if (!slot1 || !slot2)
			return false;

		return m_InventoryManager.CanMoveItemToStorage(
			slot1.GetInventoryItemComponent().GetOwner(),
			slot2.GetStorageComponent()
		);
	}

	//------------------------------------------------------------------------------------------------
	protected bool CanMoveItem(SCR_InventorySlotUI slot1, SCR_InventoryStorageBaseUI slot2)
	{
		if (!slot1 || !slot2)
			return false;

		return m_InventoryManager.CanMoveItemToStorage(
			slot1.GetInventoryItemComponent().GetOwner(),
			slot2.GetStorage()
		);
	}

	//------------------------------------------------------------------------------------------------
	void NavigationBarUpdate()
	{
		if (!m_pNavigationBar)
			return;

		if (m_bIsUsingGamepad)
		{
			NavigationBarUpdateGamepad();
			return;
		}

		m_pNavigationBar.SetAllButtonEnabled( false );
		m_pNavigationBar.SetButtonEnabled( "ButtonClose", true );

		if ( !m_pFocusedSlotUI )
			return;
		if ( m_pFocusedSlotUI.GetStorageUI() == m_pQuickSlotStorage )
			return;
		
		InventoryItemComponent itemComp = m_pFocusedSlotUI.GetInventoryItemComponent();
		bool arsenalItem = IsStorageArsenal(m_pFocusedSlotUI.GetStorageUI().GetCurrentNavigationStorage());
		if (itemComp && itemComp.GetOwner() && !arsenalItem)
			m_pNavigationBar.SetButtonEnabled("ButtonInspect", (itemComp.GetOwner().FindComponent(SCR_WeaponAttachmentsStorageComponent) != null));
		
		m_pNavigationBar.SetButtonEnabled( "ButtonSelect", true );
		m_pNavigationBar.SetButtonEnabled( "ButtonDrop", m_pFocusedSlotUI.IsDraggable() );

		bool flag = m_pFocusedSlotUI.GetStorageUI() == m_pStorageLootUI;
		m_pNavigationBar.SetButtonEnabled("ButtonPickup", flag);
		m_pNavigationBar.SetButtonEnabled("ButtonDrop", !flag);
		
		m_pNavigationBar.SetButtonEnabled( "ButtonStepBack", true );
	
		HandleSlottedItemFunction();
	}

	//------------------------------------------------------------------------------------------------
	void HandleSlottedItemFunction()
	{
		string sAction = "#AR-Inventory_Select";
		bool arsenalItem = IsStorageArsenal(m_pFocusedSlotUI.GetStorageUI().GetCurrentNavigationStorage()); // hotfix for disabling opening open action in arsenal storages

		switch ( m_pFocusedSlotUI.GetSlotedItemFunction() )
		{
			case ESlotFunction.TYPE_GADGET:
			{
				// m_pNavigationBar.SetButtonEnabled( "ButtonEquip", true );
			} break;

			case ESlotFunction.TYPE_WEAPON:
			{
				m_pNavigationBar.SetButtonEnabled( "ButtonEquip", true );
				InventoryItemComponent itemComp = m_pFocusedSlotUI.GetInventoryItemComponent();
				if (!itemComp)
				 	return;
				
				IEntity item = itemComp.GetOwner();
				if (item)
				{
					WeaponComponent weaponComp = WeaponComponent.Cast(item.FindComponent(WeaponComponent));
					if (weaponComp &&
						weaponComp.GetWeaponType() != EWeaponType.WT_FRAGGRENADE &&
						weaponComp.GetWeaponType() != EWeaponType.WT_SMOKEGRENADE &&
						weaponComp.Type() != SCR_MineWeaponComponent)
					{
						m_pNavigationBar.SetButtonEnabled( "ButtonOpenStorage", !arsenalItem );
						m_pNavigationBar.SetButtonEnabled( "ButtonOpenAsContainer", !arsenalItem );
					}
				}
			} break;

			case ESlotFunction.TYPE_MAGAZINE:
			{
				// TODO: show the Reload action
				//m_pNavigationBar.SetButtonEnabled( "ButtonUse", true );
			} break;

			case ESlotFunction.TYPE_CONSUMABLE:
			{
				// TODO: show the Consume action
				m_pNavigationBar.SetButtonEnabled( "ButtonUse", true );
			} break;

			case ESlotFunction.TYPE_STORAGE:
			{
				if( m_EStateMenuItem == EStateMenuItem.STATE_MOVING_ITEM_STARTED && m_pFocusedSlotUI != m_pSelectedSlotUI )
				{
					sAction = "#AR-Inventory_Move";
					//m_pNavigationBar.SetButtonEnabled( "ButtonSelect", false );
					//m_pNavigationBar.SetButtonEnabled( "ButtonMove", true );
				}
				// Enable in case the storage is not "togglable" - can be only shown and only opening another storage will close it
				/*else if ( m_EStateMenuStorage == EStateMenuStorage.STATE_OPENED && m_pFocusedSlotUI == m_pSelectedSlotUI && m_pFocusedSlotUI.Type() != SCR_InventorySlotStorageEmbeddedUI)
				{
					m_pNavigationBar.SetButtonEnabled( "ButtonSelect", false );
				}*/
				else if ( m_pFocusedSlotUI.Type() == SCR_InventorySlotStorageEmbeddedUI )
				{
					m_pNavigationBar.SetButtonEnabled( "ButtonOpenStorage", !arsenalItem );
					m_pNavigationBar.SetButtonEnabled( "ButtonOpenAsContainer", !arsenalItem );
				}
			} break;

			case ESlotFunction.TYPE_HEALTH:
			{
				// TODO: show the Heal action
				m_pNavigationBar.SetButtonEnabled( "ButtonUse", true );
			} break;
		}

		HandleSelectButtonState( sAction );
	}

	//------------------------------------------------------------------------------------------------
	void NavigationBarUpdateGamepad()
	{
		m_pNavigationBar.SetAllButtonEnabled(false);
		m_pNavigationBar.SetButtonEnabled("ButtonSelect", true);

		if (m_pActiveStorageUI == m_pAttachmentStorageUI)
		{
			m_pNavigationBar.SetButtonEnabled("ButtonUse", true);
			return;
		}

		if (m_bStorageSwitchMode)
		{
			m_CloseButton.SetLabel("#AR-Inventory_Close");
			bool shouldShowMove = (m_pSelectedSlotUI != null) && m_pSelectedSlotUI.IsDraggable();
			if (m_pActiveStorageUI)
				shouldShowMove &= m_pActiveStorageUI.IsStorageHighlighted();
			m_pNavigationBar.SetButtonEnabled("ButtonMove", shouldShowMove);
			m_pNavigationBar.SetButtonEnabled("ButtonSelect", !m_pSelectedSlotUI);
		}
		else
		{
			m_pNavigationBar.SetButtonEnabled("ButtonMove", m_pSelectedSlotUI != null);
			m_pNavigationBar.SetButtonEnabled("ButtonSwap", m_pSelectedSlotUI != null);
		}

		if (!m_bStorageSwitchMode &&
			m_pActiveStorageUI != m_pStorageLootUI &&
			m_pActiveStorageUI != m_pStorageListUI)
		{
			m_pNavigationBar.SetButtonEnabled("ButtonQuickSlotAssign", true);
		}

		bool isQuickSlotStorage = (m_pActiveStorageUI == m_pQuickSlotStorage);
		if (isQuickSlotStorage)
		{
			bool itmToAssign = m_pItemToAssign != null;
			m_pNavigationBar.SetAllButtonEnabled(false);
			m_pNavigationBar.SetButtonEnabled("ButtonClose", true);
			if (m_bStorageSwitchMode)
				m_pNavigationBar.SetButtonEnabled("ButtonSelect", true);

			m_pNavigationBar.SetButtonEnabled("ButtonQuickSlotAssign", itmToAssign);
			m_pNavigationBar.SetButtonEnabled("ButtonQuickSlotUnassign",
				!itmToAssign &&
				m_pFocusedSlotUI != null
			);
		}

		if (!m_pFocusedSlotUI)
			return;

		InventoryItemComponent itemComp = m_pFocusedSlotUI.GetInventoryItemComponent();
		bool arsenalItem = IsStorageArsenal(m_pFocusedSlotUI.GetStorageUI().GetCurrentNavigationStorage());
		if (itemComp && itemComp.GetOwner() && !arsenalItem)
			m_pNavigationBar.SetButtonEnabled("ButtonInspect", (itemComp.GetOwner().FindComponent(SCR_WeaponAttachmentsStorageComponent) != null));

		if (m_CloseButton)
			m_CloseButton.SetLabel("#AR-Menu_Back");

		m_pNavigationBar.SetButtonEnabled("ButtonDrop",
			(m_pFocusedSlotUI != null) &&
			!isQuickSlotStorage &&
			m_pFocusedSlotUI.IsDraggable() &&
			!m_AttachmentSpinBox.IsFocused()
		);

		bool flag = m_pFocusedSlotUI.GetStorageUI() == m_pStorageLootUI;
		m_pNavigationBar.SetButtonEnabled("ButtonPickup", flag);
		
		if (!isQuickSlotStorage)
		{
			m_pNavigationBar.SetButtonEnabled("ButtonDrop", !flag);
			HandleSlottedItemFunction();
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void HandleSelectButtonState( string sAction = "#AR-Inventory_Select" )
	{
		//TODO: this can be done better
		if ( sAction == "#AR-Inventory_Move" )
			m_pNavigationBar.SetButtonActionName( "ButtonSelect", sAction );
		else
		{
			if ( !m_pFocusedSlotUI.IsSlotSelected() )
				m_pNavigationBar.SetButtonActionName( "ButtonSelect", sAction );
			else
				m_pNavigationBar.SetButtonActionName( "ButtonSelect", "#AR-Inventory_Deselect" );
		}
	}

	//------------------------------------------------------------------------------------------------
	//! Returns the inspection screen UI
	SCR_InventoryInspectionUI GetInspectionScreen()
	{
		return m_InspectionScreen;
	}

	//------------------------------------------------------------------------------------------------
	//! Opens the inspection screen for the item slot
	void InspectItem(SCR_InventorySlotUI itemSlot)
	{
		if (m_pGearInspectionPointUI)
		{	
			m_pGearInspectionPointUI.ClearSlots();
			m_wAttachmentContainer.RemoveHandler(m_pGearInspectionPointUI);
			m_pGearInspectionPointUI = null;
			m_pInspectedSlot = null;
		}

		if (!itemSlot)
		{
			CloseAttachmentStorage();
			SetAttachmentSpinBoxActive(m_bIsUsingGamepad);
			m_PlayerRenderAttributes.RotateItemCamera(Vector(0, 0, 0), Vector(0, 0, 0), Vector(0, 0, 0)); // reset rotation
			InitializeCharacterHitZones();
			UpdateCharacterPreview();
			return;
		}
		
		InventoryItemComponent itemComp = itemSlot.GetInventoryItemComponent();
		
		if (!itemComp)
		{
			InitializeCharacterHitZones();
			CloseAttachmentStorage();
			SetAttachmentSpinBoxActive(m_bIsUsingGamepad);
			UpdateCharacterPreview(); 
			return;
		}
		
		IEntity item = itemComp.GetOwner();
		
		SCR_WeaponAttachmentsStorageComponent weaponAttachmentStorage = SCR_WeaponAttachmentsStorageComponent.Cast(item.FindComponent(SCR_WeaponAttachmentsStorageComponent));
		
		if (weaponAttachmentStorage)
		{
			m_PlayerRenderAttributes.RotateItemCamera(Vector(0, 90, 0), Vector(0, 90, 0), Vector(0, 90, 0)); // rotate inspected weapon to side view
			m_pInspectedSlot = itemSlot;
			InspectWeapon(weaponAttachmentStorage);
			UpdateGearInspectionPreview();
			return;	
		}
		
		/*	Preparations for gear inspection
		ClothNodeStorageComponent clothNodeStorage = ClothNodeStorageComponent.Cast(item.FindComponent(ClothNodeStorageComponent));
		
		if (clothNodeStorage)
		{
			m_pInspectedSlot = itemSlot;
			InspectGear(clothNodeStorage);
			UpdateGearInspectionPreview();
			return;
		}*/
	}
	
	//------------------------------------------------------------------------------------------------
	void InspectWeapon(SCR_WeaponAttachmentsStorageComponent weaponAttachmentStorage)
	{
		CloseAttachmentStorage();
		HideCharacterHitZones();
		SetAttachmentSpinBoxActive(m_bIsUsingGamepad);
		Widget inspectionFrame = m_widget.FindAnyWidget("AttachmentPoints");
		m_wAttachmentContainer = GetGame().GetWorkspace().CreateWidgets(BACKPACK_STORAGE_LAYOUT, inspectionFrame);
		m_pGearInspectionPointUI = new SCR_InventoryGearInspectionPointUI(weaponAttachmentStorage, null, this, frameSlot: inspectionFrame);
		m_wAttachmentContainer.AddHandler(m_pGearInspectionPointUI);
	}
	
	/*	Preparations for gear inspection
	void InspectGear(ClothNodeStorageComponent clothNodeStorage)
	{
		Widget inspectionFrame = m_widget.FindAnyWidget("AttachmentPoints");
		m_wAttachmentContainer = GetGame().GetWorkspace().CreateWidgets(BACKPACK_STORAGE_LAYOUT, inspectionFrame);
		m_pGearInspectionPointUI = new SCR_InventoryGearInspectionPointUI(clothNodeStorage, null, this, frameSlot: inspectionFrame);
		m_wAttachmentContainer.AddHandler(m_pGearInspectionPointUI);
	}*/
	
	SCR_InventoryGearInspectionPointUI GetGearInspectionUI()
	{
		return m_pGearInspectionPointUI;
	}

	//------------------------------------------------------------------------------------------------
	//! shows only the storages the item can be stored into
	protected void FilterOutStorages( bool bShow = true )
	{
		//Get all slots from the storage list UI
		array<SCR_InventorySlotUI> pSlotsInListUI = new array<SCR_InventorySlotUI>();
		m_pStorageListUI.GetSlots( pSlotsInListUI );
		//foreach ( SCR_InventorySlotUI pSlotFromUI : pSlotsInListUI )
		//	RegisterUIStorage( pSlotFromUI );

		if ( m_pStorageLootUI )
			m_pStorageLootUI.GetSlots( pSlotsInListUI );
		if ( GetActualStorageInCharacterStorageUI() )
			GetActualStorageInCharacterStorageUI().GetSlots( pSlotsInListUI );
		if ( m_pQuickSlotStorage )
			m_pQuickSlotStorage.GetSlots( pSlotsInListUI );
		if ( m_pWeaponStorage )
			m_pWeaponStorage.GetSlots( pSlotsInListUI );

		BaseInventoryStorageComponent pStorageTo;
		foreach ( SCR_InventorySlotUI pStorageSlotUI : pSlotsInListUI )
		{
			if ( !pStorageSlotUI )
				continue;

			pStorageTo = pStorageSlotUI.GetAsStorage();

			if ( bShow )
			{
				if ( !m_pSelectedSlotUI )
					continue;
				if ( pStorageSlotUI.GetAsStorage() == m_pSelectedSlotUI.GetStorageUI().GetCurrentNavigationStorage() )	//it's the same storage as the selected item comes from
				{
					pStorageSlotUI.SetEnabledForMove( 0 );
					continue;
				}

				InventoryItemComponent pInventoryItem = m_pSelectedSlotUI.GetInventoryItemComponent();
				if ( !pInventoryItem )
					continue;

				IEntity pItem = pInventoryItem.GetOwner();

				if ( pStorageTo )
				{
					bool bCanStore = true;

					if ( m_InventoryManager.CanInsertItemInActualStorage( pItem, pStorageTo ) )
					{
						if ( pStorageTo.Type() == SCR_InventoryStorageWeaponsUI )
							if ( !m_pWeaponStorageComp.Contains( pStorageTo.GetOwner() ) )
								bCanStore = false;

						if ( bCanStore )
							pStorageSlotUI.SetEnabledForMove( 1 );
						else
							pStorageSlotUI.SetEnabledForMove( 0 );
					}
					else
					{
						pStorageSlotUI.SetEnabledForMove( 0 );
					}
				}
				else
				{
					pStorageSlotUI.SetEnabledForMove( 0 );
				}

			}
			else
			{
				pStorageSlotUI.SetEnabledForMove( 2 );
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void FilterOutItems( bool bFilterOut )
	{
		array<SCR_InventorySlotUI> aItemsUIInLoot = {};

		//if ( m_pStorageLootUI )
			//aItemsUIInLoot.InsertAll( m_pStorageLootUI.GetUISlots() );
		if ( m_pStorageListUI )
			aItemsUIInLoot.InsertAll( m_pStorageListUI.GetUISlots() );
		//if ( m_pStorageBaseUI )
			//aItemsUIInLoot.InsertAll( m_pStorageBaseUI.GetUISlots() );

		PrintFormat( "INV: Filtering out items %1", 1.5 );

		foreach ( SCR_InventorySlotUI pSlot : aItemsUIInLoot )
		{
			if ( !pSlot.GetStorageComponent() )
			{
				if ( bFilterOut )
				{
					PrintFormat( "INV: Disabling slot %1", pSlot );
					pSlot.SetEnabledForMove( 0 );
				}
				else
				{
					PrintFormat( "INV: Reseting slot %1", pSlot );
					pSlot.SetEnabledForMove( 2 );
				}
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void Action_TryCloseInventory()
	{
		if (m_bWasJustTraversing)
		{
			m_bWasJustTraversing = false;
			return;
		}

		if (m_bIsUsingGamepad)
		{
			if (!m_bStorageSwitchMode)
			{
				SetStorageSwitchMode(true);
				return;
			}
			else
			{
				if (m_pSelectedSlotUI)
				{
					DeselectSlot();				
				}
				else
				{
					Action_CloseInventory();
					return;
				}

				SetStorageSwitchMode(false);
				ResetHighlightsOnAvailableStorages();
				FocusOnSlotInStorage(m_pActiveStorageUI);
				return;
			}
		}
		else
		{
			DeselectSlot();
		}

		Action_CloseInventory();
	}

	//------------------------------------------------------------------------------------------------
	protected void Action_CloseInventory()
	{
		array<BaseInventoryStorageComponent> traverseStorage = {};
		if (m_wLootStorage)
		{
			SCR_InventoryStorageBaseUI storageUIHandler = SCR_InventoryStorageBaseUI.Cast( m_wLootStorage.FindHandler( SCR_InventoryStorageBaseUI ) );
			storageUIHandler.GetTraverseStorage(traverseStorage);
		}

		if (!traverseStorage.IsEmpty())
		{
			BaseInventoryStorageComponent storage = traverseStorage[0];
			if (storage)
			{
				IEntity entity = storage.GetOwner();
				m_InventoryManager.PlayItemSound(entity, SCR_SoundEvent.SOUND_CONTAINER_CLOSE);
			}
		}
		else
		{
			SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_CLOSE);
		}
		
		GetGame().GetInputManager().RemoveActionListener("Inventory_Drag", EActionTrigger.DOWN, Action_DragDown);
		GetGame().GetInputManager().RemoveActionListener("Inventory", EActionTrigger.DOWN, Action_CloseInventory);
		//m_bColdStart = false; 
		if (m_pVicinity)
		{
			m_pVicinity.ManipulationComplete();
			m_iVicinityDiscoveryRadius = 0;
		}
		
		/*
		TODO: enable once the performance issue with invokers is solved in characterdamagemanager
		if ( m_CharDamageManager )
			m_CharDamageManager.GetOnDamageStateChanged().Remove( OnDamageStateChanged );
		*/

		auto playerController = GetGame().GetPlayerController();
		if (!playerController)
			return;

		auto menuManager = GetGame().GetMenuManager();
		auto menu = ChimeraMenuPreset.Inventory20Menu;

		auto inventoryMenu = menuManager.FindMenuByPreset( menu ); // prototype inventory
		if (inventoryMenu)
			menuManager.CloseMenuByPreset( menu );
		if  (m_PlayerRenderAttributes)
		{
			m_PlayerRenderAttributes.ResetDeltaRotation();
		}
		if (m_Player)
		{
			m_CharController = SCR_CharacterControllerComponent.Cast(m_Player.FindComponent(SCR_CharacterControllerComponent));
			if (m_CharController)
			{
				m_CharController.m_OnPlayerDeath.Remove(Action_CloseInventory);
			}
		}
		if (m_pCharacterWidgetHelper)
			m_pCharacterWidgetHelper.Destroy();
		m_pCharacterWidgetHelper = null;

		HideItemInfo();
		if ( m_pItemInfo )
			m_pItemInfo.Destroy();
		m_pItemInfo = null;

		if (m_InventoryManager)
			m_InventoryManager.OnInventoryMenuClosed();
	}

	//------------------------------------------------------------------------------------------------
	//!
	protected void SimpleFSM( EMenuAction EAction = EMenuAction.ACTION_SELECT  )
	{
		switch (EAction)
		{
			case EMenuAction.ACTION_MOVEINSIDE:
			{
				Action_MoveItemToStorage(m_pActiveStorageUI);
				if (m_bIsUsingGamepad)
					SetStorageSwitchMode(true);
				NavigationBarUpdate();
				if (m_pSelectedSlotUI)
				{
					m_pSelectedSlotUI.SetSelected(false);
					m_pSelectedSlotUI = null;
				}
				FocusOnSlotInStorage(m_pActiveStorageUI);
				ResetHighlightsOnAvailableStorages();
			} break;

			case EMenuAction.ACTION_SELECT:
			{
				if (!m_pFocusedSlotUI)
					return;

				if (m_pSelectedSlotUI)
					m_pSelectedSlotUI.SetSelected(false);

				if (m_bIsUsingGamepad)
				{
					m_pSelectedSlotUI = m_pFocusedSlotUI;
					m_pSelectedSlotUI.SetSelected(true);
					HighlightAvailableStorages(m_pSelectedSlotUI);
				}

				NavigationBarUpdate();
				if (m_bIsUsingGamepad)
					SetStorageSwitchMode(true);
			} break;

			case EMenuAction.ACTION_DESELECT:
			{
				if (m_pSelectedSlotUI)
				{
					ResetHighlightsOnAvailableStorages();
					m_pSelectedSlotUI.SetSelected(false);
					m_pSelectedSlotUI = null;
				}

				NavigationBarUpdate();
			} break;

			case EMenuAction.ACTION_DRAGGED:
			{
				m_EStateMenuItem = EStateMenuItem.STATE_MOVING_ITEM_STARTED;
			} break;

			case EMenuAction.ACTION_DROPPED:
			{
				Action_Drop();
				if (m_pActiveHoveredStorageUI)
					m_pActiveHoveredStorageUI.ShowContainerBorder(false);
				m_EStateMenuItem = EStateMenuItem.STATE_IDLE;
				return;
			} break;

			case EMenuAction.ACTION_MOVEBETWEEN:
			{
				if (m_pFocusedSlotUI == m_pInspectedSlot)
				{
					InspectItem(null);
				}			
				
				if (m_pFocusedSlotUI)
				{
					m_pSelectedSlotUI = m_pFocusedSlotUI;
					SCR_InventoryStorageBaseUI pStorage = m_pFocusedSlotUI.GetStorageUI();

					if (pStorage != m_pStorageLootUI)
					{
						MoveBetweenToVicinity();
						SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_VINICITY_DROP_CLICK);
					}
					else
					{
						MoveBetweenFromVicinity();
						SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_PICKUP_CLICK);
					}
					m_pSelectedSlotUI = null;
				}
				ResetHighlightsOnAvailableStorages();
			} break;

			case EMenuAction.ACTION_UNFOLD:
			{
				if ( m_pFocusedSlotUI.GetStorageUI() == m_pStorageListUI ) //if it is slot in the "storage list ui"
				{
					if ( BaseInventoryStorageComponent.Cast( m_pFocusedSlotUI.GetAsStorage() ) )	// and if it is a storage
					{
						//ShowStorage( m_pFocusedSlotUI.GetAsStorage() ); 		//show the content of the actualy selected
						ToggleStorageContainerVisibility( m_pFocusedSlotUI );
						m_EStateMenuStorage = EStateMenuStorage.STATE_OPENED;
					}
					else
					{
						//CloseOpenedStorage();	// if it is not storage, show nothing
						//ToggleStorageContainerVisibility( m_pFocusedSlotUI.GetAsStorage() );
						m_EStateMenuStorage = EStateMenuStorage.STATE_IDLE;
					}
					
				}
				else
				{
					TraverseActualSlot();
					NavigationBarUpdate();
					m_EStateMenuStorage = EStateMenuStorage.STATE_OPENED;
				}
			} break;

			case EMenuAction.ACTION_OPENCONTAINER:
			{
				OpenAsNewContainer();
			} break;
		}

		if (!m_bIsUsingGamepad)
			m_pSelectedSlotUI = m_pFocusedSlotUI;

		HideItemInfo();
	}

	//------------------------------------------------------------------------------------------------
	protected void Action_OpenContainer()
	{
		SimpleFSM(EMenuAction.ACTION_OPENCONTAINER);
	}

	//------------------------------------------------------------------------------------------------
	//!
	protected void Action_Drop()
	{
		bool onDrop = false;
		if (m_pFocusedSlotUI)
			onDrop = m_pFocusedSlotUI.OnDrop(m_pSelectedSlotUI);

		if (WidgetManager.GetWidgetUnderCursor() && WidgetManager.GetWidgetUnderCursor().GetName() == "HitZoneButton")
		{
			SCR_InventoryHitZonePointContainerUI hzContainer = SCR_InventoryHitZonePointContainerUI.Cast(m_AttachmentSpinBox.GetCurrentItemData());
			if (m_pSelectedSlotUI && hzContainer)
			{
				hzContainer.GetStorage().OnDrop(m_pSelectedSlotUI);
			}
		}
		
		if ( !onDrop && IsFocusedItemInsideDropContainer() != EDropContainer.NOCONTAINER )		//dropped to a container
		{
			if (m_pSelectedSlotUI == m_pInspectedSlot)
				InspectItem(null);
			
			if ( m_pFocusedSlotUI )
			{
				if ( IsFocusedItemInsideDropContainer() == EDropContainer.ISINSIDE )
				{
					if ( SCR_InventorySlotStorageUI.Cast( m_pFocusedSlotUI ) )	// storage
					{
						MoveItemToStorageSlot();
					}
					else if ( SCR_InventorySlotWeaponSlotsUI.Cast( m_pFocusedSlotUI ) )	// weapon slot
					{
						EquipWeaponIntoWeaponSlot();
					}
					else if ( SCR_InventorySlotUI.Cast( m_pFocusedSlotUI ) )	// simple slot
					{
						if ( SCR_InventoryStorageQuickSlotsUI.Cast( m_pFocusedSlotUI.GetStorageUI() ) )	//quick slot
						{
							//m_pFocusedSlotUI.m_iQuickSlotIndex
							SetItemToQuickSlotDrop();
							SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_HOTKEY_CONFIRM);
						}
						else
						{
							MoveItemToStorageSlot();
							/*
							if ( m_pFocusedSlotUI.GetInventoryItemComponent() )
							{
								MoveItemToStorageSlot();
							}
							else
							{
								//we are inserting into empy slot
								m_InventoryManager.EquipGadget( m_pSelectedSlotUI.GetInventoryItemComponent().GetOwner() );
							}
								*/
						}

					}
				}
			}
			else
			{
				// just container
				if (m_pActiveHoveredStorageUI)
					m_pActiveHoveredStorageUI.OnDrop(m_pSelectedSlotUI);
				if (!onDrop)
					MoveItem();
			}
		}
		else
		{
			if (WidgetManager.GetWidgetUnderCursor() == m_wPlayerRender)
			{
				EquipDraggedItem(true);
			}
			else
			{
				//dropped outside of a container
				RemoveItemFromQuickSlotDrop();
			}
		}
		
		ResetHighlightsOnAvailableStorages();
	}

	//------------------------------------------------------------------------------------------------
	void Action_QuickSlotAssign()
	{
		if (m_pItemToAssign && m_pActiveStorageUI == m_pQuickSlotStorage)
		{
			int slotId = m_pQuickSlotStorage.GetFocusedSlotId() + 1;
			SetItemToQuickSlot(slotId, m_pItemToAssign);
			FocusOnSlotInStorage(m_pQuickSlotStorage, slotId - 1);
			m_pItemToAssign = null;
		}
		else
		{
			if (m_pActiveStorageUI != m_pStorageLootUI && m_pActiveStorageUI != m_pQuickSlotStorage)
			{
				m_pItemToAssign = m_pFocusedSlotUI;
				FocusOnSlotInStorage(m_pQuickSlotStorage, 3);
			}
		}

		NavigationBarUpdate();
	}

	//------------------------------------------------------------------------------------------------
	void Action_QuickSlotUnassign()
	{
		if (!m_pFocusedSlotUI)
			return;

		InventoryItemComponent itemComp = m_pFocusedSlotUI.GetInventoryItemComponent();
		if (!itemComp)
			return;

		IEntity item = itemComp.GetOwner();
		if (!item)
			return;

		int slotId = m_pQuickSlotStorage.GetFocusedSlotId();
		slotId = Math.Clamp(slotId, 0, m_StorageManager.GetQuickSlotItems().Count());
		m_pItemToAssign = null;
		m_StorageManager.RemoveItemFromQuickSlot(item);
		ShowQuickSlotStorage();
		FocusOnSlotInStorage(m_pQuickSlotStorage, slotId);
		NavigationBarUpdate();
	}

	//------------------------------------------------------------------------------------------------
	//!
	void MoveItemToStorageSlot()
	{
		if (!m_pSelectedSlotUI)
			return;
		
		InventoryItemComponent pComp = m_pSelectedSlotUI.GetInventoryItemComponent();
		if (!pComp)
			return;

		IEntity pItem = pComp.GetOwner();
		if (!m_InventoryManager.CanMoveItem(pItem))
			return;
		m_pCallBack.m_pItem = pItem;
		m_pCallBack.m_pMenu = this;
		m_pCallBack.m_pStorageFrom = m_pSelectedSlotUI.GetStorageUI();
		m_pCallBack.m_pStorageTo = m_pFocusedSlotUI.GetStorageUI();

		BaseInventoryStorageComponent pStorageFromComponent = m_pSelectedSlotUI.GetStorageUI().GetCurrentNavigationStorage();
		BaseInventoryStorageComponent pStorageToComponent = m_pFocusedSlotUI.GetAsStorage();
		if (IsStorageArsenal(pStorageToComponent))
		{
			SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_DROP_ERROR);
			return;
		}
		
		bool shouldEquip = (m_pFocusedSlotUI.GetStorageUI() == m_pStorageListUI);
		bool equip = (shouldEquip && m_InventoryManager.EquipAny( m_StorageManager , pItem, 0, m_pCallBack ));
		
		if (!equip)
		{
			m_pCallBack.m_pStorageTo = GetOpenedStorage(pStorageToComponent);
			m_InventoryManager.InsertItem( pItem, pStorageToComponent, pStorageFromComponent, m_pCallBack );
		}
		else
		{
			m_InventoryManager.PlayItemSound(pItem, SCR_SoundEvent.SOUND_EQUIP);
		}
		/*
		if ( pItem.FindComponent( SCR_GadgetComponent ) )
			m_InventoryManager.EquipGadget( pItem, m_pCallBack );
		else
		*/
	}
	
	//------------------------------------------------------------------------------------------------
	//!
	void EquipWeaponIntoWeaponSlot()
	{
		if (!m_pSelectedSlotUI)
			return;
		
		InventoryItemComponent pComp = m_pSelectedSlotUI.GetInventoryItemComponent();
		if (!pComp)
			return;

		SCR_InventorySlotWeaponSlotsUI weaponSlot = SCR_InventorySlotWeaponSlotsUI.Cast(m_pFocusedSlotUI);
		if (!weaponSlot)
			return;
		
		IEntity pItem = pComp.GetOwner();
		if (!m_InventoryManager.CanMoveItem(pItem))
			return;
		
		m_pCallBack.m_pItem = pItem;
		m_pCallBack.m_pMenu = this;
		m_pCallBack.m_pStorageFrom = m_pSelectedSlotUI.GetStorageUI();
		m_pCallBack.m_pStorageTo = m_pFocusedSlotUI.GetStorageUI();
		
		InventoryItemComponent pItemToReplaceComp = m_pFocusedSlotUI.GetInventoryItemComponent();
		IEntity pItemToReplace;
		if (pItemToReplaceComp)
			pItemToReplace = pItemToReplaceComp.GetOwner();
		
		if (pItemToReplace)
		{
			BaseInventoryStorageComponent itemToReplaceStorage = BaseInventoryStorageComponent.Cast(pItemToReplace.FindComponent(BaseInventoryStorageComponent));
		
			if (itemToReplaceStorage && m_InventoryManager.CanInsertItemInStorage(pItem, itemToReplaceStorage, -1))
			{
				m_InventoryManager.TryInsertItemInStorage(pItem, itemToReplaceStorage, -1, m_pCallBack);
				return;
			}
		}
		
		InventoryStorageSlot itemParentSlot = pComp.GetParentSlot();
		
		if (pItemToReplace && pItem)
		{
			if (itemParentSlot && IsStorageArsenal(itemParentSlot.GetStorage()))
			{
				m_InventoryManager.TryReplaceAndDropItemAtSlot(m_pWeaponStorageComp, pItem, weaponSlot.GetWeaponSlotIndex(), m_pCallBack, true, true);
				return;
			}
			else if (pItemToReplace != pItem)
			{
				m_InventoryManager.TrySwapItemStorages(pItem, pItemToReplace, m_pCallBack);
				m_InventoryManager.PlayItemSound(pItem, SCR_SoundEvent.SOUND_EQUIP);
				return;
			}
		}
		
		BaseInventoryStorageComponent pStorageToComponent = m_pWeaponStorageComp;
		
		if (!m_InventoryManager.TryMoveItemToStorage( pItem, m_pWeaponStorageComp, weaponSlot.GetWeaponSlotIndex(), m_pCallBack ))
			m_InventoryManager.TryInsertItemInStorage(pItem, m_pWeaponStorageComp, weaponSlot.GetWeaponSlotIndex(), m_pCallBack);
		
		m_InventoryManager.PlayItemSound(pItem, SCR_SoundEvent.SOUND_EQUIP);
	}

	//------------------------------------------------------------------------------------------------
	//!
	void MoveItem( SCR_InventoryStorageBaseUI pStorageBaseUI = null )
	{
		if ( !m_pSelectedSlotUI )
			return;
		InventoryItemComponent pComp = m_pSelectedSlotUI.GetInventoryItemComponent();
		if ( !pComp )
			return;
		IEntity pItem = pComp.GetOwner();
		if (!m_InventoryManager.CanMoveItem(pItem))
			return;
		
		m_pCallBack.m_pItem = pItem;
		m_pCallBack.m_pMenu = this;
		m_pCallBack.m_pStorageToFocus = m_pSelectedSlotUI.GetStorageUI();
		m_pCallBack.m_pStorageFrom = m_pSelectedSlotUI.GetStorageUI();

		if ( pStorageBaseUI )
			m_pCallBack.m_pStorageTo = pStorageBaseUI;
		else
			m_pCallBack.m_pStorageTo = m_pActiveHoveredStorageUI;

		BaseInventoryStorageComponent pStorageTo = m_pActiveHoveredStorageUI.GetCurrentNavigationStorage();
		if (IsStorageArsenal(pStorageTo))
		{
			SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_DROP_ERROR);
			return;
		}
		
		if ( pStorageTo && EquipedWeaponStorageComponent.Cast( pStorageTo ) )
		{
			m_InventoryManager.EquipWeapon( pItem, m_pCallBack, m_pCallBack.m_pStorageFrom == m_pStorageLootUI );
			return;
		}
		else if ( pStorageTo && CharacterInventoryStorageComponent.Cast( pStorageTo ) )
		{
			m_InventoryManager.EquipAny( pStorageTo, pItem, 0, m_pCallBack );
		}
		else if ( m_pCallBack.m_pStorageTo == m_pStorageLootUI )
		{
			MoveToVicinity( pItem );
			SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_VINICITY_DROP_CLICK);
		}
		else if ( m_pCallBack.m_pStorageFrom == m_pCallBack.m_pStorageTo)
		{
			SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_CONTAINER_SAME_DROP);
		}
		else
		{
			if ( m_pCallBack.m_pStorageFrom == m_pStorageLootUI )
				MoveFromVicinity();
			else
				m_InventoryManager.InsertItem( pItem, m_pActiveHoveredStorageUI.GetCurrentNavigationStorage(), m_pCallBack.m_pStorageFrom.GetStorage(), m_pCallBack );
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//!
	protected void EquipWeapon()
	{
		if ( !m_pSelectedSlotUI )
			return;
		IEntity pItem = m_pSelectedSlotUI.GetInventoryItemComponent().GetOwner();
		if ( !pItem )
			return;
		
		if ( m_InventoryManager )
			m_InventoryManager.EquipWeapon( pItem, m_pCallBack );
	}
	
	//------------------------------------------------------------------------------------------------
	//!
	protected void MoveToVicinity( IEntity pItem )
	{
		BaseInventoryStorageComponent pStorageFrom = m_pSelectedSlotUI.GetStorageUI().GetStorage();
			
		if (!m_InventoryManager.CanMoveItem(pItem))
			return;
		
		m_pCallBack.m_pStorageFrom = GetStorageUIByBaseStorageComponent(pStorageFrom);
		if (!m_pCallBack.m_pStorageFrom)
			m_pCallBack.m_pStorageFrom = m_pSelectedSlotUI.GetStorageUI();

		m_pCallBack.m_pStorageTo = m_pStorageLootUI;
		auto storage = m_pStorageLootUI.GetCurrentNavigationStorage();

		if ( storage && !IsStorageArsenal(storage) )
			m_InventoryManager.InsertItem( pItem, m_pStorageLootUI.GetCurrentNavigationStorage(), pStorageFrom, m_pCallBack );	// moving into the opened storage in the  vicinity
		else
		{
			//droping it on the ground
			auto pSlot = m_pSelectedSlotUI.GetInventoryItemComponent().GetParentSlot();
			if ( pSlot )
			{
				m_InventoryManager.TryRemoveItemFromInventory(pItem, pSlot.GetStorage(), m_pCallBack);
				m_InventoryManager.PlayItemSound(pItem, SCR_SoundEvent.SOUND_DROP);
			}
			else
			{
				SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_DROP_ERROR);
			}
		}	
	}
		
	//------------------------------------------------------------------------------------------------
	//!
	protected void MoveFromVicinity()
	{
		IEntity pItem = m_pSelectedSlotUI.GetInventoryItemComponent().GetOwner();
		if (!m_InventoryManager.CanMoveItem(pItem))
			return;

		m_pCallBack.m_pItem = pItem;
		m_pCallBack.m_pMenu = this;

		m_pCallBack.m_pStorageFrom = m_pStorageLootUI;
		m_pCallBack.m_pStorageTo = m_pActiveHoveredStorageUI;

		if (ShouldSpawnItem())
		{
			m_pCallBack.m_bUpdateSlotOnly = true;
			BaseInventoryStorageComponent storageTo = m_InventoryManager.FindStorageForInsert(pItem, m_pActiveHoveredStorageUI.GetCurrentNavigationStorage(), EStoragePurpose.PURPOSE_ANY);
			m_InventoryManager.TrySpawnPrefabToStorage(pItem.GetPrefabData().GetPrefabName(), storageTo, -1, EStoragePurpose.PURPOSE_ANY, m_pCallBack);
			m_InventoryManager.PlayItemSound(pItem, SCR_SoundEvent.SOUND_PICK_UP);
			return;
		}

		BaseInventoryStorageComponent storagefrom = m_pStorageLootUI.GetCurrentNavigationStorage();
		if (!storagefrom)
		{
			IEntity ent = m_pVicinity.GetItemOfInterest();
			if (ent)
				storagefrom = BaseInventoryStorageComponent.Cast(ent.FindComponent(BaseInventoryStorageComponent));
		}
		
		m_InventoryManager.InsertItem(pItem, m_pActiveHoveredStorageUI.GetCurrentNavigationStorage(), storagefrom, m_pCallBack);	//a storage is already opened, try to move it there
		m_InventoryManager.PlayItemSound(pItem, SCR_SoundEvent.SOUND_PICK_UP);
	}

	//------------------------------------------------------------------------------------------------
	//!
	protected void MoveBetweenFromVicinity()
	{
		IEntity pItem = m_pSelectedSlotUI.GetInventoryItemComponent().GetOwner();
		if (!m_InventoryManager.CanMoveItem(pItem))
			return;

		m_pCallBack.m_pItem = pItem;
		m_pCallBack.m_pMenu = this;

		m_pCallBack.m_pStorageFrom = GetStorageUIFromVicinity(m_pStorageLootUI.GetCurrentNavigationStorage());
		m_pCallBack.m_pStorageTo = m_pStorageListUI;
		m_pCallBack.m_pStorageToFocus = m_pStorageLootUI;
		m_pCallBack.m_sItemToFocus = m_pSelectedSlotUI.GetItemResource();
		m_pCallBack.m_bUpdateSlotOnly = m_pSelectedSlotUI.IsStacked();

		BaseInventoryStorageComponent pStorageTo = m_StorageManager;
		if ( pItem.FindComponent( SCR_GadgetComponent ) )
		{
			BaseInventoryStorageComponent pStorageComp = m_InventoryManager.FindStorageForItem( pItem, EStoragePurpose.PURPOSE_EQUIPMENT_ATTACHMENT );
			if ( pStorageComp )
				pStorageTo = pStorageComp;
			else
			{
				pStorageComp = m_InventoryManager.FindStorageForItem( pItem, EStoragePurpose.PURPOSE_DEPOSIT );
				if ( pStorageComp )
					pStorageTo = pStorageComp;
			}
		}

		if (ShouldSpawnItem())
		{
			m_pCallBack.m_bUpdateSlotOnly = true;
			m_InventoryManager.TrySpawnPrefabToStorage(pItem.GetPrefabData().GetPrefabName(), null, -1, EStoragePurpose.PURPOSE_ANY, m_pCallBack);
			return;
		}

		m_InventoryManager.InsertItem( pItem, null, m_pStorageLootUI.GetCurrentNavigationStorage(), m_pCallBack );
	}

	//------------------------------------------------------------------------------------------------
	protected bool ShouldSpawnItem()
	{
		if (!m_pSelectedSlotUI)
			return false;
		if (!m_pSelectedSlotUI.GetStorageUI())
			return false;

		BaseInventoryStorageComponent currStorage = m_pSelectedSlotUI.GetStorageUI().GetCurrentNavigationStorage();
		return IsStorageArsenal(currStorage);
	}

	//------------------------------------------------------------------------------------------------
	protected bool IsStorageArsenal(BaseInventoryStorageComponent storage)
	{
		if (!storage)
			return false;
		return (storage.GetOwner().FindComponent(SCR_ArsenalComponent) != null);
	}

	//------------------------------------------------------------------------------------------------
	protected void EquipDraggedItem(bool forceEquip = false)
	{
		if (!m_pSelectedSlotUI || !m_pSelectedSlotUI.GetInventoryItemComponent())
			return;

		IEntity pItem = m_pSelectedSlotUI.GetInventoryItemComponent().GetOwner();
		if (!pItem)
			return;

		m_pCallBack.m_pItem = pItem;
		m_pCallBack.m_pMenu = this;
		m_pCallBack.m_pStorageFrom = m_pSelectedSlotUI.GetStorageUI();

		m_pCallBack.m_pStorageToFocus = m_pSelectedSlotUI.GetStorageUI();
		m_pCallBack.m_sItemToFocus = m_pSelectedSlotUI.GetItemResource();
		m_pCallBack.m_bShouldEquip = forceEquip;

		BaseInventoryStorageComponent pStorageTo = m_StorageManager;
		if (m_pSelectedSlotUI.GetStorageUI().Type() == SCR_InventoryStorageWeaponsUI)
		{
			m_pCallBack.m_pStorageTo = m_pWeaponStorage;
		}
		else
		{
			m_pCallBack.m_pStorageTo = m_pStorageListUI;
		}

		if (pItem.FindComponent(SCR_GadgetComponent))
		{
			BaseInventoryStorageComponent pStorageComp = m_InventoryManager.FindStorageForItem( pItem, EStoragePurpose.PURPOSE_EQUIPMENT_ATTACHMENT );
			if (pStorageComp)
				pStorageTo = pStorageComp;
			else
			{
				pStorageComp = m_InventoryManager.FindStorageForItem(pItem, EStoragePurpose.PURPOSE_DEPOSIT);
				if (pStorageComp)
					pStorageTo = pStorageComp;
			}
		}

		if (pItem.FindComponent(WeaponComponent))
		{
			BaseInventoryStorageComponent pStorageComp = m_InventoryManager.FindStorageForItem(pItem, EStoragePurpose.PURPOSE_WEAPON_PROXY);
			if (pStorageComp)
				pStorageTo = pStorageComp;
			else
			{
				pStorageComp = m_InventoryManager.FindStorageForItem(pItem, EStoragePurpose.PURPOSE_DEPOSIT);;
				if (pStorageComp)
					pStorageTo = pStorageComp;
			}
		}

		bool equip = m_InventoryManager.EquipAny(pStorageTo , pItem, 0, m_pCallBack);
		if (!equip && m_pSelectedSlotUI)
		{
			m_InventoryManager.InsertItem(pItem, null, m_pSelectedSlotUI.GetStorageUI().GetCurrentNavigationStorage(), m_pCallBack);
		}
		else
		{
			m_InventoryManager.PlayItemSound(pItem, SCR_SoundEvent.SOUND_EQUIP);
		}
	}

	//------------------------------------------------------------------------------------------------
	//!
	protected void MoveBetweenToVicinity()
	{
		InventoryItemComponent pInvComponent = m_pFocusedSlotUI.GetInventoryItemComponent();
		if(!pInvComponent)
			return;
		
		IEntity pItem = pInvComponent.GetOwner();
		if (!m_InventoryManager.CanMoveItem(pItem))
			return;

		m_pCallBack.m_pItem = pItem;
		m_pCallBack.m_pMenu = this;

		m_pCallBack.m_pStorageToFocus = GetStorageUIByBaseStorageComponent(m_pSelectedSlotUI.GetStorageUI().GetCurrentNavigationStorage());
		m_pCallBack.m_sItemToFocus = m_pSelectedSlotUI.GetItemResource();
		m_pCallBack.m_bUpdateSlotOnly = m_pSelectedSlotUI.IsStacked();

		MoveToVicinity( pItem );
	}

	//------------------------------------------------------------------------------------------------
	void DeselectSlot()
	{
		if (m_pSelectedSlotUI)
		{
			m_pSelectedSlotUI.SetSelected(false);
			m_pSelectedSlotUI = null;
		}
	}

	//------------------------------------------------------------------------------------------------
	//!
	protected void HandleItemSelection()
	{
		if ( m_pFocusedSlotUI && m_pSelectedSlotUI && m_pFocusedSlotUI != m_pSelectedSlotUI )
		{
			if ( m_bDraggingEnabled )
			{
				m_pFocusedSlotUI.SetSelected( true );			//select new one
				m_pSelectedSlotUI.SetSelected( false );		//deselect the previous one
				m_pSelectedSlotUI = m_pFocusedSlotUI;			//and register the newly selected
				FilterOutStorages( true );
				m_EStateMenuItem = EStateMenuItem.STATE_MOVING_ITEM_STARTED;
			}
		}
		else
		{
			if ( m_bDraggingEnabled )
			{
				if ( m_pSelectedSlotUI )
					m_pSelectedSlotUI.SetSelected( false );		//deselect it
				m_pSelectedSlotUI = null;						//and deregister it
				FilterOutStorages( false );
				m_EStateMenuItem = EStateMenuItem.STATE_IDLE;
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void SetOpenStorage()
	{
		IEntity m_pStorageToOpen = GetInventoryStorageManager().GetStorageToOpen();
		if (!m_pStorageToOpen)
		{
			SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_OPEN);
			return;
		}

		BaseInventoryStorageComponent comp = BaseInventoryStorageComponent.Cast(m_pStorageToOpen.FindComponent(BaseInventoryStorageComponent));
		if (!comp)
			return;

		SCR_InventoryStorageBaseUI storageUI = GetStorageUIFromVicinity(comp);
		if (!storageUI)
			return;
		if (storageUI.IsTraversalAllowed())
		{
			storageUI.Traverse(comp);
		}

		SetStorageSwitchMode(false);
		ButtonWidget lastCloseTraverseButton = storageUI.GetLastCloseTraverseButton();

		if (m_bIsUsingGamepad && lastCloseTraverseButton && !FocusOnSlotInStorage(storageUI, 0, false))
			GetGame().GetWorkspace().SetFocusedWidget(lastCloseTraverseButton);
		else
			FocusOnSlotInStorage(storageUI);
	}

	//------------------------------------------------------------------------------------------------
	//!
	protected void TraverseActualSlot()
	{
		m_pSelectedSlotUI = m_pFocusedSlotUI;
		auto storage = m_pFocusedSlotUI.GetAsStorage();
		if (storage)
		{
			auto parentUIContainer = m_pFocusedSlotUI.GetStorageUI();
			if (parentUIContainer.IsTraversalAllowed())
				parentUIContainer.Traverse(storage);
			
			ButtonWidget lastCloseTraverseButton = parentUIContainer.GetLastCloseTraverseButton();
			
			if (m_bIsUsingGamepad && lastCloseTraverseButton && !FocusOnSlotInStorage(parentUIContainer, 0, false))
				GetGame().GetWorkspace().SetFocusedWidget(lastCloseTraverseButton);
			else
				FocusOnSlotInStorage(parentUIContainer);
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void OpenAsNewContainer()
	{
		m_pSelectedSlotUI = m_pFocusedSlotUI;
		auto storage = m_pFocusedSlotUI.GetAsStorage();
		if (storage)
			OpenStorageAsContainer(storage);
	}

	//------------------------------------------------------------------------------------------------
	//! when clicked on item / storage slot using controller or mouse
	protected void Action_SelectItem()
	{
		// Ignore if we cannot interact
		if (!GetCanInteract())
			return;
		SimpleFSM( EMenuAction.ACTION_SELECT );
	}

	//------------------------------------------------------------------------------------------------
	protected void Action_DeselectItem()
	{
		SimpleFSM(EMenuAction.ACTION_DESELECT);
		ResetHighlightsOnAvailableStorages();
	}

	//------------------------------------------------------------------------------------------------
	protected void Action_MoveInsideStorage()
	{
		SimpleFSM(EMenuAction.ACTION_MOVEINSIDE);
		ResetHighlightsOnAvailableStorages();
	}

	//------------------------------------------------------------------------------------------------
	protected void Action_EquipItem()
	{
		// Ignore if we cannot interact
		if (!GetCanInteract())
			return;
		if( !m_pFocusedSlotUI )
			return;

		// TODO: rather check the m_eSlotFunction. The functionality of the item is recognized already in the constructor of the SlotUI
		InventoryItemComponent pInvComponent = m_pFocusedSlotUI.GetInventoryItemComponent();
		if( !pInvComponent )
			return;
		m_pItem = pInvComponent.GetOwner();
		if(!m_InventoryManager.CanMoveItem(m_pItem))
			return;

		SCR_GadgetComponent gadgetComp =  SCR_GadgetComponent.Cast( m_pItem.FindComponent( SCR_GadgetComponent ));
		if( gadgetComp )
		{
			//m_InventoryManager.EquipGadget( m_pItem );
		}
		else
		{
			auto weaponComp =  WeaponComponent.Cast(m_pItem.FindComponent( WeaponComponent ));
			if ( weaponComp )
				MoveWeapon();
		}

		FilterOutStorages( false );
		ResetHighlightsOnAvailableStorages();
		SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_VINICITY_EQUIP_CLICK);
	}

	//------------------------------------------------------------------------------------------------
	protected void Action_MoveBetween()
	{
		// Ignore if we cannot interact
		if (!GetCanInteract())
			return;
		SimpleFSM( EMenuAction.ACTION_MOVEBETWEEN );
		ResetHighlightsOnAvailableStorages();
	}

	//------------------------------------------------------------------------------------------------
	protected void Action_Inspect()
	{	
		if (m_pFocusedSlotUI == m_pInspectedSlot)
			InspectItem(null);
		else
			InspectItem(m_pFocusedSlotUI);
	}

	//------------------------------------------------------------------------------------------------
	//!
	protected void Action_UseItem()
	{
		if (m_pFocusedSlotUI)
		{
			SCR_InventoryHitZonePointContainerUI hzContainer = SCR_InventoryHitZonePointContainerUI.Cast(m_AttachmentSpinBox.GetCurrentItemData());
			if (hzContainer)
			{
				hzContainer.GetStorage().OnDrop(m_pFocusedSlotUI);
			}

			SCR_InventorySlotGearInspectionUI point = SCR_InventorySlotGearInspectionUI.Cast(m_AttachmentSpinBox.GetCurrentItemData());
			if (point)
			{
				m_pGearInspectionPointUI.SetSlotFocused(point);
				point.OnDrop(m_pFocusedSlotUI);
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	//!
	protected void Action_UnfoldItem()
	{
 		SimpleFSM( EMenuAction.ACTION_UNFOLD );
	}

	//------------------------------------------------------------------------------------------------
	void Action_StepBack(SCR_InventoryStorageBaseUI pParentStorage = null, int traverseStorageIndex = -1)
	{
		//TODO: m_pActiveStorageUI must be set according the active SCR_InventoryStorageBaseUI
		if (pParentStorage && pParentStorage.IsTraversalAllowed())
		{
			pParentStorage.Back(traverseStorageIndex);
			FilterOutStorages(false);
			m_EStateMenuItem = EStateMenuItem.STATE_IDLE;
			SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_CONTAINER_CLOSE);
			FocusOnSlotInStorage(pParentStorage);
			m_bWasJustTraversing = true;
		}
		ResetHighlightsOnAvailableStorages();
	}

	//------------------------------------------------------------------------------------------------
	void Action_DragDown()
	{
		if ( m_bDraggingEnabled )
			return;
		if ( !m_pFocusedSlotUI )
			return;
		if (!m_pFocusedSlotUI.IsDraggable())
			return;

		if ( m_pFocusedSlotUI && WidgetManager.GetWidgetUnderCursor() != m_pFocusedSlotUI.GetButtonWidget() )
			return;

		if 	(m_pFocusedSlotUI.Type() != SCR_InventorySlotUI &&
			m_pFocusedSlotUI.Type() != SCR_InventorySlotLBSUI &&
			m_pFocusedSlotUI.Type() != SCR_InventorySlotStorageUI &&
			m_pFocusedSlotUI.Type() != SCR_InventorySlotStorageEmbeddedUI &&
			m_pFocusedSlotUI.Type() != SCR_InventoryStorageWeaponsUI &&
			m_pFocusedSlotUI.Type() != SCR_InventorySlotWeaponUI &&
			m_pFocusedSlotUI.Type() != SCR_InventorySlotQuickSlotUI &&
			m_pFocusedSlotUI.Type() != SCR_InventorySlotWeaponSlotsUI &&
			m_pFocusedSlotUI.Type() != SCR_InventorySlotGearInspectionUI)
			return;

		m_bDraggingEnabled = false;
		WidgetManager.GetMousePos( m_iMouseX, m_iMouseY );
		GetGame().GetInputManager().AddActionListener( "Inventory_Drag", EActionTrigger.PRESSED, Action_OnDrag );
	}

	//------------------------------------------------------------------------------------------------
	void Action_OnDrag()
	{
		int iMouseX, iMouseY;
		WidgetManager.GetMousePos( iMouseX, iMouseY );
		if ( !m_bDraggingEnabled )
		{
			int dX = Math.AbsInt( iMouseX - m_iMouseX );
			int dY = Math.AbsInt( iMouseY - m_iMouseY );
			if ( ( dX < DRAG_THRESHOLD ) && ( dY < DRAG_THRESHOLD ) )
				return;
			if ( !m_pFocusedSlotUI )
				return;
			if (!m_pFocusedSlotUI.IsDraggable())
				return;
			InventoryItemComponent focusedItemComp = m_pFocusedSlotUI.GetInventoryItemComponent();
			if (!focusedItemComp)
				return;
			
			InventoryStorageSlot slot = focusedItemComp.GetParentSlot();
			if (slot)
			{
				IEntity owner = slot.GetOwner();
				while (owner)
				{
					if (GetGame().GetPlayerController().GetControlledEntity() == owner)
					{
						SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_CONTAINER_DRAG);
						break;
					}
					
					owner = owner.GetParent();
					if (!owner)
						SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_VINICITY_DRAG);
				}
			}
			else
			{
				SCR_UISoundEntity.SoundEvent(SCR_SoundEvent.SOUND_INV_VINICITY_DRAG);
			}
			
			m_bDraggingEnabled = true;
			SimpleFSM( EMenuAction.ACTION_DRAGGED );
			m_wDragDropContainer.SetVisible( true );

			ItemPreviewManagerEntity manager = GetGame().GetItemPreviewManager();
			if ( manager && m_pSelectedSlotUI )
			{
				HighlightAvailableStorages(m_pSelectedSlotUI);
				ItemPreviewWidget renderPreview = ItemPreviewWidget.Cast( m_pDragDropPreviewImage );
				IEntity previewEntity = null;
				InventoryItemComponent pComp = m_pSelectedSlotUI.GetInventoryItemComponent();
				if ( pComp )
				{
					previewEntity = pComp.GetOwner();
					if (renderPreview)
						manager.SetPreviewItem(renderPreview, previewEntity);
				}
			}
			ContainerSetPos( iMouseX, iMouseY );
			GetGame().GetInputManager().AddActionListener( "Inventory_Drag", EActionTrigger.UP, Action_DragUp );
		}
		else
		{
			ContainerSetPos( iMouseX, iMouseY );
		}
	}

	//------------------------------------------------------------------------------------------------
	void Action_DragUp()
	{
		GetGame().GetInputManager().RemoveActionListener( "Inventory_Drag", EActionTrigger.PRESSED, Action_OnDrag );
		GetGame().GetInputManager().RemoveActionListener( "Inventory_Drag", EActionTrigger.UP, Action_DragUp );
		m_wDragDropContainer.SetVisible( false );
		SimpleFSM( EMenuAction.ACTION_DROPPED );
		m_bDraggingEnabled = false;
		ResetHighlightsOnAvailableStorages();
	}

	//------------------------------------------------------------------------------------------------
	void ContainerSetPos( int iMouseX, int iMouseY )
	{
		float fWidth, fHeight;
		WorkspaceWidget wWorkSpace = GetGame().GetWorkspace();
		m_wDragDropContainer.GetScreenSize( fWidth, fHeight );
		fWidth = wWorkSpace.DPIUnscale( fWidth ) / 2;
		fHeight = wWorkSpace.DPIUnscale( fHeight ) / 2;

		m_pDragDropFrameSlotUI.SetPosX( wWorkSpace.DPIUnscale( iMouseX ) - fWidth );
		m_pDragDropFrameSlotUI.SetPosY( wWorkSpace.DPIUnscale( iMouseY ) - fHeight );
	}

	//------------------------------------------------------------------------------------------------
	// !
	protected void Action_Move( SCR_InventoryStorageBaseUI pStorageBase )
	{
		IEntity pItem = m_pSelectedSlotUI.GetInventoryItemComponent().GetOwner();
		m_InventoryManager.InsertItem( pItem, pStorageBase.GetCurrentNavigationStorage(), m_pFocusedSlotUI.GetStorageUI().GetCurrentNavigationStorage() );
		ResetHighlightsOnAvailableStorages();
	}

	//------------------------------------------------------------------------------------------------
	SCR_InventoryOpenedStorageUI GetOpenedStorage(BaseInventoryStorageComponent storage)
	{
		if (!storage)
			return null;

		foreach (SCR_InventoryOpenedStorageUI openedStorage : m_aOpenedStoragesUI)
		{
			if (openedStorage.GetStorage() == storage)
				return openedStorage;
		}
		return null;
	}

	//------------------------------------------------------------------------------------------------
	// !
	protected void Action_LootMove()
	{
		if ( !m_pFocusedSlotUI )
			return;
		IEntity pItem = m_pFocusedSlotUI.GetInventoryItemComponent().GetOwner();
		if (m_InventoryManager.CanMoveItem(pItem))
			return;

		m_pCallBack.m_pItem = pItem;
		m_pCallBack.m_pMenu = this;

		// Loot ===>>> Character
		if ( m_pFocusedSlotUI.GetStorageUI() == m_pStorageLootUI )
		{
			m_pCallBack.m_pStorageFrom = m_pStorageLootUI;
			m_pCallBack.m_pStorageTo = m_pFocusedSlotUI.GetStorageUI();

			if ( m_pDisplayedStorage.Type() == EquipedWeaponStorageComponent )			//default view opened, put the item into the first suitable storage
			{
				m_InventoryManager.InsertItem( pItem, null, null, m_pCallBack );
			}
			else
			{
				m_InventoryManager.InsertItem( pItem, m_pStorageBaseUI.GetCurrentNavigationStorage(), m_pStorageLootUI.GetCurrentNavigationStorage(), m_pCallBack ); 			//a storage is already opened, try to move it there
			}
		}
		else
		// Character ===>>> Loot
		{
			m_pCallBack.m_pStorageFrom = m_pFocusedSlotUI.GetStorageUI();
			m_pCallBack.m_pStorageTo = m_pStorageLootUI;
			auto storage = m_pStorageLootUI.GetCurrentNavigationStorage();
			if ( storage )
				m_InventoryManager.InsertItem( pItem, m_pStorageLootUI.GetCurrentNavigationStorage(), m_pDisplayedStorage, m_pCallBack );	// moving into the opened storage in the  vicinity
			else
			{
				//droping it on the ground
				auto pSlot = m_pFocusedSlotUI.GetInventoryItemComponent().GetParentSlot();
				if ( pSlot )
					m_InventoryManager.TryRemoveItemFromInventory(pItem, pSlot.GetStorage(), m_pCallBack);
			}	
		}
		ResetHighlightsOnAvailableStorages();
	}

	//------------------------------------------------------------------------------------------------
	// !
	protected bool IsStorageInsideLBS( BaseInventoryStorageComponent pStorage, ClothNodeStorageComponent pLBSStorage )
	{
		if ( !pLBSStorage )
			return false;
		array<BaseInventoryStorageComponent> aOutStorages = new array<BaseInventoryStorageComponent>();
		pLBSStorage.GetOwnedStorages( aOutStorages, 1, false );
		return aOutStorages.Find(pStorage) != -1;
	}

	//------------------------------------------------------------------------------------------------
	protected void OnItemAddedListener( IEntity item, notnull BaseInventoryStorageComponent storage )
	{
		if ( storage == m_StorageManager.GetLootStorage() )
			return;

		if ( storage.Type() == EquipedWeaponStorageComponent )	//we are updating the weapon storage on character
		{			
			if ( m_pWeaponStorage )
				m_pWeaponStorage.Refresh();
		}
		else if ( storage.Type() == WeaponAttachmentsStorageComponent )	// we are updating a weapon ( attachement / magazin inserted )
		{
			//if ( m_pWeaponStorage && m_pWeaponStorage.GetCurrentNavigationStorage() == storage )
			if ( m_pWeaponStorage )
				m_pWeaponStorage.Refresh();
		}

		if (m_StorageManager.IsItemAlreadyInQuickSlot(item))
			ShowQuickSlotStorage();

		UpdateTotalWeightText();
	}

	//------------------------------------------------------------------------------------------------
	protected void OnItemRemovedListener( IEntity item, notnull BaseInventoryStorageComponent storage )
	{
		if ( !storage )
			return;

		SCR_ConsumableItemComponent consumable = SCR_ConsumableItemComponent.Cast(item.FindComponent(SCR_ConsumableItemComponent));
		if (consumable && consumable.GetConsumableEffect().GetDeleteOnUse()) // consumables require different handling
		{
			if (m_wAttachmentStorage && m_pAttachmentStorageUI)
				m_pAttachmentStorageUI.Refresh();
			SCR_InventoryStorageBaseUI storageUI = GetStorageUIByBaseStorageComponent(storage);
			if (storageUI)
				storageUI.Refresh();
			ShowQuickSlotStorage();
		}
		
		if ( storage == m_StorageManager.GetLootStorage() )
			return;

		if (m_StorageManager.GetLastQuickSlotId(item) > -1)
			ShowQuickSlotStorage();

		UpdateTotalWeightText();
	}

	//------------------------------------------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool OnFocus(Widget w, int x, int y)
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	void OnContainerHovered( SCR_InventoryStorageBaseUI pContainer )
	{
		if ( m_bDraggingEnabled )
			pContainer.ShowContainerBorder( true );
		m_pActiveHoveredStorageUI = pContainer;
		pContainer.SetPagingActive(true);
	}

	//------------------------------------------------------------------------------------------------
	void HighlightAvailableStorages(SCR_InventorySlotUI itemSlot)
	{
		if (!itemSlot)
			return;
		
		InventoryItemComponent itemComp = itemSlot.GetInventoryItemComponent();
		if (!itemComp)
			return;
		
		IEntity itemEntity = itemComp.GetOwner();
		if (!itemEntity)
			return;
		
		InventoryStorageSlot itemParentSlot = itemComp.GetParentSlot();
		BaseInventoryStorageComponent originStorage;
		if (itemParentSlot)
			originStorage = itemParentSlot.GetStorage();
		
		BaseInventoryStorageComponent contStorage;
		float totalWeightWithInsertedItem;
		float totalOccupiedVolumeWithInsertedItem;			
		float itemWeight = itemComp.GetTotalWeight();
		
		foreach (SCR_InventoryStorageBaseUI storageBaseUI: m_aStorages)
		{	
			if (!storageBaseUI)
				continue;
			if (storageBaseUI.Type() == SCR_InventoryStorageLootUI)
				continue;
			contStorage = storageBaseUI.GetStorage();	
			if (!contStorage)
				continue;
			if (originStorage && contStorage == originStorage)
				continue;		
			if (IsStorageInsideLBS(originStorage, ClothNodeStorageComponent.Cast(contStorage)))	
				continue;
			
			totalWeightWithInsertedItem = storageBaseUI.GetTotalRoundedUpWeight(contStorage);
			totalWeightWithInsertedItem += Math.Round(itemWeight * 100) / 100;
			
			totalOccupiedVolumeWithInsertedItem = storageBaseUI.GetOccupiedVolume(contStorage);
			totalOccupiedVolumeWithInsertedItem += itemComp.GetTotalVolume();
			
			storageBaseUI.UpdateTotalWeight(totalWeightWithInsertedItem);
			storageBaseUI.UpdateVolumePercentage(storageBaseUI.GetOccupiedVolumePercentage(contStorage, totalOccupiedVolumeWithInsertedItem));
			
			if (m_InventoryManager.CanInsertItemInActualStorage(itemEntity, contStorage))
				storageBaseUI.SetStorageAsHighlighted(true);
			else	
				storageBaseUI.SetStorageAsHighlighted(false);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void ResetHighlightsOnAvailableStorages()
	{
		foreach (SCR_InventoryStorageBaseUI storageBaseUI: m_aStorages)
		{
			if (!storageBaseUI)
				continue;
			
			storageBaseUI.UpdateVolumePercentage(storageBaseUI.GetOccupiedVolumePercentage(storageBaseUI.GetStorage()));
			storageBaseUI.UpdateTotalWeight(storageBaseUI.GetTotalRoundedUpWeight(storageBaseUI.GetStorage()));
			storageBaseUI.SetStorageAsHighlighted(true);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void OnContainerLeft( SCR_InventoryStorageBaseUI pContainer )
	{		
		pContainer.ShowContainerBorder( false );
		m_pActiveHoveredStorageUI = null;
		pContainer.SetPagingActive(false);
	}
	
	//------------------------------------------------------------------------------------------------
	void OnContainerFocused(SCR_InventoryStorageBaseUI pContainer)
	{
		pContainer.ShowContainerBorder(true);
		SetActiveStorage(pContainer);
		m_pActiveStorageUI = pContainer;
		NavigationBarUpdate();
	}

	//------------------------------------------------------------------------------------------------
	void OnContainerFocusLeft(SCR_InventoryStorageBaseUI pContainer)
	{
		pContainer.ShowContainerBorder(false);
		SetActiveStorage(pContainer);
		m_pActiveStorageUI = null;
		NavigationBarUpdate();
	}

	//------------------------------------------------------------------------------------------------
	SCR_CharacterControllerComponent GetCharacterController()
	{
		return m_CharController;
	}

	//------------------------------------------------------------------------------------------------
	//! TODO: remove in future
	protected bool MoveOperation( InventoryItemComponent pItemComponent, BaseInventoryStorageComponent pStorageTo )
	{
		if ( !pItemComponent || !pStorageTo )
		{
			Print( "INV: Cannot perform move operation. Either item or storage doesn't exist", LogLevel.DEBUG );
			return false;
		}

			m_InventoryManager.EquipAny( pStorageTo, pItemComponent.GetOwner() );
		return true;
	}

	//------------------------------------------------------------------------------------------------
	//! TODO: remove in future
	void MoveWeapon()
	{
		if (!m_pFocusedSlotUI)
			return;
		
		m_pCallBack.m_pStorageFrom = m_pFocusedSlotUI.GetStorageUI();
		
		m_InventoryManager.EquipWeapon( m_pFocusedSlotUI.GetInventoryItemComponent().GetOwner(), m_pCallBack );
	}
	
	//------------------------------------------------------------------------------------------------
	//! 
	protected void RefreshUISlotStorages()
	{
		foreach ( SCR_InventorySlotUI pStorage : m_aShownStorages )
		{
			if ( pStorage )
				pStorage.Refresh();
		}
	}

	//------------------------------------------------------------------------------------------------
	protected void UpdateTotalWeightText()
	{
		if (!m_wTotalWeightText || !m_StorageManager)
			return;
		
		m_wTotalWeightText.SetText(string.Format("%1 / %2 kg", GetTotalWeight(), m_StorageManager.GetMaxLoad()));
	}
	
	//------------------------------------------------------------------------------------------------
	protected void ShowQuickSlotStorage()
	{
		if( m_wQuickSlotStorage )
		{
			m_wQuickSlotStorage.RemoveHandler( m_wQuickSlotStorage.FindHandler( SCR_InventoryStorageQuickSlotsUI ) );	//remove the handler from the widget
			m_wQuickSlotStorage.RemoveFromHierarchy();
		}
		
		Widget parent = m_widget.FindAnyWidget( "QuickSlots" );
		m_wQuickSlotStorage =  GetGame().GetWorkspace().CreateWidgets( "{A1E61EF091EAC47C}UI/layouts/Menus/Inventory/InventoryQuickSlotsGrid.layout", parent );
				
		if( !m_wQuickSlotStorage )
			return;
		
		OverlaySlot.SetVerticalAlign( m_wQuickSlotStorage, LayoutVerticalAlign.Bottom );
		OverlaySlot.SetHorizontalAlign(m_wQuickSlotStorage, LayoutHorizontalAlign.Left);
					
		m_wQuickSlotStorage.AddHandler( new SCR_InventoryStorageQuickSlotsUI( null, null, this ) );
		m_pQuickSlotStorage = SCR_InventoryStorageQuickSlotsUI.Cast( m_wQuickSlotStorage.FindHandler( SCR_InventoryStorageQuickSlotsUI ) );
	}
	
	//------------------------------------------------------------------------------------------------
	// !
	void Action_SelectQuickSlot()
	{
		if ( !GetGame().GetInputManager() )
			return;
		int iSlot = -1;

		int quickSlotCount = m_StorageManager.GetQuickSlotItems().Count();
		for (int id = 1; id <= quickSlotCount; ++id)
		{
			if (GetGame().GetInputManager().GetActionTriggered("InventoryQuickSlot" + id))
				iSlot = id;
		}

		if ( iSlot != -1 )
			SetItemToQuickSlot( iSlot );
	}

	//------------------------------------------------------------------------------------------------
	// !
	protected void RemoveItemFromQuickSlotDrop()
	{
		if ( !m_pSelectedSlotUI )
			return;
		InventoryItemComponent pInventoryComponent = m_pSelectedSlotUI.GetInventoryItemComponent();
		if ( !pInventoryComponent )
			return;
		m_StorageManager.StoreItemToQuickSlot( pInventoryComponent.GetOwner(), m_pSelectedSlotUI.GetSlotIndex() );
		ShowQuickSlotStorage();
	}

	//------------------------------------------------------------------------------------------------
	// !
	protected void SetItemToQuickSlot( int iSlotIndex, SCR_InventorySlotUI slot = null )
	{
		if (!slot)
			slot = m_pFocusedSlotUI;
		if ( !slot )
			return;
		if (iSlotIndex < WEAPON_SLOTS_COUNT)
			return;
		if ( slot.GetStorageUI() && slot.GetStorageUI().Type() == SCR_InventoryStorageLootUI )
			return;	//we don't want to take anything from vicinity to the quickslot
		if ( slot.Type() == SCR_InventorySlotQuickSlotUI )
			return;
		InventoryItemComponent pInventoryComponent = slot.GetInventoryItemComponent();
		if ( !pInventoryComponent )
			return;
		IEntity pItem = pInventoryComponent.GetOwner();
		if (!pItem)
			return;
		if (m_pWeaponStorageComp.Contains(pItem))
			return;
		SCR_ItemAttributeCollection pItemAttributes = SCR_ItemAttributeCollection.Cast( pInventoryComponent.GetAttributes() );
		if ( pItemAttributes && ( pItemAttributes.GetItemSize() != ESlotSize.SLOT_1x1 && pItemAttributes.GetItemSize() != ESlotSize.SLOT_2x1 ) )
			return; //so far only items with one line are supported ( issue on the UI side )

		m_StorageManager.StoreItemToQuickSlot( pItem, --iSlotIndex );
		ShowQuickSlotStorage();
	}

	//------------------------------------------------------------------------------------------------
	// !
	protected void SetItemToQuickSlotDrop()
	{
		if ( !m_pFocusedSlotUI )
			return;
		if ( !m_pSelectedSlotUI )
			return;
		if ( m_pSelectedSlotUI.GetStorageUI() && m_pSelectedSlotUI.GetStorageUI().Type() == SCR_InventoryStorageLootUI )
			return;	//we don't want to take anything from vicinity to the quickslot

		IEntity pOriginalEntity;
		if ( ( SCR_InventorySlotQuickSlotUI.Cast( m_pSelectedSlotUI ) ) && ( SCR_InventorySlotQuickSlotUI.Cast( m_pFocusedSlotUI ) ) ) // swapping
		{
			InventoryItemComponent pComp = m_pFocusedSlotUI.GetInventoryItemComponent();
			if ( pComp )
			{
				pOriginalEntity = pComp.GetOwner();
			}
		}

		InventoryItemComponent pInventoryComponent = m_pSelectedSlotUI.GetInventoryItemComponent();
		if ( !pInventoryComponent )
			return;
		SCR_ItemAttributeCollection pItemAttributes = SCR_ItemAttributeCollection.Cast( pInventoryComponent.GetAttributes() );
		if ( pItemAttributes && ( pItemAttributes.GetItemSize() != ESlotSize.SLOT_1x1 && pItemAttributes.GetItemSize() != ESlotSize.SLOT_2x1 ) )
			return; //so far only items with one line are supported ( issue on the UI side )
		int iSlotIndex = m_pFocusedSlotUI.GetSlotIndex();
		
		if (iSlotIndex < WEAPON_SLOTS_COUNT || m_pWeaponStorageComp.Contains(pInventoryComponent.GetOwner()))
		{
			ShowQuickSlotStorage();
			return;
		}
		
		m_StorageManager.StoreItemToQuickSlot( pInventoryComponent.GetOwner(), iSlotIndex );
		if ( pOriginalEntity )
			m_StorageManager.StoreItemToQuickSlot( pOriginalEntity, m_pSelectedSlotUI.GetSlotIndex() );
		ShowQuickSlotStorage();
	}

	//------------------------------------------------------------------------------------------------
	protected void Action_SwapItems(SCR_InventorySlotUI item1, SCR_InventorySlotUI item2)
	{
		if (!item1 || !item2)
			return;

		IEntity from = item1.GetInventoryItemComponent().GetOwner();
		IEntity to = item2.GetInventoryItemComponent().GetOwner();

		if (!(m_InventoryManager.CanMoveItem(from) && m_InventoryManager.CanMoveItem(to)))
			return;
		
		m_pCallBack.m_pStorageFrom = m_pSelectedSlotUI.GetStorageUI();
		m_pCallBack.m_pStorageTo = m_pFocusedSlotUI.GetStorageUI();
		m_pCallBack.m_pItem = from;
		m_pCallBack.m_pMenu = this;
		m_pCallBack.m_pStorageToFocus = m_pCallBack.m_pStorageFrom;
		
		//if (m_InventoryManager.CanSwapItemStorages(from, to))
		m_InventoryManager.TrySwapItemStorages(from, to, m_pCallBack); 
	}

	//------------------------------------------------------------------------------------------------
	protected void Action_MoveItemToStorage(SCR_InventoryStorageBaseUI toStorage = null)
	{
		if (!toStorage)
			toStorage = m_pFocusedSlotUI.GetStorageUI();

		if (!toStorage || !m_pSelectedSlotUI)
			return;

		MoveItem(toStorage);
		ResetHighlightsOnAvailableStorages();
	}

	//------------------------------------------------------------------------------------------------
	bool IsUsingGamepad()
	{
		return m_bIsUsingGamepad;
	}

	//------------------------------------------------------------------------------------------------
	void OnInputDeviceIsGamepad(bool isGamepad)
	{
		m_bIsUsingGamepad = isGamepad;
		SetStorageSwitchMode(m_bIsUsingGamepad);
		SetAttachmentSpinBoxActive(isGamepad);
		NavigationBarUpdate();
	}

	//------------------------------------------------------------------------------------------------
	void SCR_InventoryMenuUI()
	{
	}
	
	//------------------------------------------------------------------------------------------------
	void ~SCR_InventoryMenuUI()
	{
	}
	
	#else
	protected float GetTotalLoadWeight();
	SCR_InventoryStorageManagerComponent GetInventoryStorageManager();
	bool GetCanInteract();
	override void OnMenuOpen();
	protected void Init();
	void ShowStorage();
	void ShowStorage( BaseInventoryStorageComponent storage );
	protected void ShowStoragesList();
	//protected void ShowGadgetStorage();
	protected void ShowEquipedWeaponStorage();
	void ShowItemInfo( string sName = "", string sDescr = "", float sWeight = 0.0 );
	void HideItemInfo();
	void SetSlotFocused( SCR_InventorySlotUI pFocusedSlot );
	SCR_InventoryInspectionUI GetInspectionScreen();
	void InspectItem(SCR_InventorySlotUI itemSlot);
	void FilterOutStorages();
	protected void Action_CloseInventory();
	protected void Action_SelectItem();
	protected void Action_EquipItem();
	protected void Action_Drop();
	protected void Action_Inspect();
	protected void OnItemAddedListener( IEntity item, BaseInventoryStorageComponent storage );
	protected void OnItemRemovedListener( IEntity item, BaseInventoryStorageComponent storage );
	override bool OnClick( Widget w, int x, int y, int button );
	void FilterOutStorages( false ););
	void MoveItem();
	void MoveWeapon();
	void SCR_InventoryMenuUI();
	#endif
};



