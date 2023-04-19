class WST_Equipment
{
	
	ref array<ShopObject> shopItems = new array<ShopObject>();
	
	ref ShopObject LSzH = new ShopObject("{AC1C630517753E2B}Prefabs/Characters/HeadGear/Helmet_LShZ/Helmet_LShZ_6m2_black.et","LSzH","LSzH Black",WST_Type.WST_HELMET,10);
	ref ShopObject LSzH3M = new ShopObject("{D585D22578362515}Prefabs/Characters/HeadGear/Helmet_LShZ/Helmet_LShZ_HC_3M_black.et","LSzH3M","LSzH Black HC 3M",WST_Type.WST_HELMET,10);
	
	ref ShopObject RatnikOffcVest = new ShopObject("{915E9D97CFAB02D6}Prefabs/Characters/Vests/Vest_Ratnik_6B45/Variants/Vest_Ratnik_6B45_officer.et","RatnikOfficerVest","Ratnic Vest Offc.",WST_Type.WST_VEST,10);
	ref ShopObject Ratnik6b45Vest = new ShopObject("{B9821AF2F5D104E4}Prefabs/Characters/Vests/Vest_Ratnik_6B45/Variants/Vest_Ratnik_6B45.et","Ratnik6b45","Ratnik 6b45",WST_Type.WST_VEST,300);

	ref ShopObject FrogCombatShirt = new ShopObject("{1CF302A33F29FBEF}Prefabs/Characters/Uniforms/Jacket_FROG_Combat_Shirt.et","FrogC","Frog Combat Shirt",WST_Type.WST_JACKET,10);
	ref ShopObject M88VSR = new ShopObject("{D7402E3D0399FF54}Prefabs/Characters/Uniforms/Jacket_M88_VSR.et","M88VSR","M88 Jacker VSR",WST_Type.WST_JACKET,10);
	
	ref ShopObject Suharka = new ShopObject("{CAEDE923EF4071AE}Prefabs/Items/Equipment/Backpacks/Backpack_Suharka_type1.et","Suharka","Suharka Backpack",WST_Type.WST_BACKPACK,10);
	
	

	ref ShopObject PantsV1 = new ShopObject("{251278CF5818A866}Prefabs/Characters/Uniforms/BDUs_Pants_V1_CADPAT_Arid.et","BDUs_Pants_V1_CADPAT_Arid","BDUs Pants CADPAT Arid",WST_Type.WST_PANTS,10);
	
	ref ShopObject PantsV2 = new ShopObject("{F5E7EC1FD0642CDA}Prefabs/Characters/Uniforms/BDUs_Pants_V1_CADPAT_Multiterrain.et","BDUs_Pants_V1_CADPAT_Multiterrain","BDUs Pants CADPAT Multiterrain",WST_Type.WST_PANTS,10);

	ref ShopObject PantsV3 = new ShopObject("{28F02820ACA85E1F}Prefabs/Characters/Uniforms/BDUs_Pants_V1_OCP.et","BDUs_Pants_V1_OCP","BDUs OCP",WST_Type.WST_PANTS,10);

	ref ShopObject PantsV4 = new ShopObject("{6A334A9B21F8CF8A}Prefabs/Characters/Uniforms/Cargo_pants_grey.et","Cargo_pants_grey","Cargo Pants Grey",WST_Type.WST_PANTS,10);
	
	ref ShopObject PantsV5 = new ShopObject("{BE872E50F53F5221}Prefabs/Characters/Uniforms/Cargo_pants_khaki_black.et","Cargo_pants_khakib","Cargo Pants Khaki",WST_Type.WST_PANTS,10);

	ref ShopObject PantsV6 = new ShopObject("{468B58D9A15B95C5}Prefabs/Characters/Uniforms/crye_Pants_Multicam_black.et","crye_Pants_Multicam_black","CRYE Pants Multicam Black",WST_Type.WST_PANTS,10);
	
	

	ref ShopObject JacketsV1 = new ShopObject("{320D65C23FB1C1EB}Prefabs/Characters/Uniforms/flannel_blue.et","flannel_blue","Flannel Blue",WST_Type.WST_JACKET,10);

	ref ShopObject JacketsV2 = new ShopObject("{FAF50584B56353F9}Prefabs/Characters/Uniforms/flannel_brown.et","flannel_brown","Flannel Brown",WST_Type.WST_JACKET,10);
	
	ref ShopObject JacketsV3 = new ShopObject("{FB6C28D783BFBDF9}Prefabs/Characters/Uniforms/flannel_green.et","flannel_green","Flannel Green",WST_Type.WST_JACKET,10);
	
	ref ShopObject JacketsV4 = new ShopObject("{537D2FFA0F71D09E}Prefabs/Characters/Uniforms/OTW_shirt_CADPAT_Arid.et","OTW_shirt_CADPAT_Arid","OTW Shirt CADPAT Arid",WST_Type.WST_JACKET,10);
	
	ref ShopObject JacketsV5 = new ShopObject("{9FDBD0F130898444}Prefabs/Characters/Uniforms/OTW_shirt_CADPAT_Temperate.et","OTW_shirt_CADPAT_Temperate","OTW Shirt CADPAT Temperate",WST_Type.WST_JACKET,10);
	
	ref ShopObject JacketsV6 = new ShopObject("{7618A74CB17B66EB}Prefabs/Characters/Uniforms/OTW_shirt_Multicam_Black.et","OTW_shirt_Multicam_Black","OTW Shirt Multicam Black",WST_Type.WST_JACKET,10);
	

	
	ref map<string,ResourceName> buildArray = new map<string,ResourceName>();
	ref array<string> iterationArray = new array<string>();
	ref map<string,string> DisplayNameArray = new map<string,string>();
	ref map<string,int> TypeArray = new map<string,int>();
	ref map<string,int> PriceArray = new map<string,int>();

	private WST_Equipment instance;
	WST_Equipment GetInstance()
	{
		if(instance)
			return instance;
		else
			return new WST_Equipment();
	}
	
	int GetPriceByKey(string identifier)
	{
		return PriceArray.Get(identifier);
	}
	void WST_Equipment()
	{
		if (instance)
			return;
		instance = this;
		//
		
		
		
		
		
		
		
		
		
		
		//shopItems.Insert(AK74O);
		shopItems.Insert(LSzH);
		shopItems.Insert(LSzH3M);
		shopItems.Insert(RatnikOffcVest);		
		shopItems.Insert(Ratnik6b45Vest);
		shopItems.Insert(FrogCombatShirt);
		shopItems.Insert(Suharka);	
		shopItems.Insert(PantsV1);
		shopItems.Insert(PantsV2);
		shopItems.Insert(PantsV3);
		shopItems.Insert(PantsV4);
		shopItems.Insert(PantsV5);
		shopItems.Insert(PantsV6);

		shopItems.Insert(JacketsV1);
		shopItems.Insert(JacketsV2);
		shopItems.Insert(JacketsV3);
		shopItems.Insert(JacketsV4);
		shopItems.Insert(JacketsV5);
		shopItems.Insert(JacketsV6);
		
	
		
		foreach(ShopObject o:shopItems)
		{
			buildArray.Insert(o.m_Identifier,o.m_prefabPath);
			iterationArray.Insert(o.m_Identifier);
			DisplayNameArray.Insert(o.m_Identifier,o.m_Display);
			TypeArray.Insert(o.m_Identifier,o.m_Type);
			PriceArray.Insert(o.m_Identifier,o.m_price);
		
		}
		
		
		
	}
	
};

