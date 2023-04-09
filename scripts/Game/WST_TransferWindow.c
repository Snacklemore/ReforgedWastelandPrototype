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
	static ResourceName AK74 = "{5B308198855FBCE5}Prefabs/Weapons/Rifles/AK74/Rifle_AK74Suppressed.et";
	static ResourceName AK74_GP25 = "{2FA2E44E5B95D044}Prefabs/Weapons/Rifles/AK74/Rifle_AK74_GP25Suppressed.et";
	static ResourceName M21_ARTII = "{81EB948E6414BD6F}Prefabs/Weapons/Rifles/M14/Rifle_M21_ARTII.et";
	static ResourceName M16A2_M203 = "{7500FA1B52CC247F}Prefabs/Weapons/Rifles/M16/Rifle_M16A2_M203Suppressed.et";
	//attachements
	static ResourceName StandardSuppressor = "{7CB432CC4ED704E1}Prefabs/Weapons/Core/Suppressor_Generic_556.et";
	static ResourceName NT4Suppressor = "{904AF79F5926399A}Prefabs/Weapons/Core/Suppressor_Punky_NT4.et";
	//Grenades 
	static ResourceName GP25 = "{262F0D09C4130826}Prefabs/Weapons/Ammo/Ammo_Grenade_HE_VOG25.et";
	static ResourceName M203 = "{5375FA7CB1F68573}Prefabs/Weapons/Ammo/Ammo_Grenade_HE_M406.et";
	static ResourceName USFrag = "{E8F00BF730225B00}Prefabs/Weapons/Grenades/Grenade_M67.et";
	static ResourceName USSRFrag = "{645C73791ECA1698}Prefabs/Weapons/Grenades/Grenade_RGD5.et";
	static ResourceName Smoke = "{9DB69176CEF0EE97}Prefabs/Weapons/Grenades/Smoke_ANM8HC.et";
	//ammo 
	static ResourceName AK74Mag = "{E5912E45754CD421}Prefabs/Weapons/Magazines/Magazine_545x39_AK_30rnd_Tracer.et";
	static ResourceName M9Mag = "{9C05543A503DB80E}Prefabs/Weapons/Magazines/Magazine_9x19_M9_15rnd_Ball.et";
	static ResourceName M16Mag = "{A9A385FE1F7BF4BD}Prefabs/Weapons/Magazines/Magazine_556x45_STANAG_30rnd_Tracer.et";
	static ResourceName M14Mag = "{627255315038152A}Prefabs/Weapons/Magazines/Magazine_762x51_M14_20rnd_SpecialBall.et";
	
	//unique itemidentfier
	static string AK74identifier = "WeaponAK74";
	static string AK74_GP25identifier = "WeaponAKGP25";
	static string M21_ARTIIidentifier = "WeaponM21ARTII";
	static string M16A2_M203identifier = "WeaponM16A2M203";
	static string StandardSuppressorIdentifier ="StandardSuppressor";
	static string NT4_SuppressorIdentifier = "NT4Suppressor";
	
	//ammo
	static string AK74MagIdentifier ="AK74Mag";
	static string M9MagIdentifier ="M9Mag";
	static string M16MagIdentifier ="M16Mag";
	static string M14MagIdentifier="M14Mag";
	//Grenades 
	static string GP25Identifier ="GP25";
	static string M203Identifier ="M203";
	static string USFragIdentifier ="USFrag";
	static string USSRFragIdentifier="USSRFrag";
	static string SmokeIdentifier="Smoke";

	
	
	
	
	//display name 
	static string AK74Display = "AK 74";
	static string AK74_GP25Display = "AK GP25";
	static string M21_ARTIIDisplay = "M21 ARTII Optic";
	static string M16A2_M203Display = "M16A2 M203UBGL";
	static string StandardSuppressorDisplay = "Standard Suppressor";
	static string NT4SuppressorDisplay = "NT4 Suppressor";
	//ammo
	static string AK74MagDisplay= "AK 74 Magazine";
	static string M9MagDisplay= "M9 Pistol Magazine";
	static string M16MagDisplay= "M16 Magazine";
	static string M14MagDisplay	= "M14 Magazine";
	//Grenades 
	
	static string GP25Display="AKGP VOG25";
	static string M203Display="M203 HE (for M16GL)";
	static string USFragDisplay="US Grenade";
	static string USSRFragDisplay="RGD Grenade";
	static string SmokeDisplay="Smoke";

	
	
	
	
	
	//item price
	static int AK74Price = 50;
	static int AK74_GP25Price = 100;
	static int M21_ARTIIPrice = 100;
	static int M16A2_M203Price = 50;
	static int StandardSuppressorPrice = 200;
	static int NT4SuppressorPrice = 200;
	static int AK74MagPrice= 20;
	static int M9MagPrice= 20;
	static int M16MagPrice= 20;
	static int M14MagPrice= 20;
		//Grenades 
	static int GP25Price= 20;
	static int M203Price= 20;
	static int  USFragPrice= 20;
	static int USSRFragPrice= 20;
	static int SmokePrice= 20;
	
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
		
		
		buildArray.Insert(AK74identifier,AK74);
		buildArray.Insert(AK74_GP25identifier,AK74_GP25);
		buildArray.Insert(M21_ARTIIidentifier,M21_ARTII);
		buildArray.Insert(M16A2_M203identifier,M16A2_M203);
		buildArray.Insert(StandardSuppressorIdentifier,StandardSuppressor);
		buildArray.Insert(NT4_SuppressorIdentifier,NT4Suppressor);
		//Ammo
		
		buildArray.Insert(AK74MagIdentifier,AK74Mag);
		buildArray.Insert(M9MagIdentifier,M9Mag);
		buildArray.Insert(M16MagIdentifier,M16Mag);
		buildArray.Insert(M14MagIdentifier,M14Mag);
		
		
			//Grenades 
		buildArray.Insert(GP25Identifier,GP25);
		buildArray.Insert(M203Identifier,M203);
		buildArray.Insert(USFragIdentifier,USFrag);
		buildArray.Insert(USSRFragIdentifier,USSRFrag);
		buildArray.Insert(SmokeIdentifier,Smoke);
		
		


		iterationArray.Insert(AK74identifier);
		iterationArray.Insert(AK74_GP25identifier);
		iterationArray.Insert(M21_ARTIIidentifier);
		iterationArray.Insert(M16A2_M203identifier);
		iterationArray.Insert(StandardSuppressorIdentifier);
		iterationArray.Insert(NT4_SuppressorIdentifier);
		
		//Ammo
		iterationArray.Insert(AK74MagIdentifier);
		iterationArray.Insert(M9MagIdentifier);
		iterationArray.Insert(M16MagIdentifier);
		iterationArray.Insert(M14MagIdentifier);
		
		//Grenades 
		iterationArray.Insert(GP25Identifier);
		iterationArray.Insert(M203Identifier);
		iterationArray.Insert(USFragIdentifier);
		iterationArray.Insert(USSRFragIdentifier);
		iterationArray.Insert(SmokeIdentifier);

		DisplayNameArray.Insert(AK74identifier,AK74Display);
		DisplayNameArray.Insert(AK74_GP25identifier,AK74_GP25Display);
		DisplayNameArray.Insert(M21_ARTIIidentifier,M21_ARTIIDisplay);
		DisplayNameArray.Insert(M16A2_M203identifier,M16A2_M203Display);
		DisplayNameArray.Insert(StandardSuppressorIdentifier,StandardSuppressorDisplay);
		DisplayNameArray.Insert(NT4_SuppressorIdentifier,NT4SuppressorDisplay);
		
		//Ammo
		DisplayNameArray.Insert(AK74MagIdentifier,AK74MagDisplay);
		DisplayNameArray.Insert(M9MagIdentifier,M9MagDisplay);
		DisplayNameArray.Insert(M16MagIdentifier,M16MagDisplay);
		DisplayNameArray.Insert(M14MagIdentifier,M14MagDisplay);
			//Grenades 

		DisplayNameArray.Insert(GP25Identifier,GP25Display);
		DisplayNameArray.Insert(M203Identifier,M203Display);
		DisplayNameArray.Insert(USFragIdentifier,USFragDisplay);
		DisplayNameArray.Insert(USSRFragIdentifier,USSRFragDisplay);
		DisplayNameArray.Insert(SmokeIdentifier,SmokeDisplay);

		
		
		TypeArray.Insert(AK74identifier,WST_Type.WST_WEAPON);
		TypeArray.Insert(AK74_GP25identifier,WST_Type.WST_WEAPON);
		TypeArray.Insert(M21_ARTIIidentifier,WST_Type.WST_WEAPON);
		TypeArray.Insert(M16A2_M203identifier,WST_Type.WST_WEAPON);
		TypeArray.Insert(StandardSuppressorIdentifier,WST_Type.WST_ATTACHMENT);
		TypeArray.Insert(NT4_SuppressorIdentifier,WST_Type.WST_ATTACHMENT);
		
		//Ammo
		TypeArray.Insert(AK74MagIdentifier,WST_Type.WST_AMMO);
		TypeArray.Insert(M9MagIdentifier,WST_Type.WST_AMMO);
		TypeArray.Insert(M16MagIdentifier,WST_Type.WST_AMMO);
		TypeArray.Insert(M14MagIdentifier,WST_Type.WST_AMMO);
			//Grenades 
		TypeArray.Insert(GP25Identifier,WST_Type.WST_GRENADES);
		TypeArray.Insert(M203Identifier,WST_Type.WST_GRENADES);
		TypeArray.Insert(USFragIdentifier,WST_Type.WST_GRENADES);
		TypeArray.Insert(USSRFragIdentifier,WST_Type.WST_GRENADES);
		TypeArray.Insert(SmokeIdentifier,WST_Type.WST_GRENADES);

		
		
		PriceArray.Insert(AK74identifier,50);
		PriceArray.Insert(AK74_GP25identifier,200);
		PriceArray.Insert(M21_ARTIIidentifier,1000);
		PriceArray.Insert(M16A2_M203identifier,300);
		PriceArray.Insert(StandardSuppressorIdentifier,50);
		PriceArray.Insert(NT4_SuppressorIdentifier,50);
		//Ammo
		
		PriceArray.Insert(AK74MagIdentifier,AK74MagPrice);
		PriceArray.Insert(M9MagIdentifier,AK74MagPrice);
		PriceArray.Insert(M16MagIdentifier,AK74MagPrice);
		PriceArray.Insert(M14MagIdentifier,AK74MagPrice);
	
			//Grenades 
		PriceArray.Insert(GP25Identifier,AK74MagPrice);
		PriceArray.Insert(M203Identifier,AK74MagPrice);
		PriceArray.Insert(USFragIdentifier,AK74MagPrice);
		PriceArray.Insert(USSRFragIdentifier,AK74MagPrice);
		PriceArray.Insert(SmokeIdentifier,AK74MagPrice);

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