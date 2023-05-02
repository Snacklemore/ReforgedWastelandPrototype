



//TODO:add proper hints
class WST_TransferWindowUI : MenuBase
{
	protected static const string TEXT_TITLE = "TextTitle";
	protected static const string BUTTON_CLOSE = "ButtonClose";
	protected static const string BUTTON_BUY = "ButtonBuy";
	SCR_TabViewComponent m_TabView;
	BlurWidget wBlur ;
	bool firstSelection = true;
	ref WST_Weapon w1;
	ref WST_WeaponV2 w2;
	ref WST_Equipment e;	
	ref WST_ShopLoadOutBase w;
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


	SCR_TabViewComponent m_TabviewComponent;
	
	//weapon tab
	SCR_ListBoxComponent m_ListBoxComponentTabWeapon;
	SCR_ListBoxComponent m_ListBoxComponentTabWeapon1;	
	SCR_ListBoxComponent m_ListBoxComponentTabWeapon2;	
	RichTextWidget m_TextHeadingRifle;
	RichTextWidget m_TextHeadingPistol;
	RichTextWidget m_TextHeadingSniper;

	//equipment tab
	SCR_ListBoxComponent m_ListBoxComponentTabEquipment;
	SCR_ListBoxComponent m_ListBoxComponentTabEquipment1;	
	SCR_ListBoxComponent m_ListBoxComponentTabEquipment2;	
	RichTextWidget m_TextHeadingUniform;
	RichTextWidget m_TextHeadingHeadgear;
	RichTextWidget m_TextHeadingPants;
	
	
	//Attachments tab
	SCR_ListBoxComponent m_ListBoxComponentTabAttachments;
	SCR_ListBoxComponent m_ListBoxComponentTabAttachments1;	
	SCR_ListBoxComponent m_ListBoxComponentTabAttachments2;	
	RichTextWidget m_TextHeadingSuppressors;
	RichTextWidget m_TextHeadingOptics;
	RichTextWidget m_TextHeadingLaunchers;
	
	//Ammo Tab
	SCR_ListBoxComponent m_ListBoxComponentTabAmmo;
	SCR_ListBoxComponent m_ListBoxComponentTabAmmo1;
	SCR_ListBoxComponent m_ListBoxComponentTabAmmo2;
	RichTextWidget m_TextHeadingRifleAmmo;
	RichTextWidget m_TextHeadingPistolAmmo;
	RichTextWidget m_TextHeadingSniperAmmo;
	
	OverlayWidget m_ListBoxOverlayTabWeapon;
	OverlayWidget m_ListBoxOverlayTabWeapon1;
	OverlayWidget m_ListBoxOverlayTabWeapon2;

	OverlayWidget m_ListBoxOverlayTabAmmo;
	OverlayWidget m_ListBoxOverlayTabAmmo1;
	OverlayWidget m_ListBoxOverlayTabAmmo2;

	OverlayWidget m_ListBoxOverlayTabEquipment;
	OverlayWidget m_ListBoxOverlayTabEquipment1;
	OverlayWidget m_ListBoxOverlayTabEquipment2;
	
	OverlayWidget m_ListBoxOverlayTabAttachments;
	OverlayWidget m_ListBoxOverlayTabAttachments1;
	OverlayWidget m_ListBoxOverlayTabAttachments2;
	
	VerticalLayoutWidget m_TabViewOverlay;
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

		


		TextWidget textTitle = TextWidget.Cast(rootWidget.FindWidget(TEXT_TITLE));

		Widget panelRoot = rootWidget.FindWidget("Panel0");
		ItemPreviewWidget preview = panelRoot.FindWidget("WeaponPreview");

		
		
		
		//Weapon Ammo Equipment Attachments
		
		m_TabViewOverlay = VerticalLayoutWidget.Cast(panelRoot.FindAnyWidget("TabView"));
		m_TabviewComponent = SCR_TabViewComponent.Cast(m_TabViewOverlay.FindHandler(SCR_TabViewComponent));
		
		m_TabviewComponent.m_OnContentSelect.Insert(OnSelectTab);
		//content of first active tab
		SCR_TabViewContent contentWeapon =  m_TabviewComponent.GetShownTabComponent();
		Widget tabRootWeapon = contentWeapon.m_wTab;
		//seconde tab content
		SCR_TabViewContent contentAmmo =  m_TabviewComponent.GetEntryContent(1);
		Widget tabRootAmmo = contentAmmo.m_wTab;
		
		SCR_TabViewContent contentEquipment =  m_TabviewComponent.GetEntryContent(2);
		Widget tabRootEquipment = contentEquipment.m_wTab;
		
