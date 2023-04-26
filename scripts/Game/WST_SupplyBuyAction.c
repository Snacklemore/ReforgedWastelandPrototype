class WST_SupplyBuyAction : ScriptedUserAction
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
		//RandomGenerator randomGenerator = new RandomGenerator();
		//vector teleportPosition = randomGenerator.GenerateRandomPointInRadius(m_iSpawnMinDist, m_iSpawnMaxDist, m_vTeleportDestination);
		//pUserEntity.SetOrigin(teleportPosition);
		SCR_CampaignSuppliesComponent truckNearby = FindSupplieComponentNearby(pUserEntity);
		if(truckNearby)
			GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.WST_SupplysShopWindow);

	}
	SCR_CampaignSuppliesComponent FindSupplieComponentNearby(IEntity player)
	{
	
		GetGame().GetWorld().QueryEntitiesBySphere(player.GetOrigin(),10.0,QueryEntities);
		
		if (m_supplies.Count() > 1)
		{
			Print("Too many trucks found!");
			SCR_HintManagerComponent.ShowCustomHint("Too many trucks found! ",5.0,false,EFieldManualEntryId.NONE,false);

			return null;
		}
		else if (m_supplies.Count() == 0)
		{
			Print("No truck found!");
			SCR_HintManagerComponent.ShowCustomHint("No Truck found!",5.0,false,EFieldManualEntryId.NONE,false);

			return null;
		}
		return m_supplies[0];
	}
	
	
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
			

	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool HasLocalEffectOnlyScript()
	{
		return true;
	}
};