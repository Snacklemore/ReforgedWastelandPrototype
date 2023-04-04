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
	WST_MEDIC
};
///////////////////////////////////////////////////////////////////////------------------PreviewSetup
class WST_Weapon
{
	//"suppressed" variants are capable of fitting a suppressor and not actually suppressed
	
	/////////////////////////////////////////////////////////////////////////
	//prefab item paths
	static ResourceName AK74 = "{5B308198855FBCE5}Prefabs/Weapons/Rifles/AK74/Rifle_AK74Suppressed.et";
	static ResourceName AK74_GP25 = "{63E8322E2ADD4AA7}Prefabs/Weapons/Rifles/AK74/Rifle_AK74_GP25.et";
	static ResourceName M21_ARTII = "{81EB948E6414BD6F}Prefabs/Weapons/Rifles/M14/Rifle_M21_ARTII.et";
	static ResourceName M16A2_M203 = "{7500FA1B52CC247F}Prefabs/Weapons/Rifles/M16/Rifle_M16A2_M203Suppressed.et";
	static ResourceName StandardSuppressor = "{7CB432CC4ED704E1}Prefabs/Weapons/Core/Suppressor_Generic_556.et";
	static ResourceName NT4Suppressor = "{904AF79F5926399A}Prefabs/Weapons/Core/Suppressor_Punky_NT4.et";

	//unique itemidentfier
	static string AK74identifier = "WeaponAK74";
	static string AK74_GP25identifier = "WeaponAKGP25";
	static string M21_ARTIIidentifier = "WeaponM21ARTII";
	static string M16A2_M203identifier = "WeaponM16A2M203";
	static string StandardSuppressorIdentifier ="StandardSuppressor";
	static string NT4_SuppressorIdentifier = "NT4Suppressor";
	//display name 
	static string AK74Display = "AK 74";
	static string AK74_GP25Display = "AK GP25";
	static string M21_ARTIIDisplay = "M21 ARTII Optic";
	static string M16A2_M203Display = "M16A2 M203UBGL";
	static string StandardSuppressorDisplay = "Standard Suppressor";
	static string NT4SuppressorDisplay = "NT4 Suppressor";
	//item price
	static int AK74Price = 50;
	static int AK74_GP25Price = 100;
	static int M21_ARTIIPrice = 100;
	static int M16A2_M203Price = 50;
	static int StandardSuppressorPrice = 200;
	static int NT4SuppressorPrice = 200;
	
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
		
		iterationArray.Insert(AK74identifier);
		iterationArray.Insert(AK74_GP25identifier);
		iterationArray.Insert(M21_ARTIIidentifier);
		iterationArray.Insert(M16A2_M203identifier);
		iterationArray.Insert(StandardSuppressorIdentifier);
		iterationArray.Insert(NT4_SuppressorIdentifier);
		
		DisplayNameArray.Insert(AK74identifier,AK74Display);
		DisplayNameArray.Insert(AK74_GP25identifier,AK74_GP25Display);
		DisplayNameArray.Insert(M21_ARTIIidentifier,M21_ARTIIDisplay);
		DisplayNameArray.Insert(M16A2_M203identifier,M16A2_M203Display);
		DisplayNameArray.Insert(StandardSuppressorIdentifier,StandardSuppressorDisplay);
		DisplayNameArray.Insert(NT4_SuppressorIdentifier,NT4SuppressorDisplay);
		
		TypeArray.Insert(AK74identifier,WST_Type.WST_WEAPON);
		TypeArray.Insert(AK74_GP25identifier,WST_Type.WST_WEAPON);
		TypeArray.Insert(M21_ARTIIidentifier,WST_Type.WST_WEAPON);
		TypeArray.Insert(M16A2_M203identifier,WST_Type.WST_WEAPON);
		TypeArray.Insert(StandardSuppressorIdentifier,WST_Type.WST_ATTACHMENT);
		TypeArray.Insert(NT4_SuppressorIdentifier,WST_Type.WST_ATTACHMENT);
		
		PriceArray.Insert(AK74identifier,50);
		PriceArray.Insert(AK74_GP25identifier,200);
		PriceArray.Insert(M21_ARTIIidentifier,1000);
		PriceArray.Insert(M16A2_M203identifier,300);
		PriceArray.Insert(StandardSuppressorIdentifier,50);
		PriceArray.Insert(NT4_SuppressorIdentifier,50);
		
	
	}
	
}



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

		
		}
		
		traderComp.HandleBuyAction(spawnEntity,balance);
		//traderComp.GetInsertionSuccess();
		//might fuse spawnWeapon and UpdatWalletValue so the wallet update can kick off from the traderComponent (no timing failure, when we query 
		// result before the end of operation)
		//traderComp.UpdateWalletValue(balance);
		
		

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




		m_ListBoxOverlay =  OverlayWidget.Cast(rootWidget.FindAnyWidget("ListBoxWeapons"));
		 m_ListBoxComponent = SCR_ListBoxComponent.Cast(m_ListBoxOverlay.FindHandler(SCR_ListBoxComponent));
        if (m_ListBoxComponent)
        {
			
			foreach (ManagedDataObject o : dataObjects)
			{
				if (o.GetType() == WST_Type.WST_WEAPON)
				{
					m_ListBoxComponent.AddItem(o.GetDisplayText(),o);
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
					m_ListBoxComponent1.AddItem(o.GetDisplayText(),o);
				}
				
			}
           

        }




		//OnSelectInvoker
		m_ListBoxComponent.m_OnChanged.Insert(OnSelected);
		m_ListBoxComponent1.m_OnChanged.Insert(OnSelected);



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