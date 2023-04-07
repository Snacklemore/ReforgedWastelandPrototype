class WST_TransferCurrencyWindow : MenuBase
{
	protected static const string TEXT_TITLE = "TextTitle";
	protected static const string BUTTON_CLOSE = "ButtonClose";
	protected static const string BUTTON_BUY = "ButtonBuy";
	IEntity mOwnerEntity;
	IEntity mUserEntity;
	
	
	void Confirmed()
	{
			PlayerController pc = GetGame().GetPlayerController();
			IEntity ie = pc.GetControlledEntity();
			InventoryStorageManagerComponent storageUser = InventoryStorageManagerComponent.Cast(ie.FindComponent(InventoryStorageManagerComponent));
	
			array<typename> componentsUser = {};
			componentsUser.Insert(MoneyComponent);
			componentsUser.Insert(RplComponent);
			IEntity walletEnityUser = storageUser.FindItemWithComponents(componentsUser, EStoragePurpose.PURPOSE_DEPOSIT);
			WST_TransferComponent transferComp = WST_TransferComponent.Cast(ie.FindComponent(WST_TransferComponent));
		
		
			Widget rootWidget = GetRootWidget();
			if (!rootWidget)
						{
							Print("Error in Layout Tutorial layout creation", LogLevel.ERROR);
							return;
						}
						
			Widget panelRoot = rootWidget.FindWidget("Panel0");
						
			SCR_EditBoxComponent EditBox = SCR_EditBoxComponent.GetEditBoxComponent("EditBoxRoot0", panelRoot);
				
			int value = EditBox.GetValue().ToInt();
			Print("OnMenuOpen::TransferCurrencyWindow Confirmed!!", LogLevel.NORMAL);
		
		
		
				//get the player proxy 
			InventoryStorageManagerComponent storage = InventoryStorageManagerComponent.Cast(mOwnerEntity.FindComponent(InventoryStorageManagerComponent));
	
			array<typename> components = {};
			components.Insert(MoneyComponent);
			components.Insert(RplComponent);
			IEntity walletEnity = storage.FindItemWithComponents(components, EStoragePurpose.PURPOSE_DEPOSIT);
			RplComponent rplProxy = RplComponent.Cast(mOwnerEntity.FindComponent(RplComponent));
			RplComponent rplWallet;

			
		
		
		
			//get the local player
			//local player is the User 
			//Transfer from local player to other player 
			//Rpc to server with the amount to transfer to other player + RplId of other players wallet to add money to via MoneyManager 
			//Rpc need to be called from a component attached to character since inside menu base we have no Rpc functions.
			//1.TODO: Create new WST_TransferComponent 
			//2.TODO: Attach WST_TransferComponent to ChimeraCharacter Prefab 
						

			
			
			

			MoneyComponent walletUser;
			RplComponent rpl;
			if(walletEnityUser)
			{
				walletUser = MoneyComponent.Cast(walletEnityUser.FindComponent(MoneyComponent));
				rpl = RplComponent.Cast(walletEnityUser.FindComponent(RplComponent));
			}
			
			if (rpl)
			{
				
			
			
				//try getting rpl id of wallet via FindComponenent
				RplId walletID = rpl.Id();
				int newValue = walletUser.GetValue() - value;
				if(newValue < 0 )
					return;
				//kick off rpc to server (subtract currency from transferring player)
				transferComp.HandleCurrencyTransfer(newValue,walletID);
				
	
			}
		
			//add to proxy inventory
			MoneyComponent wallet;
			if(walletEnity)
			{
				wallet = MoneyComponent.Cast(walletEnity.FindComponent(MoneyComponent));
				rplWallet = RplComponent.Cast(walletEnity.FindComponent(RplComponent));
			}
			
			if (rplWallet)
			{
				Print("OnMenuOpen::TransferCurrencyWindow::Confirmed:: Wallet Found on Owner!", LogLevel.NORMAL);
				RplId walletID = rplWallet.Id();
				
				//kick off rpc to server(probably not firing cause the local player is not the owner of this component) 
				//workaround: fire from local player 
				int newValue = value + wallet.GetValue();
				transferComp.HandleCurrencyTransfer(newValue,walletID);
	
			}
			else
			{
				//wallet not found on receiver (which means no money owned)
				//Create wallet inside player storage with specified amount to transfer via RpcAsk to server
				Print("OnMenuOpen::TransferCurrencyWindow::Confirmed:: Wallet not found, attempting to create!", LogLevel.NORMAL);
				if(rplProxy)
				{
					Print("OnMenuOpen::TransferCurrencyWindow::Confirmed:: Player Proxy found creating wallet!", LogLevel.NORMAL);

					transferComp.CreateWallet(value,rplProxy.Id());

				}
				
			}
		
		Close();

	
	}
	
	void SetActionEntities(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		mOwnerEntity = pOwnerEntity;
		mUserEntity = pUserEntity;
	}
	//------------------------------------------------
	protected override void OnMenuOpen()
	{
		Print("OnMenuOpen::TransferCurrencyWindow menu/dialog opened!", LogLevel.NORMAL);
	


		
		/*
			ESC/Start listener
		*/
		Widget rootWidget = GetRootWidget();
		if (!rootWidget)
		{
			Print("Error in Layout Tutorial layout creation", LogLevel.ERROR);
			return;
		}
		
		Widget panelRoot = rootWidget.FindWidget("Panel0");
		
		SCR_EditBoxComponent EditBox = SCR_EditBoxComponent.GetEditBoxComponent("EditBoxRoot0", panelRoot);
		
		EditBox.m_OnConfirm.Insert(Confirmed);
		//EditBox.SetValue("TESTCHANGE");
		
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