class WST_WeaponV2
{
	//"suppressed" variants are capable of fitting a suppressor and not actually suppressed
	
	/////////////////////////////////////////////////////////////////////////
	//prefab item paths
	
	//weapon
	ref array<ShopObject> shopItems = new array<ShopObject>();
	
	ref ShopObject Rifle_VZ58P = new ShopObject("{66D32F932F7E8C1F}Prefabs/Weapons/Rifles/VZ58/Rifle_VZ58PMod.et","Rifle_VZ58P_PSO","Rifle VZ58P",WST_Type.WST_WEAPON,300);
	ref ShopObject M16_optic = new ShopObject("{FFD418F9A77519D2}Prefabs/Weapons/Attachments/Optics/Optic_4x20/Optic_4x20Mod.et","M16_optic","M16 Optic",WST_Type.WST_ATTACHMENT,300);
	ref ShopObject Optic_ARTII = new ShopObject("{D2018EDB1BBF4C88}Prefabs/Weapons/Attachments/Optics/Optic_ARTII.et","Optic_ARTII","ARTII Sniper Optic",WST_Type.WST_ATTACHMENT,300);

	
	ref ShopObject Magazine_762x39_Vz58_30rnd_Tracer = new ShopObject("{FAFA0D71E75CEBE2}Prefabs/Weapons/Magazines/Vz58/Magazine_762x39_Vz58_30rnd_Tracer.et","Magazine_762x39_Vz58_30rnd_Tracer","Magazine 762x39 SA58 30rnd",WST_Type.WST_AMMO,30);

	
	ref ShopObject Rifle_AK74N_PSO1 = new ShopObject("{E6ABF84D52589026}Prefabs/Weapons/Rifles/AK74M/Rifle_AK74MDuplicateTest.et","Rifle_AK74N_PSO1","Rifle AK74N",WST_Type.WST_WEAPON,300);
	ref ShopObject AK74M_Optic = new ShopObject("{E6ABF84D52589026}Prefabs/Weapons/Rifles/AK74M/Rifle_AK74MDuplicateTest.et","Rifle_AK74M","Rifle AK74M",WST_Type.WST_WEAPON,300);