		SCR_TabViewContent contentAttachments =  m_TabviewComponent.GetEntryContent(3);
		Widget tabRootAttachments = contentAttachments.m_wTab;
		
		m_TextHeadingRifle = RichTextWidget.Cast(tabRootWeapon.FindAnyWidget("Heading1"));
		m_TextHeadingPistol = RichTextWidget.Cast(tabRootWeapon.FindAnyWidget("Heading2"));
		m_TextHeadingSniper = RichTextWidget.Cast(tabRootWeapon.FindAnyWidget("Heading3"));
		m_TextHeadingRifle.SetText("Rilfes");
		m_TextHeadingPistol.SetText("Pistols");
		m_TextHeadingSniper.SetText("DMRs");
		
		
		
		m_TextHeadingRifleAmmo = RichTextWidget.Cast(tabRootAmmo.FindAnyWidget("Heading1"));
		m_TextHeadingPistolAmmo = RichTextWidget.Cast(tabRootAmmo.FindAnyWidget("Heading2"));
		m_TextHeadingSniperAmmo = RichTextWidget.Cast(tabRootAmmo.FindAnyWidget("Heading3"));
		m_TextHeadingRifleAmmo.SetText("Rilfe Ammo");
		m_TextHeadingPistolAmmo.SetText("Pistol Ammo");
		m_TextHeadingSniperAmmo.SetText("DMR Ammo ");
		
		
		m_TextHeadingHeadgear = RichTextWidget.Cast(tabRootEquipment.FindAnyWidget("Heading1"));
		m_TextHeadingUniform = RichTextWidget.Cast(tabRootEquipment.FindAnyWidget("Heading2"));
		m_TextHeadingPants = RichTextWidget.Cast(tabRootEquipment.FindAnyWidget("Heading3"));
		m_TextHeadingHeadgear.SetText("Headgear");
		m_TextHeadingUniform.SetText("Uniforms");
		m_TextHeadingPants.SetText("Pants");
		
		
		
		m_TextHeadingSuppressors = RichTextWidget.Cast(tabRootAttachments.FindAnyWidget("Heading1"));
		m_TextHeadingOptics = RichTextWidget.Cast(tabRootAttachments.FindAnyWidget("Heading2"));
		m_TextHeadingLaunchers = RichTextWidget.Cast(tabRootAttachments.FindAnyWidget("Heading3"));
		m_TextHeadingSuppressors.SetText("Suppressors");
		m_TextHeadingOptics.SetText("Optics");
		m_TextHeadingLaunchers.SetText("Launchers & Special");
		
		
		
		
		//tab Attachments listbox
		m_ListBoxOverlayTabAttachments2 =  OverlayWidget.Cast(tabRootAttachments.FindAnyWidget("ListBox2"));
		m_ListBoxComponentTabAttachments2= SCR_ListBoxComponent.Cast(m_ListBoxOverlayTabAttachments2.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTabAttachments2.m_OnChanged.Insert(OnSelected);
		if (m_ListBoxComponentTabAttachments2)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_UBGL )
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTabAttachments2.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		
		
		//tab Attachments listbox
		m_ListBoxOverlayTabAttachments1 =  OverlayWidget.Cast(tabRootAttachments.FindAnyWidget("ListBox1"));
		m_ListBoxComponentTabAttachments1= SCR_ListBoxComponent.Cast(m_ListBoxOverlayTabAttachments1.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTabAttachments1.m_OnChanged.Insert(OnSelected);
		if (m_ListBoxComponentTabAttachments1)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_OPTIC )
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTabAttachments1.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		
		
		//tab Attachments listbox
		m_ListBoxOverlayTabAttachments =  OverlayWidget.Cast(tabRootAttachments.FindAnyWidget("ListBox0"));
		m_ListBoxComponentTabAttachments= SCR_ListBoxComponent.Cast(m_ListBoxOverlayTabAttachments.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTabAttachments.m_OnChanged.Insert(OnSelected);
		if (m_ListBoxComponentTabAttachments)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_SUPPRESSOR )
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTabAttachments.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		
		
		
		
		//tab Equipment listbox
		m_ListBoxOverlayTabEquipment2 =  OverlayWidget.Cast(tabRootEquipment.FindAnyWidget("ListBox2"));
		m_ListBoxComponentTabEquipment2= SCR_ListBoxComponent.Cast(m_ListBoxOverlayTabEquipment2.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTabEquipment2.m_OnChanged.Insert(OnSelected);
		if (m_ListBoxComponentTabEquipment2)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_PANTS )
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTabEquipment2.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		
		
