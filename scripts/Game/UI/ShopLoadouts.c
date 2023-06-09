
class WST_ShopLoadOutBase
{
	ref array<ShopObject> shopItems = new ref array<ShopObject>();
	
	//internal structures
	ref map<string,ResourceName> buildArray = new map<string,ResourceName>();
	ref array<string> iterationArray = new array<string>();
	ref map<string,string> DisplayNameArray = new map<string,string>();
	ref map<string,int> TypeArray = new map<string,int>();
	ref map<string,int> PriceArray = new map<string,int>();

	int GetPriceByKey(string identifier)
	{
		return PriceArray.Get(identifier);
	}
	
	//joins two LoadOutObjects, returns one WST_ShopLoadOutBase object with all objects of the two objecst
	static WST_ShopLoadOutBase AddLoadOuts(WST_ShopLoadOutBase i_loadout, WST_ShopLoadOutBase k_loadout)
	{
		WST_ShopLoadOutBase newLoadOutReturn = new WST_ShopLoadOutBase();
		//get shop items from i 
		ref array<ShopObject> i_objects = new ref array<ShopObject>();
		//ref array<ShopObject> i_objects = new ref array<ShopObject>();

		i_objects = i_loadout.shopItems;
		foreach(ShopObject o:i_objects)
		{
			ShopObject copy = o;
			
		}
		//get shop items from k 
		ref array<ShopObject> k_objects = new ref array<ShopObject>();
		k_objects = k_loadout.shopItems;
		
		//new array with all objects
		ref array< ShopObject> newObjectArray = new ref array<ShopObject>();
		newObjectArray.InsertAll(k_objects);
		newObjectArray.InsertAll(i_objects);
		
		//duplicate check
		foreach(ShopObject o: k_objects)
		{
			//get prefab path 
			ResourceName n = o.m_prefabPath;
			//check if prefab path exists in other array
			foreach(ShopObject o_s : i_objects)
			{
				//get prefab path 
				ResourceName n_s = o_s.m_prefabPath;
				//check if they are the same 
				if (n_s == n )
				{
					//Duplicate found, remove from newObjectArray 
					int index = newObjectArray.Find(o_s);
					newObjectArray.Remove(index);
				
				}
			}
				
		}
		
		
		
		foreach(ShopObject o:newObjectArray)
		{
			newLoadOutReturn.buildArray.Insert(o.m_Identifier,o.m_prefabPath);
			newLoadOutReturn.iterationArray.Insert(o.m_Identifier);
			newLoadOutReturn.DisplayNameArray.Insert(o.m_Identifier,o.m_Display);
			newLoadOutReturn.TypeArray.Insert(o.m_Identifier,o.m_Type);
			newLoadOutReturn.PriceArray.Insert(o.m_Identifier,o.m_price);
		
		}
		newLoadOutReturn.shopItems = newObjectArray;
		return newLoadOutReturn;
	}

}


















class WST_Equipment : WST_ShopLoadOutBase
{
	
	ref ShopObject Optic_TA31RCO= new ShopObject("{2518CA6044D6BBDD}Prefabs/Weapons/Attachments/Optics/ta31rco/Optic_TA31RCO.et","Optic_TA31RCO","Optic TA31RCO",WST_Type.WST_OPTIC,10);
	
	ref ShopObject Optic_SU230= new ShopObject("{DA2AED4A5F958164}Prefabs/Weapons/Attachments/Optics/su230/Optic_SU230.et","Optic_SU230","Optic SU230",WST_Type.WST_OPTIC,10);

	ref ShopObject Optic_M8541= new ShopObject("{333A794F0B55C019}Prefabs/Weapons/Attachments/Optics/m8541/Optic_M8541.et","Optic_M8541","Optic M8541",WST_Type.WST_OPTIC,10);

	ref ShopObject Optic_bravo4= new ShopObject("{969F74903AFFCABE}Prefabs/Weapons/Attachments/Optics/bravo4/Optic_bravo4.et","Optic_bravo4","Optic Bravo4",WST_Type.WST_OPTIC,10);

	ref ShopObject Optic_1P87= new ShopObject("{C4B57488909B3B63}Prefabs/Weapons/Attachments/Optics/1p87/Optic_1P87.et","Optic_1P87","Optic 1P87",WST_Type.WST_OPTIC,10);
	ref ShopObject LaserANPEQ= new ShopObject("{322DE2A7FE4A7566}Prefabs/Weapons/Attachments/Lasers/Laser_ANPEQ16_base.et","LaserANPEQ","ANPEQ16 Laser",WST_Type.WST_ATTACHMENT,10);

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
	

	
	
