

class WST_TransferCurrency : ScriptedUserAction
{
	[Attribute(defvalue: "0 0 0", uiwidget: UIWidgets.Coords, desc: "Teleport destination")]
	protected vector m_vTeleportDestination;

	[Attribute(defvalue: "2", uiwidget: UIWidgets.EditBox, desc: "Spawn min. distance")]
	protected int m_iSpawnMinDist;

	[Attribute(defvalue: "4", uiwidget: UIWidgets.EditBox, desc: "Spawn max. distance")]
	protected int m_iSpawnMaxDist;

	//------------------------------------------------------------------------------------------------
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		//check if wallet is present first! (if no wallet on player no transfer)
		
		
		
		
		
		
		//check proxy Player
		
		
		SCR_CharacterDamageManagerComponent DamageManager = SCR_CharacterDamageManagerComponent.Cast(pOwnerEntity.FindComponent(SCR_CharacterDamageManagerComponent));
		if (DamageManager)
		{	
			if (DamageManager.GetState() == EDamageState.DESTROYED)
			{
				SCR_HintManagerComponent.ShowCustomHint("Hes dead Jim...","Bruh",3.0,false,EFieldManualEntryId.NONE,false);

				return;
			}
		}
		//check local Player 
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
		
		if (wallet)
		{
			SCR_HintManagerComponent.ShowCustomHint("Your current balance: "+ wallet.GetValue(),"Wallet Info",3.0,false,EFieldManualEntryId.NONE,false);
			WST_TransferCurrencyWindow base = WST_TransferCurrencyWindow.Cast(GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.WST_TransferCurrencyWindow));
			base.SetActionEntities(pOwnerEntity,pUserEntity);
		}
			


	}

	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		//get the target? how ? no idea 
		
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool HasLocalEffectOnlyScript()
	{
		return true;
	}
};