	ref ShopObject AN94O = new ShopObject("{2D406AEED3CDF362}Prefabs/Weapons/Rifles/AN94/Rifle_AN94_1P63Mod.et","WeaponAN94","AN 74 ",WST_Type.WST_WEAPON,300);

	ref ShopObject AK74O = new ShopObject("{5B308198855FBCE5}Prefabs/Weapons/Rifles/AK74/Rifle_AK74Suppressed.et","WeaponAK74","AK 74",WST_Type.WST_WEAPON,200);
	
	
	ref ShopObject Rifle_M21_ARTIIO = new ShopObject("{0EE4F8425C114813}Prefabs/Weapons/Rifles/M14/Rifle_M21SuppressedNoMag.et","WeaponM21","M21",WST_Type.WST_WEAPON,250);
	
	ref ShopObject M16O = new ShopObject("{7500FA1B52CC247F}Prefabs/Weapons/Rifles/M16/Rifle_M16A2_M203Suppressed.et","WeaponM16A2M203","M16A2 ",WST_Type.WST_WEAPON,350);
	
	ref ShopObject StandardSuppressorO = new ShopObject("{7CB432CC4ED704E1}Prefabs/Weapons/Core/Suppressor_Generic_556.et","StandardSuppressor","Standard Suppressor",WST_Type.WST_ATTACHMENT,100);
	
	ref ShopObject NT4SuppressorO = new ShopObject("{904AF79F5926399A}Prefabs/Weapons/Core/Suppressor_Punky_NT4.et","NT4Suppressor","NT4 Suppressor",WST_Type.WST_ATTACHMENT,100);
	
	ref ShopObject GP25O = new ShopObject("{262F0D09C4130826}Prefabs/Weapons/Ammo/Ammo_Grenade_HE_VOG25.et","GP25","AKGP VOG25",WST_Type.WST_GRENADES,50);
	
	ref ShopObject M203O = new ShopObject("{5375FA7CB1F68573}Prefabs/Weapons/Ammo/Ammo_Grenade_HE_M406.et","M203","M203 HE (for M16GL)",WST_Type.WST_GRENADES,50);
	
	ref ShopObject USFragO = new ShopObject("{E8F00BF730225B00}Prefabs/Weapons/Grenades/Grenade_M67.et","USFrag","US Grenade",WST_Type.WST_GRENADES,25);
	
