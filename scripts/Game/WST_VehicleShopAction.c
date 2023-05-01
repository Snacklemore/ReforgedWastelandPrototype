



class WST_VehicleShopAction : ScriptedUserAction
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
		WST_VehicleShop shop = WST_VehicleShop.Cast(GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.WST_VehicleShop));
		//owner is the shop, get rpl ID of the owner 
		RplComponent rplOwner = RplComponent.Cast(pOwnerEntity.FindComponent(RplComponent));
		if (!rplOwner)
			return;
		RplId shopId = rplOwner.Id();
		shop.SetShopId(shopId);

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