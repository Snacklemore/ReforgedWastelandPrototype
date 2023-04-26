


class WST_VehicleShop : MenuBase
{
	protected static const string TEXT_TITLE = "TextTitle";
	protected static const string BUTTON_CLOSE = "QuitButton";
	protected static const string BUTTON_BUY = "BuyButton";
	ResourceName spawnEntity;
	ref WST_Vehicle v;
	WST_TraderComponent TrdComp;
	int balance;
	int itemPrice;
	int pSelectedIndex = 0;
	void OnSelected(SCR_ListBoxComponent list , int itemIndex, bool isTheNewSelection)
	{
		Print("WST_VehicleShop::OnSelectedInvoker");
		Widget rootWidget = GetRootWidget();

		ManagedDataObject data =  list.GetItemData(itemIndex);
		Widget panelRoot = rootWidget.FindWidget("Panel0");
		ItemPreviewWidget preview = panelRoot.FindWidget("VehiclePreview");
		string vehicle
		if (data)
			vehicle = data.GetData();
		
		foreach (ManagedDataObject dataObject : dataObjects)
		{
			if (dataObject.GetData() == vehicle)
			{
				
				
				//"weapon" is the key for the hashmap
				
				ResourceName VehicleEntityPrefab = v.buildArray.Get(vehicle);
				//ResourceName needed here!!
				GetGame().GetItemPreviewManager().SetPreviewItemFromPrefab(preview,VehicleEntityPrefab,null,false);

			}
		}
		///////////////////////////////////////////////////////////////////////------------------Trading CP_Logic
		InventorySearchPredicate search;


		PlayerController pc = GetGame().GetPlayerController();
		IEntity ie = pc.GetControlledEntity();
		InventoryStorageManagerComponent storage = InventoryStorageManagerComponent.Cast(ie.FindComponent(InventoryStorageManagerComponent));

		array<typename> components = {};
		components.Insert(MoneyComponent);
		components.Insert(RplComponent);
		IEntity walletEnity = storage.FindItemWithComponents(components, EStoragePurpose.PURPOSE_DEPOSIT);

		MoneyComponent wallet;
		if(walletEnity)
		{
			wallet = MoneyComponent.Cast(walletEnity.FindComponent(MoneyComponent));
		}
		WST_TraderComponent traderComp = WST_TraderComponent.Cast(ie.FindComponent(WST_TraderComponent));
		if (traderComp) 
		{
	    TrdComp = traderComp;
	
			
			
			
		foreach (string key : v.iterationArray)
		{
			if (key == vehicle)
			{
				if (!wallet) 
				{
					spawnEntity ="";
					balance = -1;
		          	Print("WST_VehicleShop::OnSelected::noWallet");
		           	return;
						
						
	        	}
				balance = wallet.GetValue();
				balance = balance - v.GetPriceByKey(key);
				itemPrice = v.GetPriceByKey(key);
				if (balance < 0) 
				{
					spawnEntity = "";
			
				    Print("WST_VehicleShop::OnSelected::noBalance");
				    return;
				}
				ResourceName n = v.buildArray.Get(key);
				spawnEntity = n;
			}
		}
			
		
		
		}

	}
	
	
	protected void Buy()
	{
		InventorySearchPredicate search;


		PlayerController pc = GetGame().GetPlayerController();
		IEntity ie = pc.GetControlledEntity();
		InventoryStorageManagerComponent storage = InventoryStorageManagerComponent.Cast(ie.FindComponent(InventoryStorageManagerComponent));

		array<typename> components = {};
		components.Insert(MoneyComponent);
		components.Insert(RplComponent);
		IEntity walletEnity = storage.FindItemWithComponents(components, EStoragePurpose.PURPOSE_DEPOSIT);

		MoneyComponent wallet;
		if(walletEnity)
		{
			wallet = MoneyComponent.Cast(walletEnity.FindComponent(MoneyComponent));
		}
		if (!wallet)
			return;
		WST_TraderComponent traderComp = WST_TraderComponent.Cast(ie.FindComponent(WST_TraderComponent));
		if (!traderComp)
		{
			Print("WST_VehicleShop::Buy::No TraderComponent!! ");
			return;
		
		}
		Print("WST_VehicleShop::Buy::spawn entity:  "+ spawnEntity);
		//concept for spawn position setup 
		//-->WST_VehicleSpawnPoint : SCR_Position 
		//-register to GameMode on init 
		//-RplComponent
		//-distance check nearest vehicle spawn point
		
		if(balance < 0)
		{
			SCR_HintManagerComponent.ShowCustomHint("You broke bro... " ,"Not your priceclass.",5.0,false,EFieldManualEntryId.NONE,false);
			return;

		}
		SCR_HintManagerComponent.ShowCustomHint("Payed: "+ itemPrice ,"Item bought!",5.0,false,EFieldManualEntryId.NONE,false);

		traderComp.HandleVehicleBuyAction(spawnEntity,balance);
		
		balance = balance - itemPrice;
	
	}
	///////////////////////////////////////////////////////////////////////------------------PreviewSetup
	