	ref ShopObject USSRFragO = new ShopObject("{645C73791ECA1698}Prefabs/Weapons/Grenades/Grenade_RGD5.et","USSRFrag","RGD Grenade",WST_Type.WST_GRENADES,25);
	
	ref ShopObject SmokeO = new ShopObject("{9DB69176CEF0EE97}Prefabs/Weapons/Grenades/Smoke_ANM8HC.et","Smoke","Smoke",WST_Type.WST_GRENADES,25);
	
	ref ShopObject AK74MagO = new ShopObject("{E5912E45754CD421}Prefabs/Weapons/Magazines/Magazine_545x39_AK_30rnd_Tracer.et","AK74Mag","AK 74 Magazine",WST_Type.WST_AMMO,10);
	
	ref ShopObject M9MagO = new ShopObject("{9C05543A503DB80E}Prefabs/Weapons/Magazines/Magazine_9x19_M9_15rnd_Ball.et","M9Mag","M9 Pistol Magazine",WST_Type.WST_AMMO,10);
	
	ref ShopObject M16MagO = new ShopObject("{A9A385FE1F7BF4BD}Prefabs/Weapons/Magazines/Magazine_556x45_STANAG_30rnd_Tracer.et","M16Mag","M16 Magazine",WST_Type.WST_AMMO,10);
	
	ref ShopObject M14MagO = new ShopObject("{627255315038152A}Prefabs/Weapons/Magazines/Magazine_762x51_M14_20rnd_SpecialBall.et","M14Mag","M14 Magazine",WST_Type.WST_AMMO,100);
	ref ShopObject UBGL203 = new ShopObject("{4E1032FAA257BEC5}Prefabs/Weapons/Attachments/Underbarrel/UGL_M203Mod.et","UBGL203","UBGL203",WST_Type.WST_ATTACHMENT,300);
	
	ref ShopObject GP25attachment = new ShopObject("{CDC3268678BA8765}Prefabs/Weapons/Attachments/Underbarrel/UGL_GP25Mod.et","UBGP25","GP25 UBGL",WST_Type.WST_ATTACHMENT,300);
	ref ShopObject Optic_1P63 = new ShopObject("{CDC3268678BA8765}Prefabs/Weapons/Attachments/Underbarrel/UGL_GP25Mod.et","Optic_1P63","Optic 1P63",WST_Type.WST_ATTACHMENT,300);
	ref ShopObject PSO1 = new ShopObject("{C850A33226B8F9C1}Prefabs/Weapons/Attachments/Optics/Optic_PSO1.et","PSO1","PSO1",WST_Type.WST_ATTACHMENT,100);

	
	ref map<string,ResourceName> buildArray = new map<string,ResourceName>();
	ref array<string> iterationArray = new array<string>();
	ref map<string,string> DisplayNameArray = new map<string,string>();
	ref map<string,int> TypeArray = new map<string,int>();
	ref map<string,int> PriceArray = new map<string,int>();

	private WST_WeaponV2 instance;
	WST_WeaponV2 GetInstance()
	{
		if(instance)
			return instance;
		else
			return new WST_WeaponV2();
	}
	
	int GetPriceByKey(string identifier)
	{
		return PriceArray.Get(identifier);
	}
	void WST_WeaponV2()
	{
		if (instance)
			return;
		instance = this;
		//
		shopItems.Insert(AK74O);
		shopItems.Insert(Rifle_M21_ARTIIO);
		shopItems.Insert(M16O);
		shopItems.Insert(StandardSuppressorO);
		
		shopItems.Insert(NT4SuppressorO);
		shopItems.Insert(GP25O);
		shopItems.Insert(M203O);
		shopItems.Insert(USFragO);
		shopItems.Insert(USSRFragO);
		
		shopItems.Insert(SmokeO);
		shopItems.Insert(AK74MagO);
		shopItems.Insert(M9MagO);
		shopItems.Insert(M16MagO);
		shopItems.Insert(M14MagO);
		shopItems.Insert(AN94O);
		shopItems.Insert(AK74M_Optic);
		shopItems.Insert(M16_optic);

		shopItems.Insert(Rifle_VZ58P);
		shopItems.Insert(Rifle_AK74N_PSO1);
		shopItems.Insert(Magazine_762x39_Vz58_30rnd_Tracer);
		shopItems.Insert(UBGL203);
		shopItems.Insert(GP25attachment);
		shopItems.Insert(Optic_ARTII);
		//shopItems.Insert(Optic_1P63);
		shopItems.Insert(PSO1);	
		
		
		foreach(ShopObject o:shopItems)
		{
			buildArray.Insert(o.m_Identifier,o.m_prefabPath);
			iterationArray.Insert(o.m_Identifier);
			DisplayNameArray.Insert(o.m_Identifier,o.m_Display);
			TypeArray.Insert(o.m_Identifier,o.m_Type);
			PriceArray.Insert(o.m_Identifier,o.m_price);
		
		}
		
		
		
	}
	
};


