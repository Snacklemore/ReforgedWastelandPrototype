//------------------------------------------------------------------------------------------------
modded class SCR_CampaignBuildingStartUserAction 
{		
	
	

	
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{		
		if (GetUserRank(user) >= m_ProviderComponent.GetAccessRank())
			return true;
		
		FactionAffiliationComponent factionAffiliationComp = FactionAffiliationComponent.Cast(user.FindComponent(FactionAffiliationComponent));
		if (!factionAffiliationComp)
			return false;
		
		//HOTFIX on stable because Revision: 79642 is not merged
		//string rankName;
		//SCR_Faction faction = SCR_Faction.Cast(factionAffiliationComp.GetAffiliatedFaction());
		//if (faction)
		//	rankName = faction.GetRankName(m_ProviderComponent.GetAccessRank());
			
		SetCannotPerformReason("Too low rank");
		return false;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		if (!m_ProviderComponent)
			InitializeSuppliesComponent();
				
		if (m_ProviderComponent.IsPlayerFactionSame(user))
			return true;
		return true;
	}
	
	


	
	
};
