
class WST_SaveStashBoxComponentClass : ScriptComponentClass
{


};



class WST_SaveStashBoxComponent : ScriptComponent
{

	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcAsk_Hide()
	{
		Print("WST_SaveStashBoxPlacementComponent::RpcHideServer");
	}
	
	void AskHide()
	{
		Rpc(RpcAsk_Hide);
	}

}