class WST_Weapon
{
	//"suppressed" variants are capable of fitting a suppressor and not actually suppressed
	
	/////////////////////////////////////////////////////////////////////////
	//prefab item paths
	
	//weapon
	ref array<ShopObject> shopItems = new array<ShopObject>();
	
	ref ShopObject UBGL203 = new ShopObject("{4E1032FAA257BEC5}Prefabs/Weapons/Attachments/Underbarrel/UGL_M203Mod.et","UBGL203","UBGL203",WST_Type.WST_ATTACHMENT,300);

	ref ShopObject AN94O = new ShopObject("{2D406AEED3CDF362}Prefabs/Weapons/Rifles/AN94/Rifle_AN94_1P63Mod.et","WeaponAN94","AN 74 ",WST_Type.WST_WEAPON,300);

	ref ShopObject AK74O = new ShopObject("{5B308198855FBCE5}Prefabs/Weapons/Rifles/AK74/Rifle_AK74Suppressed.et","WeaponAK74","AK 74",WST_Type.WST_WEAPON,200);
	
	
	ref ShopObject Rifle_M21_ARTIIO = new ShopObject("{0EE4F8425C114813}Prefabs/Weapons/Rifles/M14/Rifle_M21SuppressedNoMag.et","WeaponM21ARTII","M21 ",WST_Type.WST_WEAPON,650);
	
	ref ShopObject M16O = new ShopObject("{7500FA1B52CC247F}Prefabs/Weapons/Rifles/M16/Rifle_M16A2_M203Suppressed.et","WeaponM16A2M203","M16A2",WST_Type.WST_WEAPON,350);
	
	ref ShopObject StandardSuppressorO = new ShopObject("{7CB432CC4ED704E1}Prefabs/Weapons/Core/Suppressor_Generic_556.et","StandardSuppressor","Standard Suppressor",WST_Type.WST_ATTACHMENT,100);
	
	ref ShopObject NT4SuppressorO = new ShopObject("{904AF79F5926399A}Prefabs/Weapons/Core/Suppressor_Punky_NT4.et","NT4Suppressor","NT4 Suppressor",WST_Type.WST_ATTACHMENT,100);
	
	ref ShopObject GP25O = new ShopObject("{262F0D09C4130826}Prefabs/Weapons/Ammo/Ammo_Grenade_HE_VOG25.et","GP25","AKGP VOG25",WST_Type.WST_GRENADES,50);
	
	ref ShopObject M203O = new ShopObject("{5375FA7CB1F68573}Prefabs/Weapons/Ammo/Ammo_Grenade_HE_M406.et","M203","M203 HE (for M16GL)",WST_Type.WST_GRENADES,50);
	
	ref ShopObject USFragO = new ShopObject("{E8F00BF730225B00}Prefabs/Weapons/Grenades/Grenade_M67.et","USFrag","US Grenade",WST_Type.WST_GRENADES,25);
	
	ref ShopObject USSRFragO = new ShopObject("{645C73791ECA1698}Prefabs/Weapons/Grenades/Grenade_RGD5.et","USSRFrag","RGD Grenade",WST_Type.WST_GRENADES,25);
	
