class WST_Vehicle
{
	//gonna need another component which searches for SCR_Position to spawn vehicle on
	
	/////////////////////////////////////////////////////////////////////////
	//prefab item paths
	static ResourceName US_Jeep = "{F649585ABB3706C4}Prefabs/Vehicles/Wheeled/M151A2/M151A2.et";
	

	//unique itemidentfier
	static string US_JeepIdentifier = "US_Jeep";
	
	//display name 
	static string US_JeepDisplay = "Jeep";
	
	//item price
	static int US_JeepPrice = 100;
	
	
	ref map<string,ResourceName> buildArray = new map<string,ResourceName>();
	ref array<string> iterationArray = new array<string>();
	ref map<string,string> DisplayNameArray = new map<string,string>();
	ref map<string,int> TypeArray = new map<string,int>();
	ref map<string,int> PriceArray = new map<string,int>();

	private WST_Vehicle instance;
	WST_Vehicle GetInstance()
	{
		if(instance)
			return instance;
		else
			return new WST_Vehicle();
	}
	
	int GetPriceByKey(string identifier)
	{
		return PriceArray.Get(identifier);
	}
	void WST_Vehicle()
	{
		if (instance)
			return;
		instance = this;
		//
		buildArray.Insert(US_JeepIdentifier,US_Jeep);
		
		iterationArray.Insert(US_JeepIdentifier);
		
		DisplayNameArray.Insert(US_JeepIdentifier,US_JeepDisplay);
		
		TypeArray.Insert(US_JeepIdentifier,WST_Type.WST_VEHICLE);
		
		PriceArray.Insert(US_JeepIdentifier,50);
		
		
	
	}
	
}

class WST_VehicleShop : MenuBase
{
	protected static const string TEXT_TITLE = "TextTitle";
	protected static const string BUTTON_CLOSE = "QuitButton";
	protected static const string BUTTON_BUY = "BuyButton";
	ResourceName spawnEntity;
	ref WST_Vehicle v;
	WST_TraderComponent TrdComp;
	int balance;
	
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

		          	Print("WST_VehicleShop::OnSelected::noWallet");
		           	return;
						
						
	        	}
					balance = wallet.GetValue();
				    balance = balance - v.GetPriceByKey(key);
				
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
		traderComp.HandleVehicleBuyAction(spawnEntity,balance);
	
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
	
		SetActionContext("VehicleShop");
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
					m_ListBoxComponent.AddItem(o.GetDisplayText(),o);
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
#ifdef WORKBENCH
			inputManager.RemoveActionListener("MenuOpenWB", EActionTrigger.DOWN, Close);
			inputManager.RemoveActionListener("MenuBackWB", EActionTrigger.DOWN, Close);
#endif
		}
	}

	//------------------------------------------------------------------------------------------------


}