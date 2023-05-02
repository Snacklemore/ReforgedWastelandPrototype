

//TODO:add proper hints
class WST_HiddenTrader : MenuBase
{
	protected static const string TEXT_TITLE = "TextTitle";
	protected static const string BUTTON_CLOSE = "QuitButton";
	protected static const string BUTTON_BUY = "BuyButton";
	SCR_TabViewComponent m_TabView;
	BlurWidget wBlur ;
	bool firstSelection = true;
	ref WST_Equipment d;
	ref WST_WeaponV2 c;
	ref WST_ShopLoadOutBase e;
	ResourceName spawnEntity;
	WST_TraderComponent TrdComp;
	int balance;
	int itemPrice;
	
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
		WST_TraderComponent traderComp = WST_TraderComponent.Cast(ie.FindComponent(WST_TraderComponent));
		if (!traderComp)
		{
			Print("WST_HiddenTrader::Buy::No TraderComponent!! ");
			return;
		
		}
		if(balance < 0)
		{
			
			SCR_HintManagerComponent.ShowCustomHint("You broke bro... " ,"Not your priceclass.",5.0,false,EFieldManualEntryId.NONE,false);
			return;
		}
			
		SCR_HintManagerComponent.ShowCustomHint("Payed: "+ itemPrice ,"Item bought!",5.0,false,EFieldManualEntryId.NONE,false);

		traderComp.HandleBuyAction(spawnEntity,balance);
		