	ref ShopObject SmokeO = new ShopObject("{9DB69176CEF0EE97}Prefabs/Weapons/Grenades/Smoke_ANM8HC.et","Smoke","Smoke",WST_Type.WST_GRENADES,25);
	
	ref ShopObject AK74MagO = new ShopObject("{E5912E45754CD421}Prefabs/Weapons/Magazines/Magazine_545x39_AK_30rnd_Tracer.et","AK74Mag","AK 74 Magazine",WST_Type.WST_AMMO,10);
	
	ref ShopObject M9MagO = new ShopObject("{9C05543A503DB80E}Prefabs/Weapons/Magazines/Magazine_9x19_M9_15rnd_Ball.et","M9Mag","M9 Pistol Magazine",WST_Type.WST_AMMO,10);
	
	ref ShopObject M16MagO = new ShopObject("{A9A385FE1F7BF4BD}Prefabs/Weapons/Magazines/Magazine_556x45_STANAG_30rnd_Tracer.et","M16Mag","M16 Magazine",WST_Type.WST_AMMO,10);
	
	ref ShopObject M14MagO = new ShopObject("{627255315038152A}Prefabs/Weapons/Magazines/Magazine_762x51_M14_20rnd_SpecialBall.et","M14Mag","M14 Magazine",WST_Type.WST_AMMO,100);
	
	
	ref ShopObject PSO1 = new ShopObject("{C850A33226B8F9C1}Prefabs/Weapons/Attachments/Optics/Optic_PSO1.et","PSO1","PSO1",WST_Type.WST_ATTACHMENT,100);

	
	ref map<string,ResourceName> buildArray = new map<string,ResourceName>();
	ref array<string> iterationArray = new array<string>();
	ref map<string,string> DisplayNameArray = new map<string,string>();
	ref map<string,int> TypeArray = new map<string,int>();
	ref map<string,int> PriceArray = new map<string,int>();

	private WST_Weapon instance;
	WST_Weapon GetInstance()
	{
		if(instance)
			return instance;
		else
			return new WST_Weapon();
	}
	
	int GetPriceByKey(string identifier)
	{
		return PriceArray.Get(identifier);
	}
	void WST_Weapon()
	{
		if (instance)
			return;
		instance = this;
		//
		
		
		shopItems.Insert(AK74O);
		shopItems.Insert(Rifle_M21_ARTIIO);
		shopItems.Insert(M16O);
		shopItems.Insert(StandardSuppressorO);
		
		shopItems.Insert(NT4SuppressorO);
		shopItems.Insert(GP25O);
		shopItems.Insert(M203O);
		shopItems.Insert(USFragO);
		shopItems.Insert(USSRFragO);
		
		shopItems.Insert(SmokeO);
		shopItems.Insert(AK74MagO);
		shopItems.Insert(M9MagO);
		shopItems.Insert(M16MagO);
		shopItems.Insert(M14MagO);
		shopItems.Insert(AN94O);
		shopItems.Insert(UBGL203);

		
			
		foreach(ShopObject o:shopItems)
		{
			buildArray.Insert(o.m_Identifier,o.m_prefabPath);
			iterationArray.Insert(o.m_Identifier);
			DisplayNameArray.Insert(o.m_Identifier,o.m_Display);
			TypeArray.Insert(o.m_Identifier,o.m_Type);
			PriceArray.Insert(o.m_Identifier,o.m_price);
		
		}
		
		
		
	}
	
};

class ManagedDataObject : Managed
{
	string TestStringData;
	WST_Type Type;
	string DisplayText;
	
	void SetData(string data)
	{
		TestStringData = data;
	}
	
	string GetData()
	{
		return TestStringData;
	}
	
	string GetDisplayText()
	{
		return DisplayText;
	}
	
	WST_Type GetType()
	{	
		return Type;
	}
	
	void SetDisplayText(string txt)
	{
		DisplayText = txt;
	}
	void SetType(WST_Type s )
	{
		Type = s;
	}

};


///////////////////////////////////////////////////////////////////////------------------PreviewSetup