		//tab Equipment listbox
		m_ListBoxOverlayTabEquipment1 =  OverlayWidget.Cast(tabRootEquipment.FindAnyWidget("ListBox1"));
		m_ListBoxComponentTabEquipment1= SCR_ListBoxComponent.Cast(m_ListBoxOverlayTabEquipment1.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTabEquipment1.m_OnChanged.Insert(OnSelected);
		if (m_ListBoxComponentTabEquipment1)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_JACKET )
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTabEquipment1.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		
		
		//tab Equipment listbox
		m_ListBoxOverlayTabEquipment =  OverlayWidget.Cast(tabRootEquipment.FindAnyWidget("ListBox0"));
		m_ListBoxComponentTabEquipment= SCR_ListBoxComponent.Cast(m_ListBoxOverlayTabEquipment.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTabEquipment.m_OnChanged.Insert(OnSelected);
		if (m_ListBoxComponentTabEquipment)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_HELMET )
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTabEquipment.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		
		
		//tab ammo listbox
		m_ListBoxOverlayTabAmmo =  OverlayWidget.Cast(tabRootAmmo.FindAnyWidget("ListBox0"));
		m_ListBoxComponentTabAmmo= SCR_ListBoxComponent.Cast(m_ListBoxOverlayTabAmmo.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTabAmmo.m_OnChanged.Insert(OnSelected);
		if (m_ListBoxComponentTabAmmo)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_RIFLEAMMO )
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTabAmmo.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		
		
		//tab ammo listbox
		m_ListBoxOverlayTabAmmo1 =  OverlayWidget.Cast(tabRootAmmo.FindAnyWidget("ListBox1"));
		m_ListBoxComponentTabAmmo1= SCR_ListBoxComponent.Cast(m_ListBoxOverlayTabAmmo1.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTabAmmo1.m_OnChanged.Insert(OnSelected);
		if (m_ListBoxComponentTabAmmo1)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_PISTOLAMMO )
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTabAmmo1.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		
		//tab ammo listbox
		m_ListBoxOverlayTabAmmo2 =  OverlayWidget.Cast(tabRootAmmo.FindAnyWidget("ListBox2"));
		m_ListBoxComponentTabAmmo2 = SCR_ListBoxComponent.Cast(m_ListBoxOverlayTabAmmo2.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTabAmmo2.m_OnChanged.Insert(OnSelected);
		if (m_ListBoxComponentTabAmmo2)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_SNIPERAMMO )
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTabAmmo2.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		//tab Weapon listbox
		m_ListBoxOverlayTabWeapon =  OverlayWidget.Cast(tabRootWeapon.FindAnyWidget("ListBox0"));
		m_ListBoxComponentTabWeapon= SCR_ListBoxComponent.Cast(m_ListBoxOverlayTabWeapon.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTabWeapon.m_OnChanged.Insert(OnSelected);
		if (m_ListBoxComponentTabWeapon)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_RIFLE)
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTabWeapon.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		
		
		m_ListBoxOverlayTabWeapon1 =  OverlayWidget.Cast(tabRootWeapon.FindAnyWidget("ListBox1"));
		m_ListBoxComponentTabWeapon1= SCR_ListBoxComponent.Cast(m_ListBoxOverlayTabWeapon1.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTabWeapon1.m_OnChanged.Insert(OnSelected);
		
		if (m_ListBoxComponentTabWeapon1)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_PISTOL )
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTabWeapon1.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



        }
		
		
		//more detailed categorising needed (eg pistol, rifle, etc)
		m_ListBoxOverlayTabWeapon2 =  OverlayWidget.Cast(tabRootWeapon.FindAnyWidget("ListBox2"));
		m_ListBoxComponentTabWeapon2= SCR_ListBoxComponent.Cast(m_ListBoxOverlayTabWeapon2.FindHandler(SCR_ListBoxComponent));
		m_ListBoxComponentTabWeapon2.m_OnChanged.Insert(OnSelected);
		
		if (m_ListBoxComponentTabWeapon2)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_SNIPERRIFLE )
				{
					string price = o.GetData();
					int i_price = w.GetPriceByKey(price);
					price = i_price.ToString();
					m_ListBoxComponentTabWeapon2.AddItem(o.GetDisplayText() +"  $$: "+price ,o);
				}
				
			}
           



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
	
	void OnSelectTab()
	{
		
	}
	
	
	
	
	
	protected override void OnMenuInit()
	{
		w1 = new WST_Weapon();
		w2 = new WST_WeaponV2();
		e = new WST_Equipment();
		ref WST_ShopLoadOutBase w_h = WST_ShopLoadOutBase.AddLoadOuts(w1,w2);
		w = WST_ShopLoadOutBase.AddLoadOuts(w_h,e);
		setupDataObjects();
	}
	
	
	
	
	

	//------------------------------------------------------------------------------------------------

};