	private ref array<ref ManagedDataObject> dataObjects = new array<ref ManagedDataObject>();
	void setupDataObjects()
	{
		
		for (int i = 0;i < v.iterationArray.Count();i++)
		{
		
			string key = v.iterationArray.Get(i);
			ManagedDataObject o = new ManagedDataObject();
			o.SetData(key);
			o.SetType(v.TypeArray.Get(key));
			o.SetDisplayText(v.DisplayNameArray.Get(key));
			dataObjects.Insert(o);
		}
		
	}
	//------------------------------------------------
	protected override void OnMenuOpen()
	{
		Print("OnMenuOpen::WST_VehicleShop menu/dialog opened!", LogLevel.NORMAL);
	
		v = new WST_Vehicle();
		setupDataObjects();
		
		Widget rootWidget = GetRootWidget();
		if (!rootWidget)
		{
			Print("Error in Layout Tutorial layout creation", LogLevel.ERROR);
			return;
		}
		
		
		
		
		
		Widget panelRoot = rootWidget.FindWidget("Panel0");
		SCR_ListBoxComponent m_ListBoxComponent;
		OverlayWidget m_ListBoxOverlay;
		
		m_ListBoxOverlay =  OverlayWidget.Cast(rootWidget.FindAnyWidget("ListBoxVehicle"));
		m_ListBoxComponent = SCR_ListBoxComponent.Cast(m_ListBoxOverlay.FindHandler(SCR_ListBoxComponent));
		
		m_ListBoxComponent.m_OnChanged.Insert(OnSelected);
		
		 if (m_ListBoxComponent)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_VEHICLE)
				{
					string price = o.GetData();
					int i_price = v.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponent.AddItem(o.GetDisplayText()+"  "+price ,o);
				}
				
			}
           



        }
		
		
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
		{
			// this is for the menu/dialog to close when pressing ESC
			// an alternative is to have a button with the SCR_NavigationButtonComponent component
			// and its Action Name field set to MenuBack - this would activate the button on ESC press
			inputManager.AddActionListener("MenuOpen", EActionTrigger.DOWN, Close);
			inputManager.AddActionListener("MenuBack", EActionTrigger.DOWN, Close);
									inputManager.AddActionListener("MenuChange", EActionTrigger.DOWN, Close);

#ifdef WORKBENCH // in Workbench, F10 is used because ESC closes the preview
			inputManager.AddActionListener("MenuOpenWB", EActionTrigger.DOWN, Close);
			inputManager.AddActionListener("MenuBackWB", EActionTrigger.DOWN, Close);
#endif
		}
		
		
		SCR_ButtonTextComponent buttonClose = SCR_ButtonTextComponent.GetButtonText(BUTTON_CLOSE, rootWidget);
		if (buttonClose)
			buttonClose.m_OnClicked.Insert(Close);
		else
			Print("Button Close not found - won't be able to exit by button", LogLevel.WARNING);

		/*
			buy  button
		*/

		SCR_ButtonTextComponent buttonChange = SCR_ButtonTextComponent.GetButtonText(BUTTON_BUY, rootWidget);
		if (buttonChange)
			buttonChange.m_OnClicked.Insert(Buy);
		else
			Print("Button exit not found", LogLevel.WARNING); // the button can be missing without putting the layout in jeopardy
		
	}

	//------------------------------------------------------------------------------------------------
	protected override void OnMenuClose()
	{
		// here we clean action listeners added above as the good practice wants it
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
		{
			inputManager.RemoveActionListener("MenuOpen", EActionTrigger.DOWN, Close);
			inputManager.RemoveActionListener("MenuBack", EActionTrigger.DOWN, Close);
						inputManager.RemoveActionListener("MenuChange", EActionTrigger.DOWN, Close);

#ifdef WORKBENCH
			inputManager.RemoveActionListener("MenuOpenWB", EActionTrigger.DOWN, Close);
			inputManager.RemoveActionListener("MenuBackWB", EActionTrigger.DOWN, Close);
#endif
		}
	}

	//------------------------------------------------------------------------------------------------


}