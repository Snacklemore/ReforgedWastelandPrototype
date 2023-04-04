
modded class SCR_InventorySlotUI : ScriptedWidgetComponent
{
	[Attribute("1", UIWidgets.ComboBox, "Slot size", "", ParamEnumArray.FromEnum(ESlotSize) )]
	private ESlotSize m_ESlotSize;
	protected int										m_iSizeX;
	protected int										m_iSizeY;
	protected int										m_iPage;				//helper variable telling us what page the slot should be stored on
	
	private WorkspaceWidget 							m_workspaceWidget 		= null;
	Widget												m_widget 				= null;	
	private ButtonWidget								m_wButton;
	private ImageWidget									m_wIcon 				= null;
	private RenderTargetWidget							m_wPreviewImage 		= null;
	
//	protected SCR_InventoryStorageManagerComponent	m_InventoryManager		= null;		//TODO: not needed can be taken from storageBaseUI
	protected SCR_InventoryStorageBaseUI				m_pStorageUI;
		
	protected InventoryItemComponent 					m_pItem;
	protected ref SCR_ItemAttributeCollection			m_Attributes;
	const string										ITEM_LAYOUT = "{0921F9EB5F3843BA}UI/layouts/Menus/Inventory/Inventory20Item.layout";
	const int											LMB_CLICK	= 0;
	const int											RMB_CLICK	= 1;
	const int											MID_CLICK	= 2;
	protected bool										m_bEnabled 	= true;
	protected bool										m_bSelected = false;
	protected Widget									m_wSelectedEffect, m_wMoveEffect, m_wDimmerEffect;
	protected TextWidget								m_wTextQuickSlot = null;
	protected TextWidget								m_wTextQuickSlotLarge = null;
	protected int										m_iQuickSlotIndex;
	
	protected ProgressBarWidget							m_wVolumeBar, m_wAmmoCount, m_wCurrentMagazineAmmoCount;
	protected int										m_iStackNumber = 1;
	protected TextWidget								m_wStackNumber, m_wMagazineNumber;
	protected OverlayWidget								m_wItemLockThrobber;
	protected string								   	m_sItemName = "";
	protected int										m_aAmmoCountActual = -1;
	protected int										m_aAmmoCountMax = -1;
	
	
	protected bool										m_bVisible;
	protected ESlotFunction								m_eSlotFunction = ESlotFunction.TYPE_GENERIC;
	
	#ifdef DEBUG_INVENTORY20
		protected TextWidget							m_wDbgClassText1;
		protected TextWidget							m_wDbgClassText2;
		protected TextWidget							m_wDbgClassText3;
	#endif

	//------------------------------------------------------------------------ USER METHODS ------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------
	void UpdateReferencedComponent( InventoryItemComponent pComponent )
	{
		if ( m_widget )
			Destroy();
		m_pItem = pComponent;
		
		if (m_pItem && m_pItem.GetAttributes())
			m_Attributes = SCR_ItemAttributeCollection.Cast( m_pItem.GetAttributes() );			//set the slot attributes (size) based on the information stored in the item 
		if(! m_Attributes)
			return;
		if (m_pItem && !m_Attributes.IsVisible())
			return;

		auto vehicleAttributes = SCR_InventoryVehicleVisibilityAttribute.Cast(m_Attributes.FindAttribute(SCR_InventoryVehicleVisibilityAttribute));
		if (vehicleAttributes && !ShouldVehicleSlotBeVisible(vehicleAttributes))
			return;

		m_workspaceWidget = GetGame().GetWorkspace();
		Widget wGrid = m_pStorageUI.GetStorageGrid();
		m_widget = m_workspaceWidget.CreateWidgets( SetSlotSize(), wGrid );
		m_widget.AddHandler( this );	//calls the HandlerAttached()
	}
	
	//------------------------------------------------------------------------------------------------
	void UpdateInventorySlot(InventoryItemComponent comp, int stackNumber)
	{
		m_pItem = comp;
		m_iStackNumber = stackNumber;
		Refresh();
	}

	//------------------------------------------------------------------------------------------------
	bool IsDraggable()
	{
		if (!m_Attributes)
			return true;

		return m_Attributes.IsDraggable();
	}

	//------------------------------------------------------------------------------------------------
	protected string SetSlotSize()
	{
		string slotLayout = SLOT_LAYOUT_1x1;
		switch ( m_Attributes.GetItemSize() ) 
		{
			case ESlotSize.SLOT_1x1:	{ slotLayout = SLOT_LAYOUT_1x1; m_iSizeX = 1; m_iSizeY = 1; } break;
			case ESlotSize.SLOT_2x1:	{ slotLayout = SLOT_LAYOUT_2x1; m_iSizeX = 2; m_iSizeY = 1; } break;
			case ESlotSize.SLOT_2x2:	{ slotLayout = SLOT_LAYOUT_2x2; m_iSizeX = 2; m_iSizeY = 2; } break;
			case ESlotSize.SLOT_3x3:	{ slotLayout = SLOT_LAYOUT_3x3; m_iSizeX = 3; m_iSizeY = 3; } break;
		}
		return slotLayout;
	}
	//------------------------------------------------------------------------------------------------
	protected void Init()
	{
		m_iPage = -1;		//no page placement by default
		//and create the visual slot
		
		m_wTextQuickSlot = TextWidget.Cast( m_widget.FindAnyWidget( "TextQuickSlot" ) );
		m_sItemName = GetItemName();	//debug purposes
		
		if (m_pItem)
			m_pItem.m_OnLockedStateChangedInvoker.Insert(OnChangeLockState);
		
		//m_widget = w;
		SetSlotVisible( m_bVisible );
		SetItemFunctionality();
		// If is it storage or attachment, register it to the array for the future use
		if (!BaseInventoryStorageComponent.Cast(m_pItem) && !WeaponAttachmentsStorageComponent.Cast(m_pItem))
			return;
		
		if (m_pStorageUI && m_pStorageUI.GetInventoryMenuHandler())
			m_pStorageUI.GetInventoryMenuHandler().RegisterUIStorage(this);
	}
	
	//------------------------------------------------------------------------------------------------	
	//! returns the storage component associated with this UI component
	BaseInventoryStorageComponent GetStorageComponent() { return BaseInventoryStorageComponent.Cast(m_pItem); }
	
	//------------------------------------------------------------------------------------------------
	LoadoutAreaType GetLoadoutArea()
	{
		if ( !m_pItem )
			return null;
		auto pClothComponent = BaseLoadoutClothComponent.Cast( m_pItem.GetOwner().FindComponent( BaseLoadoutClothComponent ) );
		if ( !pClothComponent )
			return null;
		
		return pClothComponent.GetAreaType();
	}
	
	
	//------------------------------------------------------------------------------------------------
	void SetStackNumber( int i ) { m_iStackNumber = i; }
	void IncreaseStackNumber() { m_iStackNumber++; }
	void IncreaseStackNumber( int i ) { m_iStackNumber = m_iStackNumber + i; }

	//------------------------------------------------------------------------------------------------	
	//!
	protected void SetAmmoCount()
	{
		if ( !m_pItem )
			return;
		MagazineComponent pMagazineComponent = MagazineComponent.Cast( m_pItem.GetOwner().FindComponent( MagazineComponent ) );
		if ( !pMagazineComponent )
			return;
		m_aAmmoCountActual = pMagazineComponent.GetAmmoCount();
		m_aAmmoCountMax = pMagazineComponent.GetMaxAmmoCount();
	}
	
	//------------------------------------------------------------------------------------------------
	Widget GetButtonWidget() { return m_wButton; }	
	//------------------------------------------------------------------------------------------------
	bool IsSlotEnabled() { return m_bEnabled; }
	//------------------------------------------------------------------------------------------------
	bool IsSlotSelected() { return m_bSelected; }
	//------------------------------------------------------------------------------------------------
	//! should be the slot visible?
	void SetSlotVisible( bool bVisible )
	{
		m_bVisible = bVisible;
		m_widget.SetEnabled( bVisible );
		m_widget.SetVisible( bVisible );
		
		if( bVisible )
		{
			m_wPreviewImage = RenderTargetWidget.Cast( m_widget.FindAnyWidget( "item" ) );
			ItemPreviewManagerEntity manager = GetGame().GetItemPreviewManager();
			if (manager)
			{
				ItemPreviewWidget renderPreview = ItemPreviewWidget.Cast( m_wPreviewImage );
				IEntity previewEntity = null;
				if (m_pItem)
					previewEntity = m_pItem.GetOwner();
				if (renderPreview)
					manager.SetPreviewItem(renderPreview, previewEntity, null, true);
			}
			
			//if the slot has storage, then show its volume bar
			BaseInventoryStorageComponent pStorageTo = GetStorageComponent();
			if ( pStorageTo )
			{
				m_wVolumeBar = ProgressBarWidget.Cast( m_widget.FindAnyWidget( "VolumeBar" ) );
				m_wVolumeBar.SetVisible( true );
			}
			m_wSelectedEffect = m_widget.FindAnyWidget( "SelectedFrame" );
			m_wMoveEffect = m_widget.FindAnyWidget( "IconMove" );
			m_wDimmerEffect = m_widget.FindAnyWidget( "Dimmer" );
			m_wButton = ButtonWidget.Cast( m_widget.FindAnyWidget( "ItemButton" ) );
			m_wStackNumber = TextWidget.Cast( m_widget.FindAnyWidget( "stackNumber" ) );
			m_wItemLockThrobber = OverlayWidget.Cast(m_widget.FindAnyWidget("itemLockThrobber"));
		
			if ( m_iStackNumber > 0 )
			{
				//get moneyvalue
				MoneyComponent parent;
				if (m_pItem)
				{
					 parent = MoneyComponent.Cast( m_pItem.GetOwner().FindComponent(MoneyComponent));

				
				}
				if (parent)
				{
					m_wStackNumber.SetText( parent.GetValue().ToString() );
					m_wStackNumber.SetVisible( true );

				}
				else
				{
					m_wStackNumber.SetText( m_iStackNumber.ToString() );
					m_wStackNumber.SetVisible( true );
				}
				
			}
			else
			{
				m_wStackNumber.SetVisible( false );
			}
			
			m_wAmmoCount = ProgressBarWidget.Cast( m_widget.FindAnyWidget( "ammoCount" ) );
			if ( m_wAmmoCount )
			{
				SetAmmoCount();
				UpdateAmmoCount();
			}
			
			m_wMagazineNumber = TextWidget.Cast( m_widget.FindAnyWidget( "magazineCount" ) );
			m_wCurrentMagazineAmmoCount = ProgressBarWidget.Cast( m_widget.FindAnyWidget( "currentMagazineAmmoCount" ) );
			if ( m_wMagazineNumber && m_wCurrentMagazineAmmoCount )
			{
				UpdateWeaponAmmoCount();
			}
			
			#ifdef DEBUG_INVENTORY20
				if ( !m_pItem )
				{
					array<string> dbgText = new array<string>();
					this.ToString().Split( "<", dbgText, false );
 					m_wDbgClassText1 = TextWidget.Cast( m_widget.FindAnyWidget( "dbgTextClass1" ) );
					m_wDbgClassText2 = TextWidget.Cast( m_widget.FindAnyWidget( "dbgTextClass2" ) );
					m_wDbgClassText3 = TextWidget.Cast( m_widget.FindAnyWidget( "dbgTextClass3" ) );
					m_wDbgClassText1.SetText( dbgText[0] );
					m_wDbgClassText1.SetEnabled( true );
					m_wDbgClassText1.SetVisible( true );
					m_wDbgClassText2.SetText( dbgText[1] );
					m_wDbgClassText2.SetEnabled( true );
					m_wDbgClassText2.SetVisible( true );
					m_pItem.ToString().Split( "<", dbgText, false );
					if ( dbgText.Count() > 1 )
					{
						m_wDbgClassText3.SetText( dbgText[1] );
						m_wDbgClassText3.SetEnabled( true );
						m_wDbgClassText3.SetVisible( true );
					}
				}
			#endif
			UpdateVolumeBarValue();
			
		}
		else
		{
			m_wPreviewImage = null;
			m_wSelectedEffect = null;
			m_wMoveEffect = null;
			m_wDimmerEffect = null;
			m_wButton = null;
			m_wVolumeBar = null;
			m_wTextQuickSlot = null;
			m_wTextQuickSlotLarge = null;
			m_wStackNumber = null;
			#ifdef DEBUG_INVENTORY20
				m_wDbgClassText1 = null;
				m_wDbgClassText2 = null;
				m_wDbgClassText3 = null;
			#endif
		}
	}

	//------------------------------------------------------------------------------------------------
	// ! 
	int GetSlotIndex() { return m_iQuickSlotIndex; }
	
	//------------------------------------------------------------------------------------------------
	// ! 
	void SetQuickSlotIndexVisible( TextWidget textQuickSlot, bool bVisible )
	{
		if ( textQuickSlot && GetGame().GetInputManager().IsUsingMouseAndKeyboard() )
		{
			textQuickSlot.SetText( ( (m_iQuickSlotIndex + 1) % 10 ).ToString() );
			textQuickSlot.SetVisible( bVisible );
		}
	}
	
	
	//------------------------------------------------------------------------------------------------	
	void OnSlotFocused()
	{
		if ( !m_pStorageUI )
			return;
		m_pStorageUI.SetSlotFocused( this, true );
		SCR_InventoryMenuUI pMenuManager = m_pStorageUI.GetInventoryMenuHandler();
		if ( pMenuManager )
		{
			#ifdef DEBUG_INVENTORY20
			
			BaseInventoryStorageComponent dbgStorage;
			InventoryStorageSlot dbgStorageParentSlot;
			
			if ( m_pStorageUI.GetStorage() )
			{
				dbgStorage = m_pStorageUI.GetStorage();
				if ( m_pStorageUI.GetStorage().GetParentSlot() )
					dbgStorageParentSlot = m_pStorageUI.GetStorage().GetParentSlot();
			}
						
			PrintFormat( "INV:OnSlotFocused | m_pStorageUI: %1 | Focused slotUI: %2 | Parent storage: %3 | from parent slot: %4", m_pStorageUI, this, dbgStorage, dbgStorageParentSlot );
			#endif
			pMenuManager.SetSlotFocused( this, GetStorageUI(), true );
		}
	}
	
	//------------------------------------------------------------------------------------------------	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		OnSlotFocused();
		if ( m_wButton )
			if ( m_wButton.FindHandler( SCR_ButtonComponent ) )
				m_wButton.FindHandler( SCR_ButtonComponent ).OnFocus( w, x, y );
		return false;
	}
		
	//------------------------------------------------------------------------------------------------	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		OnSlotFocusLost();
		if ( m_wButton )
			if ( m_wButton.FindHandler( SCR_ButtonComponent ) )
				m_wButton.FindHandler( SCR_ButtonComponent ).OnFocusLost( w, x, y );
		return false;
	}
	
	//------------------------------------------------------------------------------------------------	
	void OnSlotFocusLost()
	{
		if (!m_pStorageUI)
			return;
		m_pStorageUI.SetSlotFocused( this, false );
		SCR_InventoryMenuUI pMenuManager = m_pStorageUI.GetInventoryMenuHandler();
		if ( pMenuManager )
			pMenuManager.SetSlotFocused( this, m_pStorageUI, false );	
	}
	
	//------------------------------------------------------------------------------------------------	
	void ToggleSelected()
	{
		m_bSelected = !m_bSelected;
		if ( !m_wSelectedEffect )
			return;

		m_wSelectedEffect.SetVisible( m_bSelected );
		m_pStorageUI.SetSlotSelected( this, m_bSelected );
	}
	//------------------------------------------------------------------------------------------------
	void SetSelected( bool select )
	{
		//TODO: show the selected effect should be done as a component
		m_bSelected = select;
		if ( !m_wSelectedEffect )
			return;

		m_wSelectedEffect.SetVisible(select);
	}
	
	//------------------------------------------------------------------------------------------------
	void SetEnabled( bool enable )
	{
		if( enable )
			m_widget.SetOpacity( BUTTON_OPACITY_ENABLED );
		else
			m_widget.SetOpacity( BUTTON_OPACITY_DISABLED );
			
		m_widget.SetEnabled( enable );
	}
	
	bool OnDrop(SCR_InventorySlotUI slot)
	{
		return false;
	}

	//------------------------------------------------------------------------------------------------
	// ! 0 - Disable for move
	// ! 1 - Enable for move
	// ! 2 - Reset
	void SetEnabledForMove( int iSelect )
	{
		switch ( iSelect )
		{
			case 0:
			{
				m_widget.SetEnabled( false );
				if ( m_wMoveEffect )
					m_wMoveEffect.SetVisible( false );	
			} break;
			
			case 1:
			{
				m_widget.SetEnabled( true );
				if ( m_wMoveEffect )
					m_wMoveEffect.SetVisible( true );	
			
			} break;
			
			case 2:
			{
				m_widget.SetEnabled( true );
				if ( m_wMoveEffect )
					m_wMoveEffect.SetVisible( false );	
			} break;
			
		}

	}
		
	//------------------------------------------------------------------------------------------------
	InventoryItemComponent GetInventoryItemComponent() { return m_pItem; }
	//------------------------------------------------------------------------------------------------
	BaseInventoryStorageComponent GetAsStorage() { return BaseInventoryStorageComponent.Cast( m_pItem ); }
	
	//------------------------------------------------------------------------------------------------
	// ! Returns the UI storage the slot is associated with
	SCR_InventoryStorageBaseUI GetStorageUI() { return m_pStorageUI; }	
	
	//------------------------------------------------------------------------------------------------
	bool IsSelected() { return m_bSelected; }
	
	//------------------------------------------------------------------------------------------------
	protected UIInfo GetItemDetails()
	{
		if( !m_Attributes )
			return null;
		return m_Attributes.GetUIInfo();
	}
	
	//------------------------------------------------------------------------------------------------
	string GetItemName()
	{
		UIInfo itemDetails =  GetItemDetails();
		if( !itemDetails )
			return string.Empty;
		return itemDetails.GetName();	
	}
	
	//------------------------------------------------------------------------------------------------
	
	
	//------------------------------------------------------------------------------------------------	
	void ClearItemDetails()
	{
		if( m_pStorageUI )
			m_pStorageUI.ShowItemDetails( "", "", "" );
	}
	
	
	//------------------------------------------------------------------------------------------------
	

	//------------------------------------------------------------------------------------------------	
	protected SCR_InventoryStorageManagerComponent GetInventoryManager()	{ return m_pStorageUI.GetInventoryManager(); }
	
	//------------------------------------------------------------------------------------------------	
	bool RemoveItem()
	{
		SCR_InventoryStorageManagerComponent invMan = GetInventoryManager();
		if (!invMan || !invMan.CanMoveItem(m_pItem.GetOwner()))
			return false;
		InventoryStorageSlot pSlot = m_pItem.GetParentSlot();
		if( !pSlot )
			return false;
		auto pStorage = pSlot.GetStorage();
		if( !pStorage )
			return false;
		if(!invMan.CanRemoveItemFromStorage(m_pItem.GetOwner(), pStorage))
			return false;
		
		//TODO: return the check back once the true/false issue is solved
		//if(!invMan().TryRemoveItemFromInventory(m_pItem.GetOwner(), pStorage))
		//return false;		//can't be removed, exit				

		invMan.TryRemoveItemFromInventory(m_pItem.GetOwner(), pStorage);
		
		if( !m_pStorageUI )
			return false;
		if ( m_widget )
		{		
			m_widget.RemoveHandler( this );
			m_widget.RemoveFromHierarchy();
		}
		m_pStorageUI.RemoveSlotUI( this );
		//delete this;
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------	
	//! stores the type of the functionality of the item in the slot
	void SetItemFunctionality()
	{
		if( !m_pItem )
			return;
		IEntity item = m_pItem.GetOwner();
		if (!item)
			return;
		
		if ( MagazineComponent.Cast( item.FindComponent( MagazineComponent ) ) )
		{
			m_eSlotFunction = ESlotFunction.TYPE_MAGAZINE;
			SetAmmoCount();
			return;
		} 
		
		if ( WeaponComponent.Cast( item.FindComponent( WeaponComponent ) ) )
		{
			m_eSlotFunction = ESlotFunction.TYPE_WEAPON;
			return;
		}
		
		if ( SCR_GadgetComponent.Cast( item.FindComponent( SCR_GadgetComponent ) ) )
		{
			m_eSlotFunction = ESlotFunction.TYPE_GADGET;
			return;
		}
	}
	
	//------------------------------------------------------------------------------------------------	
	SCR_CharacterInventoryStorageComponent GetCharacterStorage(IEntity entity)
	{
		ChimeraCharacter character = ChimeraCharacter.Cast(entity);
		if (!character)
			return null;
		
		CharacterControllerComponent controller = character.GetCharacterController();
		if (!controller)
			return null;
		
		SCR_InventoryStorageManagerComponent storageManager = GetInventoryManager();
		if (!storageManager)
			return null;
		
		return storageManager.GetCharacterStorage();
	}
	
	//------------------------------------------------------------------------------------------------	
	void UseItem(IEntity player)
	{
		if (!m_pItem)
			return;
		
		IEntity item = m_pItem.GetOwner();
		if (!item)
			return;
		
		SCR_CharacterInventoryStorageComponent storage = GetCharacterStorage(player);
		if (!storage)
			return;
		
		if (storage.UseItem(item, m_eSlotFunction))
			Refresh();
	}
	
	//------------------------------------------------------------------------------------------------	
	bool CanUseItem(IEntity player)
	{
		if (!m_pItem)
			return false;
		
		IEntity item = m_pItem.GetOwner();
		if (!item)
			return false;
		
		SCR_CharacterInventoryStorageComponent storage = GetCharacterStorage(player);
		if (!storage)
			return false;
		
		return storage.CanUseItem(item, m_eSlotFunction);
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool ShouldVehicleSlotBeVisible(SCR_InventoryVehicleVisibilityAttribute attr)
	{
		IEntity vehicle = m_pItem.GetOwner();
		SCR_VehicleDamageManagerComponent dm = SCR_VehicleDamageManagerComponent.Cast(vehicle.FindComponent(SCR_VehicleDamageManagerComponent));
		if (dm && dm.IsDestroyed())
			return false;

		ChimeraCharacter player = ChimeraCharacter.Cast(GetGame().GetPlayerController().GetControlledEntity());

		if (attr.GetVisibleInVehicleOnly() && !player.IsInVehicle())
			return false;

		if (!attr.GetVisibleForVehicleFactionOnly())
			return false;

		if (!Vehicle.Cast(vehicle))
			vehicle = vehicle.GetParent();

		FactionAffiliationComponent vehicleFaction = FactionAffiliationComponent.Cast(vehicle.FindComponent(FactionAffiliationComponent));
		if (!vehicleFaction || !vehicleFaction.GetAffiliatedFaction())
			return true;

		FactionAffiliationComponent playerFaction = FactionAffiliationComponent.Cast(player.FindComponent(FactionAffiliationComponent));
		if (!playerFaction || !playerFaction.GetAffiliatedFaction())
			return false;

		return (playerFaction.GetAffiliatedFaction() == vehicleFaction.GetAffiliatedFaction());
	}

	//------------------------------------------------------------------------------------------------	
	// !
	protected void UpdateVolumeBarValue()
	{
		if ( m_wVolumeBar && m_pItem )
		{
			BaseInventoryStorageComponent pStorage = GetAsStorage();
			if ( !pStorage )
				return;
			
			string name = ""; 
			if ( pStorage.GetAttributes() && pStorage.GetAttributes().GetUIInfo() )
			{
				name = pStorage.GetAttributes().GetUIInfo().GetName();
			}
			
			float fOccupied = 0.0;
			//TODO: The folowing part needs refactor! This enumeration of attached storages is used on more places.
			if ( pStorage.Type() == ClothNodeStorageComponent )
			{
				array<BaseInventoryStorageComponent> pOwnedStorages = new array<BaseInventoryStorageComponent>();
				pStorage.GetOwnedStorages( pOwnedStorages, 1, false );
				foreach ( BaseInventoryStorageComponent pSubStorage : pOwnedStorages )
					fOccupied += pSubStorage.GetOccupiedSpace();
			}
			else
			{
				fOccupied = pStorage.GetOccupiedSpace();
			}
			float fCapacity = pStorage.GetMaxVolumeCapacity();
			m_wVolumeBar.SetMax( fCapacity );
			m_wVolumeBar.SetCurrent( fOccupied );
			
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void UpdateAmmoCount()
	{
		if ( m_eSlotFunction != ESlotFunction.TYPE_MAGAZINE )
			return;
		if( !m_pItem )
			return;

		if ( m_aAmmoCountActual > -1 && m_aAmmoCountMax != 1 )
		{
			m_wAmmoCount.SetMax( m_aAmmoCountMax );
			m_wAmmoCount.SetCurrent( m_aAmmoCountActual );
			m_wAmmoCount.SetVisible( true );
		}
		else
		{
			m_wAmmoCount.SetVisible( false );
		}
	}

	//------------------------------------------------------------------------------------------------	
	//!
	int GetAmmoCount() { return m_aAmmoCountActual; }
		
	//------------------------------------------------------------------------------------------------
	protected void UpdateStackNumber()
	{
		if ( !m_wStackNumber )
			return;

		m_wStackNumber.SetVisible(m_iStackNumber > 1);
		m_wStackNumber.SetText( m_iStackNumber.ToString() );
	}
	
	//------------------------------------------------------------------------------------------------
	protected void UpdateWeaponAmmoCount()
	{
		if (!m_pItem)
			return;
		if (!m_wMagazineNumber)
			return;
		// Check if the storage is vicinity. We don't want to display ammo counts for weapons that are on the ground.
		if (SCR_InventoryStorageLootUI.Cast(GetStorageUI()))
			return;
		BaseWeaponComponent weaponComp = BaseWeaponComponent.Cast( m_pItem.GetOwner().FindComponent( BaseWeaponComponent ) );
		if (!weaponComp)
			return;
		BaseMuzzleComponent weaponMuzzleComp = weaponComp.GetCurrentMuzzle();
		if (!weaponMuzzleComp)
			return;
		
		int currentAmmoCount = weaponMuzzleComp.GetAmmoCount();
		int maxAmmoCount = weaponMuzzleComp.GetMaxAmmoCount();
		int magazineCount = GetInventoryManager().GetMagazineCountByWeapon(weaponComp);
		
		m_wMagazineNumber.SetText("+" + magazineCount);

		m_wCurrentMagazineAmmoCount.SetMax(maxAmmoCount);
		m_wCurrentMagazineAmmoCount.SetCurrent(currentAmmoCount);

		m_wMagazineNumber.SetVisible(true);
		m_wCurrentMagazineAmmoCount.SetVisible(true);

		if (currentAmmoCount > 0 && magazineCount == 0)
		{
			m_wCurrentMagazineAmmoCount.SetColorInt( CURRENT_AMMO_MAGAZINE );
			m_wMagazineNumber.SetOpacity( 0 );
		}
		else if (currentAmmoCount == 0 && magazineCount == 0)
		{
			m_wMagazineNumber.SetColorInt( CURRENT_AMMO_EMPTY );
			m_wMagazineNumber.SetOpacity( 1 );
			
			m_wCurrentMagazineAmmoCount.SetColorInt( CURRENT_AMMO_EMPTY );
			m_wCurrentMagazineAmmoCount.SetCurrent( maxAmmoCount );
			m_wCurrentMagazineAmmoCount.SetOpacity( CURRENT_AMMO_OPACITY );
		}
		else
		{
			m_wMagazineNumber.SetColorInt( CURRENT_AMMO_TEXT );
			m_wMagazineNumber.SetOpacity( 1 );
			
			m_wCurrentMagazineAmmoCount.SetColorInt( CURRENT_AMMO_MAGAZINE );
			m_wCurrentMagazineAmmoCount.SetOpacity( CURRENT_AMMO_OPACITY );
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void Refresh()
	{
		UpdateVolumeBarValue();
		UpdateStackNumber();
		UpdateAmmoCount();
		UpdateWeaponAmmoCount();
	}
		
	
	//------------------------------------------------------------------------------------------------	
	//!Removes just the UI slot
	void Destroy()
	{
		if (m_pStorageUI.GetInventoryMenuHandler())
			m_pStorageUI.GetInventoryMenuHandler().UnregisterUIStorage( this );
		
		m_widget.RemoveHandler( this );
		m_widget.RemoveFromHierarchy();
		if (m_wPreviewImage)
		{
			ItemPreviewManagerEntity manager = GetGame().GetItemPreviewManager();
			if (manager)
				{
					ItemPreviewWidget renderPreview = ItemPreviewWidget.Cast( m_wPreviewImage );
					if (renderPreview)
						manager.SetPreviewItem(renderPreview, null);
				}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void OnChangeLockState()
	{
		if (!m_pItem)
			return;

		if (m_wItemLockThrobber)
			m_wItemLockThrobber.SetVisible(m_pItem.IsLocked());
	}
	
	//------------------------------------------------------------------------------------------------
	void SetSelectedQuickSlot( bool select )
	{
		m_bSelected = select;
		if ( !m_wSelectedEffect )
			return;
		m_wSelectedEffect.SetVisible( select );
	}
	
	//------------------------------------------------------------------------------------------------
	ECommonItemType GetCommonItemType()
	{
		if ( !m_Attributes )
			return ECommonItemType.NONE;
		return m_Attributes.GetCommonType();
	}
	
	//------------------------------------------------------------------------------------------------
	
	
	//------------------------------------------------------------------------ COMMON METHODS ----------------------------------------------------------------------
	
	//------------------------------------------------------------------------------------------------
	override bool OnSelect(Widget w, int x, int y)
	{
		//GetGame().GetWorkspace().SetFocusedWidget(w);
		//w.SetOpacity(  );
		return false;
	}
		
	//------------------------------------------------------------------------------------------------	
	override bool OnFocus(Widget w, int x, int y)
	{
		OnSlotFocused();
		return false;
	}
	
	//------------------------------------------------------------------------------------------------	
	override bool OnFocusLost(Widget w, int x, int y)
	{
		OnSlotFocusLost();
		return false;
	}
	
	//------------------------------------------------------------------------------------------------	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		ClearItemDetails();
		return false;
	}
	/*
	//------------------------------------------------------------------------------------------------	
	override bool OnMouseButtonDown( Widget w, int x, int y, int button )
	{
		m_mouseButtonDown = true;
		PrintFormat( "INV: widget: %1, ( %2, %3 ), button: %4 DOWN", w, x, y, button );
		return false;
	}
	
	//------------------------------------------------------------------------------------------------	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		m_mouseButtonDown = false;
		PrintFormat( "INV: widget: %1, ( %2, %3 ), button: %4 UP", w, x, y, button );
		return false;
	}
	*/

	//------------------------------------------------------------------------------------------------	
	Widget GetWidget()	{ return m_widget; }
	
	//------------------------------------------------------------------------------------------------	
	int GetColumnSize() { return m_iSizeX; }
	
	//------------------------------------------------------------------------------------------------	
	int GetRowSize() { return m_iSizeY; }
		
	//------------------------------------------------------------------------------------------------
	void SetSlotSize( ESlotSize slotSize) { m_ESlotSize = slotSize; }
	
	//------------------------------------------------------------------------------------------------
	ESlotSize GetSlotSize()	{ return m_ESlotSize; }
	
	//------------------------------------------------------------------------------------------------	
	void SetPage( int iPage )	{ m_iPage = iPage; }
	
	//------------------------------------------------------------------------------------------------	
	int GetPage()	{ return m_iPage; }
	
	//------------------------------------------------------------------------------------------------	
	//! What functionality the item in the slot has? ( weapon, magazine, health, consumable... )
	ESlotFunction GetSlotedItemFunction() { return m_eSlotFunction; }
	
	//------------------------------------------------------------------------------------------------
	void SetIcon( ResourceName texture )
	{
		m_wIcon = ImageWidget.Cast( m_widget.FindAnyWidget( "icon" ) );
		if ( !m_wIcon )
			return;
		m_wIcon.LoadImageTexture( 0, texture );
		m_wIcon.SetVisible( true );
	}
		
	//------------------------------------------------------------------------------------------------
	override void HandlerAttached( Widget w )
	{
		Init();	
	}

	//------------------------------------------------------------------------------------------------
	ResourceName GetItemResource()
	{
		if (!m_pItem)
			return ResourceName.Empty;
		return m_pItem.GetOwner().GetPrefabData().GetPrefabName();
	}

	//------------------------------------------------------------------------------------------------
	bool IsStacked()
	{
		return (m_iStackNumber > 1);
	}

	//------------------------------------------------------------------------------------------------
	void SCR_InventorySlotUI( InventoryItemComponent pComponent = null, SCR_InventoryStorageBaseUI pStorageUI = null, bool bVisible = true, int iSlotIndex = -1, SCR_ItemAttributeCollection pAttributes = null )
	{
		m_pStorageUI = pStorageUI;	
		m_bVisible = bVisible;
		m_iQuickSlotIndex = iSlotIndex;
		m_Attributes = pAttributes;
		UpdateReferencedComponent( pComponent );
	}
	
	//------------------------------------------------------------------------------------------------
	void ~SCR_InventorySlotUI()
	{
	}
};