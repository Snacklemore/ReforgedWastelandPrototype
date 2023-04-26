

//TODO:add proper hints
class WST_SupplyTraderWindow : MenuBase
{
	protected static const string TEXT_TITLE = "TextTitle";
	protected static const string BUTTON_CLOSE = "QuitButton";
	protected static const string BUTTON_BUY = "BuyButton";
	
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

		//traderComp.HandleBuyAction(spawnEntity,balance);
		
		
		
		

	}
	///////////////////////////////////////////////////////////////////////------------------PreviewSetup
	
	ref	array<SCR_CampaignSuppliesComponent> m_supplies = {}; 
	private bool QueryEntities(IEntity e)
	{
		m_supplies.Clear();
		Vehicle supplyTruck = Vehicle.Cast(e);
		if(!Vehicle)
			return true;
		 SCR_CampaignSuppliesComponent supplies = SCR_CampaignSuppliesComponent.GetSuppliesComponent(e);
		if(supplies)
		{
			m_supplies.Insert(supplies);	
			return false;
		}
		return true;
	}

	void Confirmed()
	{
		PlayerController pc = GetGame().GetPlayerController();
		IEntity ie = pc.GetControlledEntity();
		InventoryStorageManagerComponent storageUser = InventoryStorageManagerComponent.Cast(ie.FindComponent(InventoryStorageManagerComponent));
	
		array<typename> componentsUser = {};
		componentsUser.Insert(MoneyComponent);
		componentsUser.Insert(RplComponent);
		IEntity walletEnityUser = storageUser.FindItemWithComponents(componentsUser, EStoragePurpose.PURPOSE_DEPOSIT);
			
		
		
		Widget rootWidget = GetRootWidget();
		if (!rootWidget)
					{
							Print("Error in Layout Tutorial layout creation", LogLevel.ERROR);
							return;
					}
						
		Widget panelRoot = rootWidget.FindWidget("Panel0");
						
		SCR_EditBoxComponent EditBox = SCR_EditBoxComponent.GetEditBoxComponent("EditBoxRoot0", panelRoot);
				
		int supplyamount = EditBox.GetValue().ToInt();
		Print("OnMenuOpen::SupplyTraderWindow Confirmed!!", LogLevel.NORMAL);
		Print("OnMenuOpen::SupplyTraderWindow amount bought " + supplyamount);
		
		//scan for vehicles in a 5m radius around the player
		
		GetGame().GetWorld().QueryEntitiesBySphere(ie.GetOrigin(),15.0,QueryEntities);
		if (m_supplies.Count() > 1)
		{
			Print("more than one truck found", LogLevel.WARNING);
			return;
		}
		
				
		if (m_supplies.Count() == 0)
		{
			Print("no truck found", LogLevel.WARNING);
			SCR_HintManagerComponent.ShowCustomHint("No Truck found!",5.0,false,EFieldManualEntryId.NONE,false);

			return;
		}
		
		RplId suplliesId = Replication.FindId(m_supplies[0]);
		WST_TraderComponent traderComponent = WST_TraderComponent.Cast(ie.FindComponent(WST_TraderComponent));
		if(!traderComponent)
			return;
		
		traderComponent.LoadBoughtSupplies(suplliesId,supplyamount,2);
		Close();

	
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
		Widget overlayWidget = panelRoot.FindWidget("Overlay0");
		
		SCR_EditBoxComponent EditBox = SCR_EditBoxComponent.GetEditBoxComponent("EditBoxRoot0", overlayWidget);
		
		
		

		


		EditBox.m_OnConfirm.Insert(Confirmed);


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
		
	}
	protected override void OnMenuInit()
	{
		
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