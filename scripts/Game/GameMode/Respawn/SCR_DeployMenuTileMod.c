



modded class SCR_FactionMenuTile 
{
	override void SetSpawnPointsAvailable(string factionKey)
	{
		
		//hack it to always false, since you can always spawn in this wasteland
		if (m_wText.GetText() == factionKey)
		{
			bool available= false;
			
			m_wNoSpawnPoints.SetVisible(available);
		}
	}

};