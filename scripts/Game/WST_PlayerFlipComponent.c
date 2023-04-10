class WST_PlayerFlipComponentClass : ScriptComponentClass
{


};


class WST_PlayerFlipComponent: ScriptComponent
{
	
	
	
	
	
	void Flip(RplId id)
	{				
		Print("WST_FlipComponent::Flip id: "+ id);

		Rpc(RpcDo_Flip,id);
		//RpcDo_Flip();
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	void RpcDo_Flip(RplId id)
	{
		Print("WST_FlipComponent::rpc fired");
		
		IEntity ToBeFlippedEntity = IEntity.Cast(Replication.FindItem(id));
		vector transform[4];
		
		ToBeFlippedEntity.GetTransform(transform);
		
		vector angle = Math3D.MatrixToAngles(transform);
		Math3D.AnglesToMatrix(Vector(angle[0],0,0),transform);
		
		
		transform[3][1] = transform[3][1]+0.5;
		
		ToBeFlippedEntity.SetTransform(transform);
		ToBeFlippedEntity.GetPhysics().ApplyImpulse("0 -1 0");
	
	}
	
	
	
	
};