		balance = balance - itemPrice;
		
		

	}
	///////////////////////////////////////////////////////////////////////------------------PreviewSetup
	
	private ref array<ref ManagedDataObject> dataObjects = new array<ref ManagedDataObject>();
	void setupDataObjects()
	{
		
		
		//iterate through iterationArray for keys for data map
		
		for (int i = 0;i < e.iterationArray.Count();i++)
		{
		
			string key = e.iterationArray.Get(i);
			ManagedDataObject o = new ManagedDataObject();
			o.SetData(key);
			o.SetType(e.TypeArray.Get(key));
			o.SetDisplayText(e.DisplayNameArray.Get(key));
			dataObjects.Insert(o);
		}
	}
	

	void OnSelected(SCR_ListBoxComponent list , int itemIndex, bool isTheNewSelection)
	{
		///////////////////////////////////////////////////////////////////////------------------PreviewSetup

		Print("WST_HiddenTrader::OnSelectedInvoker");
		Widget rootWidget = GetRootWidget();

		ManagedDataObject data =  list.GetItemData(itemIndex);
		Widget panelRoot = rootWidget.FindWidget("Panel0");
		ItemPreviewWidget preview = panelRoot.FindWidget("WeaponPreview");
		string weapon;
		if (data)
			weapon = data.GetData();
		
		foreach (ManagedDataObject dataObject : dataObjects)
		{
			if (dataObject.GetData() == weapon)
			{
				
				
				//"weapon" is the key for the hashmap
				
				ResourceName WeaponEntityPrefab = e.buildArray.Get(weapon);
				
				//ResourceName needed here!!
				GetGame().GetItemPreviewManager().SetPreviewItemFromPrefab(preview,WeaponEntityPrefab,null,false);
				break;

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
	
			
			
			
			foreach (string key : e.iterationArray)
			{
				if (key == weapon)
				{
					if (!wallet) 
					{
						spawnEntity ="";
						balance = -1;
		            	Print("WST_HiddenTrader::OnSelected::noWallet");
		            	return;
						
						
	        		}
					balance = wallet.GetValue();
				    balance = balance - e.GetPriceByKey(key);
					itemPrice = e.GetPriceByKey(key);
				    if (balance < 0) 
					{
						spawnEntity = "";
			
				         Print("WST_HiddenTrader::OnSelected::noBalance");
				         return;
				     }
					ResourceName n = e.buildArray.Get(key);
				    spawnEntity = n;
					break;
				}
			}
			
			
			
		
		
		}

	}

	SCR_TabViewComponent m_TabviewComponent;
	SCR_ListBoxComponent m_ListBoxComponentTab;
	OverlayWidget m_ListBoxOverlayTab;
		
	SCR_ListBoxComponent m_ListBoxComponentTab2;
	OverlayWidget m_ListBoxOverlayTab2;
	VerticalLayoutWidget m_TabViewOverlay;
	void showTab(int index)
	{
		m_TabviewComponent.ShowTab(index);
	}
	void OnSelectTab()
	{
		
	}
	//------------------------------------------------------------------------------------------------
	protected override void OnMenuOpen()
	{
		Print("OnMenuOpen: menu/dialog opened!", LogLevel.NORMAL);
		

		Widget rootWidget = GetRootWidget();
		if (!rootWidget)
		{
			Print("Error in Layout Tutorial layout creation", LogLevel.ERROR);
			return;
		}




		Widget panelRoot = rootWidget.FindWidget("Panel0");
		ItemPreviewWidget preview = panelRoot.FindWidget("WeaponPreview");

		SCR_ListBoxComponent m_ListBoxComponent;
		OverlayWidget m_ListBoxOverlay;
		
		
		
		SCR_ListBoxComponent m_ListBoxComponentTab3;
		OverlayWidget m_ListBoxOverlayTab3;
		
		
		
		SCR_ListBoxComponent m_ListBoxComponent1;
		OverlayWidget m_ListBoxOverlay1;
		SCR_ListBoxComponent m_ListBoxComponent2;
		OverlayWidget m_ListBoxOverlay2;
		SCR_ListBoxComponent m_ListBoxComponent3;
		OverlayWidget m_ListBoxOverlay3;
		SCR_ListBoxComponent m_ListBoxComponent4;
		OverlayWidget m_ListBoxOverlay4;
		
		
		
		
		m_TabViewOverlay = VerticalLayoutWidget.Cast(rootWidget.FindAnyWidget("TabView"));
		m_TabviewComponent = SCR_TabViewComponent.Cast(m_TabViewOverlay.FindHandler(SCR_TabViewComponent));
		
		m_TabviewComponent.m_OnContentSelect.Insert(OnSelectTab);
		//content of first active tab
		SCR_TabViewContent content1 =  m_TabviewComponent.GetShownTabComponent();
		Widget tabRoot0 = content1.m_wTab;
		//seconde tab content
		SCR_TabViewContent content2 =  m_TabviewComponent.GetEntryContent(1);
		Widget tabRoot2 = content2.m_wTab;
		
		
		//tab 2 listbox
		m_ListBoxOverlayTab2 =  OverlayWidget.Cast(tabRoot2.FindAnyWidget("ListBox0"));
		m_ListBoxComponentTab2= SCR_ListBoxComponent.Cast(m_ListBoxOverlayTab2.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTab2.m_OnChanged.Insert(OnSelected);
		if (m_ListBoxComponentTab2)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_RIFLEAMMO || o.GetType() == WST_Type.WST_ATTACHMENT || o.GetType() == WST_Type.WST_PISTOLAMMO || o.GetType() == WST_Type.WST_SNIPERAMMO)
				{
					string price = o.GetData();
					int i_price = e.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTab2.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		
		
		
		//tab 1 listbox
		m_ListBoxOverlayTab =  OverlayWidget.Cast(tabRoot0.FindAnyWidget("ListBox0"));
		m_ListBoxComponentTab = SCR_ListBoxComponent.Cast(m_ListBoxOverlayTab.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTab.m_OnChanged.Insert(OnSelected);

		 if (m_ListBoxComponentTab)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_RIFLE || o.GetType() == WST_Type.WST_PISTOL || o.GetType() == WST_Type.WST_SNIPERRIFLE )
				{
					string price = o.GetData();
					int i_price = e.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTab.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		
		
		
		m_ListBoxOverlay =  OverlayWidget.Cast(rootWidget.FindAnyWidget("ListBoxHelmets"));
		m_ListBoxComponent = SCR_ListBoxComponent.Cast(m_ListBoxOverlay.FindHandler(SCR_ListBoxComponent));
		
		
        if (m_ListBoxComponent)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_HELMET)
				{
					string price = o.GetData();
					int i_price = e.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponent.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }


		m_ListBoxOverlay1 =  OverlayWidget.Cast(rootWidget.FindAnyWidget("ListBoxVests"));
		 m_ListBoxComponent1 = SCR_ListBoxComponent.Cast(m_ListBoxOverlay1.FindHandler(SCR_ListBoxComponent));
        if (m_ListBoxComponent1)
        {
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_VEST)
				{
					string price = o.GetData();
					int i_price = e.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponent1.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           

        }


		m_ListBoxOverlay2 =  OverlayWidget.Cast(rootWidget.FindAnyWidget("ListBoxBackpacks"));
		m_ListBoxComponent2 = SCR_ListBoxComponent.Cast(m_ListBoxOverlay2.FindHandler(SCR_ListBoxComponent));
		
		 if (m_ListBoxComponent2)
        {
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_BACKPACK)
				{
					string price = o.GetData();
					int i_price = e.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponent2.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           

        }
		
		m_ListBoxOverlay3 =  OverlayWidget.Cast(rootWidget.FindAnyWidget("ListBoxJackets"));
		m_ListBoxComponent3 = SCR_ListBoxComponent.Cast(m_ListBoxOverlay3.FindHandler(SCR_ListBoxComponent));
		
		 if (m_ListBoxComponent3)
        {
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_JACKET)
				{
					string price = o.GetData();
					int i_price = e.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponent3.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           

        }
		
		m_ListBoxOverlay4 =  OverlayWidget.Cast(rootWidget.FindAnyWidget("ListBoxPants"));
		m_ListBoxComponent4 = SCR_ListBoxComponent.Cast(m_ListBoxOverlay4.FindHandler(SCR_ListBoxComponent));
		
		 if (m_ListBoxComponent4)
        {
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_PANTS)
				{
					string price = o.GetData();
					int i_price = e.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponent4.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           

        }

		//OnSelectInvoker
		m_ListBoxComponent.m_OnChanged.Insert(OnSelected);
		m_ListBoxComponent1.m_OnChanged.Insert(OnSelected);
		m_ListBoxComponent2.m_OnChanged.Insert(OnSelected);
		m_ListBoxComponent3.m_OnChanged.Insert(OnSelected);
		m_ListBoxComponent4.m_OnChanged.Insert(OnSelected);


		SCR_ButtonTextComponent buttonClose = SCR_ButtonTextComponent.GetButtonText(BUTTON_CLOSE, panelRoot);
		if (buttonClose)
			buttonClose.m_OnClicked.Insert(Close);
		else
			Print("Button Close not found - won't be able to exit by button", LogLevel.WARNING);

		/*
			buy  button
		*/

		SCR_ButtonTextComponent buttonChange = SCR_ButtonTextComponent.GetButtonText(BUTTON_BUY, panelRoot);
		if (buttonChange)
			buttonChange.m_OnClicked.Insert(Buy);
		else
			Print("Button Change not found", LogLevel.WARNING); // the button can be missing without putting the layout in jeopardy

		/*
			ESC/Start listener
		*/

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
		else if (!buttonClose)
		{
			Print("Auto-closing the menu that has no exit path", LogLevel.WARNING);
			Close();
			return;
		}
	}
	protected override void OnMenuInit()
	{
		d = new WST_Equipment();
		c = new WST_WeaponV2();
		e = WST_ShopLoadOutBase.AddLoadOuts(d,c);
		setupDataObjects();
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

};