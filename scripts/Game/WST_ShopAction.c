class WST_UserAction : ScriptedUserAction
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
				GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.WST_TransferWindow);

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