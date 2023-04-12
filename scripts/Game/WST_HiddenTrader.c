
///////////////////////////////////////////////////////////////////////------------------PreviewSetup
class WST_Equipment
{
	//"suppressed" variants are capable of fitting a suppressor and not actually suppressed
	
	/////////////////////////////////////////////////////////////////////////
	//prefab item paths
	
	//weapon
	ref array<ShopObject> shopItems = new array<ShopObject>();
	
	ref ShopObject LSzH = new ShopObject("{AC1C630517753E2B}Prefabs/Characters/HeadGear/Helmet_LShZ/Helmet_LShZ_6m2_black.et","LSzH","LSzH Black",WST_Type.WST_HELMET,300);
	ref ShopObject LSzH3M = new ShopObject("{D585D22578362515}Prefabs/Characters/HeadGear/Helmet_LShZ/Helmet_LShZ_HC_3M_black.et","LSzH3M","LSzH Black HC 3M",WST_Type.WST_HELMET,300);
	
	ref ShopObject RatnikOffcVest = new ShopObject("{915E9D97CFAB02D6}Prefabs/Characters/Vests/Vest_Ratnik_6B45/Variants/Vest_Ratnik_6B45_officer.et","RatnikOfficerVest","Ratnic Vest Offc.",WST_Type.WST_VEST,300);
	ref ShopObject Ratnik6b45Vest = new ShopObject("{B9821AF2F5D104E4}Prefabs/Characters/Vests/Vest_Ratnik_6B45/Variants/Vest_Ratnik_6B45.et","Ratnik6b45","Ratnik 6b45",WST_Type.WST_VEST,300);

	ref ShopObject FrogCombatShirt = new ShopObject("{1CF302A33F29FBEF}Prefabs/Characters/Uniforms/Jacket_FROG_Combat_Shirt.et","FrogC","Frog Combat Shirt",WST_Type.WST_JACKET,300);
	ref ShopObject M88VSR = new ShopObject("{D7402E3D0399FF54}Prefabs/Characters/Uniforms/Jacket_M88_VSR.et","M88VSR","M88 Jacker VSR",WST_Type.WST_JACKET,300);
	
	ref ShopObject Suharka = new ShopObject("{CAEDE923EF4071AE}Prefabs/Items/Equipment/Backpacks/Backpack_Suharka_type1.et","Suharka","Suharka Backpack",WST_Type.WST_BACKPACK,300);

	ref ShopObject Pants = new ShopObject("{738798231DAF3078}Prefabs/Characters/Uniforms/Pants_6b51_VKPO.et","6b51Pants","6b51 Pants",WST_Type.WST_PANTS,300);


	
	ref map<string,ResourceName> buildArray = new map<string,ResourceName>();
	ref array<string> iterationArray = new array<string>();
	ref map<string,string> DisplayNameArray = new map<string,string>();
	ref map<string,int> TypeArray = new map<string,int>();
	ref map<string,int> PriceArray = new map<string,int>();

	private WST_Equipment instance;
	WST_Equipment GetInstance()
	{
		if(instance)
			return instance;
		else
			return new WST_Equipment();
	}
	
	int GetPriceByKey(string identifier)
	{
		return PriceArray.Get(identifier);
	}
	void WST_Equipment()
	{
		if (instance)
			return;
		instance = this;
		//
		
		
		
		
		
		
		
		
		
		
		//shopItems.Insert(AK74O);
		shopItems.Insert(LSzH);
		shopItems.Insert(LSzH3M);
		shopItems.Insert(RatnikOffcVest);		
		shopItems.Insert(Ratnik6b45Vest);
		shopItems.Insert(FrogCombatShirt);
		shopItems.Insert(Suharka);	
		shopItems.Insert(Pants);
		
	
		
		foreach(ShopObject o:shopItems)
		{
			buildArray.Insert(o.m_Identifier,o.m_prefabPath);
			iterationArray.Insert(o.m_Identifier);
			DisplayNameArray.Insert(o.m_Identifier,o.m_Display);
			TypeArray.Insert(o.m_Identifier,o.m_Type);
			PriceArray.Insert(o.m_Identifier,o.m_price);
		
		}
		
		
		
	}
	
}


//TODO:add proper hints
class WST_HiddenTrader : MenuBase
{
	protected static const string TEXT_TITLE = "TextTitle";
	protected static const string BUTTON_CLOSE = "QuitButton";
	protected static const string BUTTON_BUY = "BuyButton";
	SCR_TabViewComponent m_TabView;
	BlurWidget wBlur ;
	bool firstSelection = true;
	ref WST_Equipment e;

	ResourceName spawnEntity;
	WST_TraderComponent TrdComp;
	int balance;
	
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
			Print("WST_TransferWindow::Buy::No TraderComponent!! ");
			return;
		
		}
		
		traderComp.HandleBuyAction(spawnEntity,balance);
		
		
		

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

		Print("WST_TransferWindow::OnSelectedInvoker");
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

		            	Print("WST_TransferWindowUI::OnSelected::noWallet");
		            	return;
						
						
	        		}
					balance = wallet.GetValue();
				    balance = balance - e.GetPriceByKey(key);//w.GetWeaponPriceByKey(key);
				
				    if (balance < 0) 
					{
						spawnEntity = "";
			
				         Print("WST_TransferWindowUI::OnSelected::noBalance");
				         return;
				     }
					ResourceName n = e.buildArray.Get(key);
				    spawnEntity = n;
				}
			}
			
		
		
		}

	}



	//------------------------------------------------------------------------------------------------
	protected override void OnMenuOpen()
	{
		Print("OnMenuOpen: menu/dialog opened!", LogLevel.NORMAL);
		e = new WST_Equipment();
		setupDataObjects();
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
		SCR_ListBoxComponent m_ListBoxComponent1;
		OverlayWidget m_ListBoxOverlay1;
		SCR_ListBoxComponent m_ListBoxComponent2;
		OverlayWidget m_ListBoxOverlay2;
		SCR_ListBoxComponent m_ListBoxComponent3;
		OverlayWidget m_ListBoxOverlay3;
		SCR_ListBoxComponent m_ListBoxComponent4;
		OverlayWidget m_ListBoxOverlay4;
		
		
		
		
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

};