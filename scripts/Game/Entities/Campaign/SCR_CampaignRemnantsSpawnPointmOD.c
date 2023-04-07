

//------------------------------------------------------------------------------------------------
//! Spawn point for Remnant forces in Campaign
modded class SCR_CampaignRemnantsSpawnPoint 
{
	[Attribute("-1", UIWidgets.EditBox, "Waypoints will be placed in ascending sequence. Valid only for hierarchy children.")]
	protected int m_iWaypointIndex;
	
	[Attribute("0", UIWidgets.ComboBox, "Valid only for hierarchy parent.", "", ParamEnumArray.FromEnum(SCR_CampaignRemnantsGroupType))]
	private SCR_CampaignRemnantsGroupType m_eGroupType;
	
	[Attribute("0", UIWidgets.EditBox, "How often will the group respawn. (0 = no respawn)")]
	protected int m_iGroupRespawnPeriod;
	
	[Attribute("0", UIWidgets.CheckBox, "This entity is used as a respawn position from which Remnants will move to their target.")]
	protected bool m_bIsRespawn;
	
	//------------------------------------------------------------------------------------------------
	int GetWaypointIndex()
	{
		return m_iWaypointIndex;
	}
	
	void SetGroupType(SCR_CampaignRemnantsGroupType type)
	{
		m_eGroupType = type;
	}
	
	void SetIsRespawn(bool b)
	{
		m_bIsRespawn = b;
	}
	
	void SetGroupRespawnPeriod(int n)
	{
		m_iGroupRespawnPeriod = n;
	}
	//------------------------------------------------------------------------------------------------
	SCR_CampaignRemnantsGroupType GetGroupType()
	{
		return m_eGroupType;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetGroupRespawnPeriod()
	{
		return m_iGroupRespawnPeriod;
	}
	
	//------------------------------------------------------------------------------------------------
	bool GetIsRespawn()
	{
		return m_bIsRespawn;
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		// No need to register children unless the group type is specified (indicating default base defenders near HQ)
		if (GetParent() && m_eGroupType == SCR_CampaignRemnantsGroupType.RANDOM)
			return;
		
		SCR_BaseGameModeWasteland campaign = SCR_BaseGameModeWasteland.GetInstance();
		
		if (!campaign)
			return;
		int no = Math.RandomInt(1,5);
		SetGroupType(no);
		// Gamemode will take care of the rest
		campaign.RegisterRemnantsPresence(this);
		Print("SCR_CampaignRemnantsSpawnPoint::OnInit()");
	}
	
	//------------------------------------------------------------------------------------------------
	void SCR_CampaignRemnantsSpawnPoint(IEntitySource src, IEntity parent)
	{
		#ifdef WORKBENCH
			SetFlags(EntityFlags.TRACEABLE, false);
		#else
			SetFlags(EntityFlags.NO_LINK, false);
		#endif
		
		SetEventMask(EntityEvent.INIT);
	}
};

