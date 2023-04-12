
modded class SCR_InventoryMenuUI 
{	
	
	
	//-----------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	protected void SetFocusedSlotEffects()
	{
		if( !m_pFocusedSlotUI )
		{
			if( m_wProgressBarWeightItem )
				m_wProgressBarWeightItem.SetCurrent( 0.0 );
			return;
		}

		//show info about the item
		InventoryItemComponent invItemComp = m_pFocusedSlotUI.GetInventoryItemComponent();
		if ( !invItemComp )
			return;
		auto attribs = SCR_ItemAttributeCollection.Cast( invItemComp.GetAttributes() );

		if ( !attribs )
			return;
		UIInfo itemInfo = attribs.GetUIInfo();
		IEntity ie = invItemComp.GetOwner();
		MoneyComponent mc = MoneyComponent.Cast(ie.FindComponent(MoneyComponent));
		int value
		if(mc)
			value = mc.GetValue();
		if ( !itemInfo )
			HideItemInfo();
		else
		{
			if(mc)
				ShowItemInfo( itemInfo.GetName(), value.ToString(), invItemComp.GetTotalWeight(), SCR_InventoryUIInfo.Cast(itemInfo) );
			else
				ShowItemInfo( itemInfo.GetName(), itemInfo.GetDescription(), invItemComp.GetTotalWeight(), SCR_InventoryUIInfo.Cast(itemInfo) );
		}
		
		
		//show the weight on the progressbar
		//TODO: overlap or add on the end, depending on if the item is already in the storage or is going to be added
		if( m_wProgressBarWeightItem )
		{
			float weight = invItemComp.GetTotalWeight();
			m_wProgressBarWeightItem.SetCurrent( weight );
		};

		NavigationBarUpdate();
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
};



