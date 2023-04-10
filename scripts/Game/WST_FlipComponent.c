class WST_FlipComponentClass : ScriptComponentClass
{


};



class WST_FlipComponent: ScriptComponent
{
	IEntity ToBeFlippedEntity;
	
	
	
	
	void Flip()
	{
		
		Rpc(RpcDo_Flip);
		//RpcDo_Flip();
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcDo_Flip()
	{
		ToBeFlippedEntity = GetOwner();
		vector transform[4];
		Print("WST_FlipComponent::rpc fired");
		ToBeFlippedEntity.GetTransform(transform);
		
		vector angle = Math3D.MatrixToAngles(transform);
		Math3D.AnglesToMatrix(Vector(angle[0],0,0),transform);
		
		
		transform[3][1] = transform[3][1]+0.5;
		
		ToBeFlippedEntity.SetTransform(transform);
		ToBeFlippedEntity.GetPhysics().ApplyImpulse("0 -1 0");
	
	}
	
	
	
	
};
