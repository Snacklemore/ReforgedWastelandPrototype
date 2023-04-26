
class WST_SaveStashBoxPlacementComponentClass : ScriptComponentClass
{


};



class WST_SaveStashBoxPlacementComponent : ScriptComponent
{

	SCR_InventoryStorageManagerComponent m_storagemanager;
	SCR_BasePreviewEntity m_previewEntity = null ;
	IEntity m_StashBox;
	vector m_offsetToPlayer = Vector(1,0,0);
	
	void SetPreviewEntity(IEntity stashBox, SCR_BasePreviewEntity preview)
	{
		GetGame().GetMenuManager().CloseAllMenus();
		m_previewEntity = preview;
		m_StashBox = stashBox;
		
		GetGame().GetInputManager().AddActionListener("Inventory", EActionTrigger.DOWN, PlaceAndOpenInventory);
		
		//might kick off placement start here?
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcAsk_PlaceBox(vector pos,RplId boxRplId)
	{
		IEntity s_stashbox = Replication.FindItem(boxRplId);
		if (!s_stashbox)
		{
			Print("WST_SaveStashBoxPlacementComponent::no stash box entity found");
			return;
		}
		
		s_stashbox.SetOrigin(pos);
	}
	void PlaceAndOpenInventory()
	{
		Print("WST_SaveStashBoxPlacementComponent::OpenInventory");
		//probably need to place via rpc
		if (!m_StashBox)
		{
			Print("WST_SaveStashBoxPlacementComponent::no stash box entity found");
			return;
		}
		RplComponent rpl = RplComponent.Cast(m_StashBox.FindComponent(RplComponent));
		RplId id = rpl.Id();
		vector pos = m_previewEntity.GetOrigin();
		Rpc(RpcAsk_PlaceBox,pos,id);
	}
	event protected override void EOnFixedFrame(IEntity owner, float timeSlice)
	{
		
		if(!m_previewEntity)
				return;
		IEntity posPlayer = GetGame().GetPlayerController().GetControlledEntity();
		vector transform[4];
		posPlayer.GetTransform(transform);
		
		
		
		//transform[3][1] = transform[3][0]+1.0;
				
		m_previewEntity.SetTransform(transform);
		
		
		
		
		
		
	}
	
	
	
	
	
	
	void WST_SaveStashBoxPlacementComponent()
	{
		SetEventMask(GetOwner(),EntityEvent.FIXEDFRAME);
	}

}