	private WST_Equipment instance;
	WST_Equipment GetInstance()
	{
		if(instance)
			return instance;
		else
			return new WST_Equipment();
	}
	
	
	void WST_Equipment()
	{
		if (instance)
			return;
		instance = this;	
		
		
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
		shopItems.Insert(LaserANPEQ);
		shopItems.Insert(Optic_1P87);
		shopItems.Insert(Optic_bravo4);
		shopItems.Insert(Optic_M8541);
		shopItems.Insert(Optic_SU230);
		shopItems.Insert(Optic_TA31RCO);
		
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

class WST_WeaponV2 : WST_ShopLoadOutBase
{
	//"suppressed" variants are capable of fitting a suppressor and not actually suppressed
	
	/////////////////////////////////////////////////////////////////////////
	//prefab item paths
	
	//weapon
	
	ref ShopObject Rifle_VZ58P = new ShopObject("{66D32F932F7E8C1F}Prefabs/Weapons/Rifles/VZ58/Rifle_VZ58PMod.et","Rifle_VZ58P_PSO","Rifle VZ58P",WST_Type.WST_RIFLE,300);
	ref ShopObject M16_optic = new ShopObject("{FFD418F9A77519D2}Prefabs/Weapons/Attachments/Optics/Optic_4x20/Optic_4x20Mod.et","M16_optic","M16 Optic",WST_Type.WST_OPTIC,300);
	ref ShopObject Optic_ARTII = new ShopObject("{D2018EDB1BBF4C88}Prefabs/Weapons/Attachments/Optics/Optic_ARTII.et","Optic_ARTII","ARTII Sniper Optic",WST_Type.WST_OPTIC,300);

	
	ref ShopObject Magazine_762x39_Vz58_30rnd_Tracer = new ShopObject("{FAFA0D71E75CEBE2}Prefabs/Weapons/Magazines/Vz58/Magazine_762x39_Vz58_30rnd_Tracer.et","Magazine_762x39_Vz58_30rnd_Tracer","Magazine 762x39 SA58 30rnd",WST_Type.WST_RIFLEAMMO,30);

	
	ref ShopObject Rifle_AK74N_PSO1 = new ShopObject("{E6ABF84D52589026}Prefabs/Weapons/Rifles/AK74M/Rifle_AK74MDuplicateTest.et","Rifle_AK74N_PSO1","Rifle AK74N",WST_Type.WST_RIFLE,300);
	ref ShopObject AK74M_Optic = new ShopObject("{E6ABF84D52589026}Prefabs/Weapons/Rifles/AK74M/Rifle_AK74MDuplicateTest.et","Rifle_AK74M","Rifle AK74M",WST_Type.WST_RIFLE,300);

	ref ShopObject AN94O = new ShopObject("{2D406AEED3CDF362}Prefabs/Weapons/Rifles/AN94/Rifle_AN94_1P63Mod.et","WeaponAN94","AN 74 ",WST_Type.WST_RIFLE,300);

	ref ShopObject AK74O = new ShopObject("{5B308198855FBCE5}Prefabs/Weapons/Rifles/AK74/Rifle_AK74Suppressed.et","WeaponAK74","AK 74",WST_Type.WST_RIFLE,200);
	
	
	ref ShopObject Rifle_M21_ARTIIO = new ShopObject("{0EE4F8425C114813}Prefabs/Weapons/Rifles/M14/Rifle_M21SuppressedNoMag.et","WeaponM21","M21",WST_Type.WST_SNIPERRIFLE,250);
	
	ref ShopObject M16O = new ShopObject("{7500FA1B52CC247F}Prefabs/Weapons/Rifles/M16/Rifle_M16A2_M203Suppressed.et","WeaponM16A2M203","M16A2 ",WST_Type.WST_RIFLE,350);
	
	ref ShopObject StandardSuppressorO = new ShopObject("{7CB432CC4ED704E1}Prefabs/Weapons/Core/Suppressor_Generic_556.et","StandardSuppressor","Standard Suppressor",WST_Type.WST_SUPPRESSOR,100);
	
	ref ShopObject NT4SuppressorO = new ShopObject("{904AF79F5926399A}Prefabs/Weapons/Core/Suppressor_Punky_NT4.et","NT4Suppressor","NT4 Suppressor",WST_Type.WST_SUPPRESSOR,100);
	
	ref ShopObject GP25O = new ShopObject("{262F0D09C4130826}Prefabs/Weapons/Ammo/Ammo_Grenade_HE_VOG25.et","GP25","AKGP VOG25",WST_Type.WST_GRENADES,50);
	
	ref ShopObject M203O = new ShopObject("{5375FA7CB1F68573}Prefabs/Weapons/Ammo/Ammo_Grenade_HE_M406.et","M203","M203 HE (for M16GL)",WST_Type.WST_GRENADES,50);
	
	ref ShopObject USFragO = new ShopObject("{E8F00BF730225B00}Prefabs/Weapons/Grenades/Grenade_M67.et","USFrag","US Grenade",WST_Type.WST_GRENADES,25);
	
	ref ShopObject USSRFragO = new ShopObject("{645C73791ECA1698}Prefabs/Weapons/Grenades/Grenade_RGD5.et","USSRFrag","RGD Grenade",WST_Type.WST_GRENADES,25);
	
	ref ShopObject SmokeO = new ShopObject("{9DB69176CEF0EE97}Prefabs/Weapons/Grenades/Smoke_ANM8HC.et","Smoke","Smoke",WST_Type.WST_GRENADES,25);
	
	ref ShopObject AK74MagO = new ShopObject("{E5912E45754CD421}Prefabs/Weapons/Magazines/Magazine_545x39_AK_30rnd_Tracer.et","AK74Mag","AK 74 Magazine",WST_Type.WST_RIFLEAMMO,10);
	
	ref ShopObject M9MagO = new ShopObject("{9C05543A503DB80E}Prefabs/Weapons/Magazines/Magazine_9x19_M9_15rnd_Ball.et","M9Mag","M9 Pistol Magazine",WST_Type.WST_PISTOLAMMO,10);
	
	ref ShopObject M16MagO = new ShopObject("{A9A385FE1F7BF4BD}Prefabs/Weapons/Magazines/Magazine_556x45_STANAG_30rnd_Tracer.et","M16Mag","M16 Magazine",WST_Type.WST_RIFLEAMMO,10);
	
	ref ShopObject M14MagO = new ShopObject("{627255315038152A}Prefabs/Weapons/Magazines/Magazine_762x51_M14_20rnd_SpecialBall.et","M14Mag","M14 Magazine",WST_Type.WST_SNIPERAMMO,100);
	ref ShopObject UBGL203 = new ShopObject("{4E1032FAA257BEC5}Prefabs/Weapons/Attachments/Underbarrel/UGL_M203Mod.et","UBGL203","UBGL203",WST_Type.WST_UBGL,300);
	
	ref ShopObject GP25attachment = new ShopObject("{CDC3268678BA8765}Prefabs/Weapons/Attachments/Underbarrel/UGL_GP25Mod.et","UBGP25","GP25 UBGL",WST_Type.WST_UBGL,300);
	ref ShopObject PSO1 = new ShopObject("{957E93AD919222C1}Prefabs/Weapons/Attachments/Optics/Optic_PSO1Mod.et","PSO1","Optic PSO1",WST_Type.WST_OPTIC,100);

	
	
	private WST_WeaponV2 instance;
	WST_WeaponV2 GetInstance()
	{
		if(instance)
			return instance;
		else
			return new WST_WeaponV2();
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


class WST_Weapon : WST_ShopLoadOutBase
{
	//"suppressed" variants are capable of fitting a suppressor and not actually suppressed
	
	/////////////////////////////////////////////////////////////////////////
	//prefab item paths
	
	//weapon
	
	ref ShopObject M4 = new ShopObject("{CED9AFEB5B9350BB}Prefabs/Weapons/Rifles/M4A1/RVX_M4A1_RifleMod.et","WeaponM4A1","M4A1",WST_Type.WST_RIFLE,200);
	ref ShopObject M4MK18 = new ShopObject("{42DC70AE2E69965F}Prefabs/Weapons/Rifles/M4A1/RVX_MK18_RifleMod.et","WeaponMK18","M4A1 Mk18",WST_Type.WST_RIFLE,200);

	
	ref ShopObject UBGL203 = new ShopObject("{4E1032FAA257BEC5}Prefabs/Weapons/Attachments/Underbarrel/UGL_M203Mod.et","UBGL203","UBGL203",WST_Type.WST_LAUNCHER,300);

	ref ShopObject AN94O = new ShopObject("{2D406AEED3CDF362}Prefabs/Weapons/Rifles/AN94/Rifle_AN94_1P63Mod.et","WeaponAN94","AN 74 ",WST_Type.WST_RIFLE,300);

	ref ShopObject AK74O = new ShopObject("{5B308198855FBCE5}Prefabs/Weapons/Rifles/AK74/Rifle_AK74Suppressed.et","WeaponAK74","AK 74",WST_Type.WST_RIFLE,200);
	
	

	ref ShopObject Rifle_M21_ARTIIO = new ShopObject("{0EE4F8425C114813}Prefabs/Weapons/Rifles/M14/Rifle_M21SuppressedNoMag.et","WeaponM21ARTII","M21 ",WST_Type.WST_SNIPERRIFLE,650);
	
	ref ShopObject M16O = new ShopObject("{7500FA1B52CC247F}Prefabs/Weapons/Rifles/M16/Rifle_M16A2_M203Suppressed.et","WeaponM16A2M203","M16A2",WST_Type.WST_RIFLE,350);
	
	ref ShopObject StandardSuppressorO = new ShopObject("{7CB432CC4ED704E1}Prefabs/Weapons/Core/Suppressor_Generic_556.et","StandardSuppressor","Standard Suppressor",WST_Type.WST_SUPPRESSOR,100);
	
	ref ShopObject NT4SuppressorO = new ShopObject("{904AF79F5926399A}Prefabs/Weapons/Core/Suppressor_Punky_NT4.et","NT4Suppressor","NT4 Suppressor",WST_Type.WST_SUPPRESSOR,100);
	
	ref ShopObject GP25O = new ShopObject("{262F0D09C4130826}Prefabs/Weapons/Ammo/Ammo_Grenade_HE_VOG25.et","GP25","AKGP VOG25",WST_Type.WST_GRENADES,50);
	
	ref ShopObject M203O = new ShopObject("{5375FA7CB1F68573}Prefabs/Weapons/Ammo/Ammo_Grenade_HE_M406.et","M203","M203 HE (for M16GL)",WST_Type.WST_GRENADES,50);
	
	ref ShopObject USFragO = new ShopObject("{E8F00BF730225B00}Prefabs/Weapons/Grenades/Grenade_M67.et","USFrag","US Grenade",WST_Type.WST_GRENADES,25);
	
	ref ShopObject USSRFragO = new ShopObject("{645C73791ECA1698}Prefabs/Weapons/Grenades/Grenade_RGD5.et","USSRFrag","RGD Grenade",WST_Type.WST_GRENADES,25);
	
	ref ShopObject SmokeO = new ShopObject("{9DB69176CEF0EE97}Prefabs/Weapons/Grenades/Smoke_ANM8HC.et","Smoke","Smoke",WST_Type.WST_GRENADES,25);
	
	ref ShopObject AK74MagO = new ShopObject("{E5912E45754CD421}Prefabs/Weapons/Magazines/Magazine_545x39_AK_30rnd_Tracer.et","AK74Mag","AK 74 Magazine",WST_Type.WST_RIFLEAMMO,10);
	
	ref ShopObject M9MagO = new ShopObject("{9C05543A503DB80E}Prefabs/Weapons/Magazines/Magazine_9x19_M9_15rnd_Ball.et","M9Mag","M9 Pistol Magazine",WST_Type.WST_PISTOLAMMO,10);
	
	ref ShopObject M16MagO = new ShopObject("{A9A385FE1F7BF4BD}Prefabs/Weapons/Magazines/Magazine_556x45_STANAG_30rnd_Tracer.et","M16Mag","M16 Magazine",WST_Type.WST_RIFLEAMMO,10);
	
	ref ShopObject M14MagO = new ShopObject("{627255315038152A}Prefabs/Weapons/Magazines/Magazine_762x51_M14_20rnd_SpecialBall.et","M14Mag","M14 Magazine",WST_Type.WST_SNIPERAMMO,100);
	
	
	
	ref ShopObject AKSUN = new ShopObject("{693B6256851CE42A}Prefabs/Weapons/Rifles/AKS74UN/Rifle_AKS74UNMod.et","AKSUN","AK47S UN",WST_Type.WST_RIFLE,100);
	ref ShopObject M27IAR = new ShopObject("{BB8184D1D15EC5EC}Prefabs/Weapons/Rifles/M27IAR/Rifle_M27IARMod.et","M27IAR","M27I AR",WST_Type.WST_RIFLE,400);
	ref ShopObject APS = new ShopObject("{6EF1D0C87AF5E709}Prefabs/Weapons/Handguns/APS/Handgun_APSMod.et","APS","APS Automatic Pistol",WST_Type.WST_PISTOL,100);

	
	ref ShopObject APSMag = new ShopObject("{DFC0532A49E17C39}Prefabs/Weapons/Magazines/aps_magazine/Magazine_9x18_APS_20rnd_Ball.et","APSMag","APS Magazine",WST_Type.WST_PISTOLAMMO,100);


	private WST_Weapon instance;
	WST_Weapon GetInstance()
	{
		if(instance)
			return instance;
		else
			return new WST_Weapon();
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
		shopItems.Insert(M4);
		shopItems.Insert(M4MK18);
		shopItems.Insert(AKSUN);
		//shopItems.Insert(M27IAR);
		//shopItems.Insert(APS);
		//shopItems.Insert(APSMag);
			
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


class WST_Vehicle : WST_ShopLoadOutBase
{
	//TODO: Rework shops (too much manual typing)
	
	//create one object per shop item, feed them parameters, loop through objects
	/////////////////////////////////////////////////////////////////////////
	
	ref ShopObject HelicopterMD500 = new ShopObject("{CE2BCACA842B6D78}Prefabs/Vehicles/Helicopters/R3D_MD500/R3D_MD500_Black_Benches.et","HeliMD500","MD500 Helicopter",WST_Type.WST_VEHICLE,450);

	ref ShopObject US_JeepO = new ShopObject("{F649585ABB3706C4}Prefabs/Vehicles/Wheeled/M151A2/M151A2.et","US_Jeep","Jeep",WST_Type.WST_VEHICLE,100);
	
	ref ShopObject T14TankO = new ShopObject("{48BB423641D40E77}Prefabs/Vehicles/Tracked/T14/Tank_T14.et","T14Tank","T14 Tank",WST_Type.WST_VEHICLE,100);

	ref ShopObject K17BerezokO = new ShopObject("{31F0866273C46D76}Prefabs/Vehicles/Wheeled/K17/APC_K17_Berezok.et","K17Berezok","K17 Berezok",WST_Type.WST_VEHICLE,100);

	
	ref ShopObject M923A1 = new ShopObject("{9A0D72816DFFDB7F}Prefabs/Vehicles/Wheeled/M923A1/M923A1.et","M923A1","M923 A1",WST_Type.WST_VEHICLE,100);
	ref ShopObject M1025 = new ShopObject("{4A71F755A4513227}Prefabs/Vehicles/Wheeled/M998/M1025.et","M1025","M1025",WST_Type.WST_VEHICLE,100);
	ref ShopObject Ural4320 = new ShopObject("{4597626AF36C0858}Prefabs/Vehicles/Wheeled/Ural4320/Ural4320.et","Ural4320","Ural 4320",WST_Type.WST_VEHICLE,100);
	ref ShopObject SupplyTruck = new ShopObject("{784685DC17DC6C89}Prefabs/MP/Campaign/Assets/CampaignSupplyTruckEast1Mod.et","SupplyUral4320","Supply Truck(for building)",WST_Type.WST_VEHICLE,400);

	
	
	private WST_Vehicle instance;
	WST_Vehicle GetInstance()
	{
		if(instance)
			return instance;
		else
			return new WST_Vehicle();
	}
	
	
	void WST_Vehicle()
	{
		if (instance)
			return;
		instance = this;
		
		shopItems.Insert(US_JeepO);
		shopItems.Insert(T14TankO);
		shopItems.Insert(K17BerezokO);
		shopItems.Insert(M923A1);
		shopItems.Insert(M1025);
		shopItems.Insert(Ural4320);
		shopItems.Insert(SupplyTruck);
		shopItems.Insert(HelicopterMD500);
		
		
		foreach(ShopObject o : shopItems)
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


class ShopObject
{
	ResourceName m_prefabPath;
	string m_Identifier;
	string m_Display;
	int m_Type;
	int m_price;


	void ShopObject(ResourceName path,string Identifier,string Display,int type,int price)
	{
		m_prefabPath = path;
		m_Identifier = Identifier;
		m_Display = Display;
		m_price = price;
		m_Type = type;
	}
};




///////////////////////////////////////////////////////////////////////------------------PreviewSetup