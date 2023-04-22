



//TODO:add proper hints
class WST_TransferWindowUI : MenuBase
{
	protected static const string TEXT_TITLE = "TextTitle";
	protected static const string BUTTON_CLOSE = "ButtonClose";
	protected static const string BUTTON_BUY = "ButtonBuy";
	SCR_TabViewComponent m_TabView;
	BlurWidget wBlur ;
	bool firstSelection = true;
	ref WST_Weapon w;

	ResourceName spawnEntity;
	WST_TraderComponent TrdComp;
	int balance;
	int itemPrice;
	int pSelectedIndex = 0;
	SCR_ListBoxComponent currenListBox ;
	//rate limit?
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
		
		for (int i = 0;i < w.iterationArray.Count();i++)
		{
		
			string key = w.iterationArray.Get(i);
			ManagedDataObject o = new ManagedDataObject();
			o.SetData(key);
			o.SetType(w.TypeArray.Get(key));
			o.SetDisplayText(w.DisplayNameArray.Get(key));
			dataObjects.Insert(o);
		}
	}

	void OnSelected(SCR_ListBoxComponent list , int itemIndex, bool isTheNewSelection)
	{
		///////////////////////////////////////////////////////////////////////------------------PreviewSetup

		Print("WST_TransferWindow::OnSelectedInvoker");
		Widget rootWidget = GetRootWidget();
		pSelectedIndex = itemIndex;
		currenListBox = list;
		ManagedDataObject data =  list.GetItemData(itemIndex);
		Widget panelRoot = rootWidget.FindWidget("Panel0");
		ItemPreviewWidget preview = panelRoot.FindWidget("WeaponPreview");
		string weapon
		if (data)
			weapon = data.GetData();
		
		foreach (ManagedDataObject dataObject : dataObjects)
		{
			if (dataObject.GetData() == weapon)
			{
				
				
				//"weapon" is the key for the hashmap
				
				ResourceName WeaponEntityPrefab = w.buildArray.Get(weapon);
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
	
			
			
			
			foreach (string key : w.iterationArray)
			{
				if (key == weapon)
				{
					if (!wallet) 
					{
						spawnEntity ="";
						balance = -1;
		            	Print("WST_TransferWindowUI::OnSelected::noWallet");
		            	return;
						
						
	        		}
					balance = wallet.GetValue();
				    balance = balance - w.GetPriceByKey(key);//w.GetWeaponPriceByKey(key);
					itemPrice = w.GetPriceByKey(key);
				    if (balance < 0) 
					{
						spawnEntity = "";
			
				         Print("WST_TransferWindowUI::OnSelected::noBalance");
				         return;
				     }
					ResourceName n = w.buildArray.Get(key);
				    spawnEntity = n;
				}
			}
			
		
		
		}

	}



	//------------------------------------------------------------------------------------------------
	protected override void OnMenuOpen()
	{
		Print("OnMenuOpen: menu/dialog opened!", LogLevel.NORMAL);
		w = new WST_Weapon();
		setupDataObjects();
		Widget rootWidget = GetRootWidget();
		if (!rootWidget)
		{
			Print("Error in Layout Tutorial layout creation", LogLevel.ERROR);
			return;
		}

		/*
			Close button
		*/


		TextWidget textTitle = TextWidget.Cast(rootWidget.FindWidget(TEXT_TITLE));

		Widget panelRoot = rootWidget.FindWidget("Panel0");
		TextWidget textTitleAttachments = TextWidget.Cast(panelRoot.FindWidget("TextAttachments"));
		TextWidget textTitleWeapons = TextWidget.Cast(panelRoot.FindWidget("TextWeapons"));
		textTitleAttachments.SetText("Attachments");
		textTitleWeapons.SetText("Weapons");


		if (!textTitle)
		{
			Print("Title as TextWidget could not be found", LogLevel.WARNING);
			return;
		}
		textTitle.SetText("Trade");


		ItemPreviewWidget preview = panelRoot.FindWidget("WeaponPreview");


		SCR_ListBoxComponent m_ListBoxComponent;
		OverlayWidget m_ListBoxOverlay;

		SCR_ListBoxComponent m_ListBoxComponent1;
		OverlayWidget m_ListBoxOverlay1;

		
		SCR_ListBoxComponent m_ListBoxComponent2;
		OverlayWidget m_ListBoxOverlay2;

		SCR_ListBoxComponent m_ListBoxComponent3;
		OverlayWidget m_ListBoxOverlay3;
		
		
		m_ListBoxOverlay3 =  OverlayWidget.Cast(rootWidget.FindAnyWidget("ListBoxGrenades"));
		m_ListBoxComponent3 = SCR_ListBoxComponent.Cast(m_ListBoxOverlay3.FindHandler(SCR_ListBoxComponent));
        if (m_ListBoxComponent3)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_GRENADES)
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponent3.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		
		m_ListBoxOverlay2 =  OverlayWidget.Cast(rootWidget.FindAnyWidget("ListBoxAmmo"));
		 m_ListBoxComponent2 = SCR_ListBoxComponent.Cast(m_ListBoxOverlay2.FindHandler(SCR_ListBoxComponent));
        if (m_ListBoxComponent2)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_AMMO)
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponent2.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }


		m_ListBoxOverlay =  OverlayWidget.Cast(rootWidget.FindAnyWidget("ListBoxWeapons"));
		 m_ListBoxComponent = SCR_ListBoxComponent.Cast(m_ListBoxOverlay.FindHandler(SCR_ListBoxComponent));
        if (m_ListBoxComponent)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_WEAPON)
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponent.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }


		m_ListBoxOverlay1 =  OverlayWidget.Cast(rootWidget.FindAnyWidget("ListBoxAttachments"));
		 m_ListBoxComponent1 = SCR_ListBoxComponent.Cast(m_ListBoxOverlay1.FindHandler(SCR_ListBoxComponent));
        if (m_ListBoxComponent1)
        {
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_ATTACHMENT)
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponent1.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           

        }




		//OnSelectInvoker
		m_ListBoxComponent.m_OnChanged.Insert(OnSelected);
		m_ListBoxComponent1.m_OnChanged.Insert(OnSelected);
		m_ListBoxComponent2.m_OnChanged.Insert(OnSelected);
		m_ListBoxComponent3.m_OnChanged.Insert(OnSelected);


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