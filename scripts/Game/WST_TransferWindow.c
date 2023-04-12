class ManagedDataObject : Managed
{
	string TestStringData;
	WST_Type Type;
	string DisplayText;
	
	void SetData(string data)
	{
		TestStringData = data;
	}
	
	string GetData()
	{
		return TestStringData;
	}
	
	string GetDisplayText()
	{
		return DisplayText;
	}
	
	WST_Type GetType()
	{	
		return Type;
	}
	
	void SetDisplayText(string txt)
	{
		DisplayText = txt;
	}
	void SetType(WST_Type s )
	{
		Type = s;
	}

};

enum WST_Type
{
	WST_WEAPON,
	WST_ATTACHMENT,
	WST_OPTIC,
	WST_EQUIPMENT,
	WST_VEST,
	WST_JACKET,
	WST_PANTS,
	WST_HELMET,
	WST_BACKPACK,
	WST_AMMO,
	WST_GRENADES,
	WST_EXPLOSIVES,
	WST_VEHICLES,
	WST_CONSUMABLE,
	WST_MEDIC,
	WST_VEHICLE
};
///////////////////////////////////////////////////////////////////////------------------PreviewSetup
class WST_Weapon
{
	//"suppressed" variants are capable of fitting a suppressor and not actually suppressed
	
	/////////////////////////////////////////////////////////////////////////
	//prefab item paths
	
	//weapon
	ref array<ShopObject> shopItems = new array<ShopObject>();
	
	
	ref ShopObject AN94O = new ShopObject("{AF2B2A5270708E19}Prefabs/Weapons/Rifles/AN94/Rifle_AN94_1P63.et","WeaponAN94","AN 74 with 1P63",WST_Type.WST_WEAPON,300);

	ref ShopObject AK74O = new ShopObject("{5B308198855FBCE5}Prefabs/Weapons/Rifles/AK74/Rifle_AK74Suppressed.et","WeaponAK74","AK 74",WST_Type.WST_WEAPON,200);
	
	ref ShopObject AK74GPO = new ShopObject("{2FA2E44E5B95D044}Prefabs/Weapons/Rifles/AK74/Rifle_AK74_GP25Suppressed.et","WeaponAKGP25","AK GP25",WST_Type.WST_WEAPON,300);
	
	ref ShopObject Rifle_M21_ARTIIO = new ShopObject("{81EB948E6414BD6F}Prefabs/Weapons/Rifles/M14/Rifle_M21_ARTII.et","WeaponM21ARTII","M21 ARTII Optic",WST_Type.WST_WEAPON,650);
	
	ref ShopObject M16O = new ShopObject("{7500FA1B52CC247F}Prefabs/Weapons/Rifles/M16/Rifle_M16A2_M203Suppressed.et","WeaponM16A2M203","M16A2 M203UBGL",WST_Type.WST_WEAPON,350);
	
	ref ShopObject StandardSuppressorO = new ShopObject("{7CB432CC4ED704E1}Prefabs/Weapons/Core/Suppressor_Generic_556.et","StandardSuppressor","Standard Suppressor",WST_Type.WST_ATTACHMENT,100);
	
	ref ShopObject NT4SuppressorO = new ShopObject("{904AF79F5926399A}Prefabs/Weapons/Core/Suppressor_Punky_NT4.et","NT4Suppressor","NT4 Suppressor",WST_Type.WST_ATTACHMENT,100);
	
	ref ShopObject GP25O = new ShopObject("{262F0D09C4130826}Prefabs/Weapons/Ammo/Ammo_Grenade_HE_VOG25.et","GP25","AKGP VOG25",WST_Type.WST_GRENADES,50);
	
	ref ShopObject M203O = new ShopObject("{5375FA7CB1F68573}Prefabs/Weapons/Ammo/Ammo_Grenade_HE_M406.et","M203","M203 HE (for M16GL)",WST_Type.WST_GRENADES,50);
	
	ref ShopObject USFragO = new ShopObject("{E8F00BF730225B00}Prefabs/Weapons/Grenades/Grenade_M67.et","USFrag","US Grenade",WST_Type.WST_GRENADES,25);
	
	ref ShopObject USSRFragO = new ShopObject("{645C73791ECA1698}Prefabs/Weapons/Grenades/Grenade_RGD5.et","USSRFrag","RGD Grenade",WST_Type.WST_GRENADES,25);
	
	ref ShopObject SmokeO = new ShopObject("{9DB69176CEF0EE97}Prefabs/Weapons/Grenades/Smoke_ANM8HC.et","Smoke","Smoke",WST_Type.WST_GRENADES,25);
	
	ref ShopObject AK74MagO = new ShopObject("{E5912E45754CD421}Prefabs/Weapons/Magazines/Magazine_545x39_AK_30rnd_Tracer.et","AK74Mag","AK 74 Magazine",WST_Type.WST_AMMO,10);
	
	ref ShopObject M9MagO = new ShopObject("{9C05543A503DB80E}Prefabs/Weapons/Magazines/Magazine_9x19_M9_15rnd_Ball.et","M9Mag","M9 Pistol Magazine",WST_Type.WST_AMMO,10);
	
	ref ShopObject M16MagO = new ShopObject("{A9A385FE1F7BF4BD}Prefabs/Weapons/Magazines/Magazine_556x45_STANAG_30rnd_Tracer.et","M16Mag","M16 Magazine",WST_Type.WST_AMMO,10);
	
	ref ShopObject M14MagO = new ShopObject("{627255315038152A}Prefabs/Weapons/Magazines/Magazine_762x51_M14_20rnd_SpecialBall.et","M14Mag","M14 Magazine",WST_Type.WST_AMMO,100);
	
	
	ref map<string,ResourceName> buildArray = new map<string,ResourceName>();
	ref array<string> iterationArray = new array<string>();
	ref map<string,string> DisplayNameArray = new map<string,string>();
	ref map<string,int> TypeArray = new map<string,int>();
	ref map<string,int> PriceArray = new map<string,int>();

	private WST_Weapon instance;
	WST_Weapon GetInstance()
	{
		if(instance)
			return instance;
		else
			return new WST_Weapon();
	}
	
	int GetPriceByKey(string identifier)
	{
		return PriceArray.Get(identifier);
	}
	void WST_Weapon()
	{
		if (instance)
			return;
		instance = this;
		//
		shopItems.Insert(AK74O);
		shopItems.Insert(AK74GPO);
		shopItems.Insert(Rifle_M21_ARTIIO);
		shopItems.Insert(M16O);
		shopItems.Insert(StandardSuppressorO);
		
		shopItems.Insert(NT4SuppressorO);
		shopItems.Insert(GP25O);
		shopItems.Insert(M203O);
		shopItems.Insert(USFragO);
		shopItems.Insert(USSRFragO);
		
		shopItems.Insert(SmokeO);
		shopItems.Insert(AK74MagO);
		shopItems.Insert(M9MagO);
		shopItems.Insert(M16MagO);
		shopItems.Insert(M14MagO);
		shopItems.Insert(AN94O);
		
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

		            	Print("WST_TransferWindowUI::OnSelected::noWallet");
		            	return;
						
						
	        		}
					balance = wallet.GetValue();
				    balance = balance - w.GetPriceByKey(key);//w.GetWeaponPriceByKey(key);
				
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