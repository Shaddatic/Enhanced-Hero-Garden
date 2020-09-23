#include "stdafx.h"
#include "LandTableInfo.h"
#include "ModelInfo.h"
#include "SA2ModLoader.h"
#include "IniFile.h"
#include "functs.h"

#define ReplaceSET(a, b) helperFunctions.ReplaceFile("resource\\gd_PC\\" a ".bin", "resource\\gd_PC\\" b ".bin");

extern "C"
{

	NJS_VECTOR hTransportPos { 0, -0.1f + 2.0f, 0 };

	void TransporterLoadingHero()
	{
		NJS_VECTOR position;
		position.x = hTransportPos.x;
		position.y = -0.1f;
		position.z = hTransportPos.z;
		Transporter_Load(&position, 24940);
	}

	void sALO_Horse_Load()
	{
		NJS_VECTOR heroHorsePos = { 64.0f, 0.0f, 24.0f };
		ALO_Horse_Load_New((int)&heroHorsePos);
	}
	int heroHorseRot = 42000;

	void hALO_Ball_Load()
	{
		NJS_VECTOR ballPos = { -71.0f, 5.0f, 27.0f };
		NJS_VECTOR otherVector = { 0.0f, 0.0f, 0.0f };
		ALO_Ball_Load(&ballPos, &otherVector);
	}

	ObjectFunc(sub_54CD40, 0x54CD40);
	void CloudSpin(ObjectMaster* a1)
	{
		LandTable* EHero = (LandTable*)GetDllData("objLandTableHero");
		sub_54CD40(a1);
		EHero->COLList[6].Model->ang[1] += 1;
		EHero->COLList[7].Model->ang[1] += 2;
		EHero->COLList[8].Model->ang[1] += 4;
	}

	NJS_TEXNAME eHeroTexCount[37];
	NJS_TEXLIST eHeroTexList = { arrayptrandlength(eHeroTexCount) };


	__declspec(dllexport) void Init(const char* path, const HelperFunctions& helperFunctions)
	{

		HMODULE hmodule = GetModuleHandle(__TEXT("Data_DLL_orig"));

		WriteCall((void*)0x0052B408, nullsub_1); //Disables Height
		WriteCall((void*)0x0052B430, nullsub_1);

		WriteData((int*)0x54D3AB, (int)CloudSpin);

		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");

		bool dcComp = config->getBool("GenSet", "DCComp", false);
		if (dcComp)
		{
			hTransportPos.x = 109.80f;
			hTransportPos.z = 110.13f;
		}
		else
		{
			hTransportPos.x = 123.27f;
			hTransportPos.z = 122.27f;
		}

		WriteData((float**)0x52B91F, &hTransportPos.x);
		WriteData((float**)0x0052B927, &hTransportPos.y);
		WriteData((float**)0x0052B930, &hTransportPos.z);
		WriteCall((void*)0x0054D3F4, TransporterLoadingHero);

		ReplaceSET("set_chao_hero_s", "set_chao_ehero_s")

		bool altSET = config->getBool("GenSet", "AltSet", true);
		bool Set = config->getBool("GenSet", "Set", true);
		if (Set)
		{
			if (altSET)
			{
				ReplaceSET("set_chao_hero_u", "set_chao_hero_u_alt")
			}
		}
		else
		{
			ReplaceSET("set_chao_hero_u", "set_null")
		}

		bool toys = config->getBool("GenSet", "Toys", true);
		if (toys)
		{
			WriteCall((void*)0x0054D490, sALO_Horse_Load);
			WriteData((int*)0x00580F80, heroHorseRot);
			WriteCall((void*)0x0054D44B, hALO_Ball_Load); //Toys
		}

		((NJS_TEXLIST*)GetDllData("texlist_al_stg_hero_tex"))->textures = eHeroTexCount;

		LandTable* Herotable = (LandTable*)GetDllData("objLandTableHero");
		NJS_OBJECT* river = (NJS_OBJECT*)GetDllData("object_ghero_nc_river00_river00");
		NJS_OBJECT* spout0 = (NJS_OBJECT*)GetDllData("object_ghero_nc_spout00_spout00");
		NJS_OBJECT* spout1 = (NJS_OBJECT*)GetDllData("object_ghero_nc_spout01_spout01");
		NJS_OBJECT* spout2 = (NJS_OBJECT*)GetDllData("object_ghero_nc_spout02_spout02");
		NJS_OBJECT* spout3 = (NJS_OBJECT*)GetDllData("object_ghero_nc_spout03_spout03");
		NJS_OBJECT* pot = (NJS_OBJECT*)GetDllData("object_ghero_nc_pot_pot");

		LandTable* EHero = (LandTable*)GetProcAddress(hmodule, "objLandTableHero");
		*EHero = *(new LandTableInfo(std::string(path) + "\\Landtables\\EHero.sa2blvl"))->getlandtable();
		EHero->TextureList = &eHeroTexList;
		EHero->TextureName = (char*)"al_stg_hero_tex";

		NJS_OBJECT* ERiver = EHero->COLList[0].Model;
		NJS_OBJECT* ESpout0 = EHero->COLList[1].Model;
		NJS_OBJECT* ESpout1 = EHero->COLList[2].Model;
		NJS_OBJECT* ESpout2 = EHero->COLList[3].Model;
		NJS_OBJECT* ESpout3 = EHero->COLList[4].Model;
		NJS_OBJECT* EPot = EHero->COLList[5].Model;

		EHero->COLList[0].Flags &= ~0x80000000;
		EHero->COLList[1].Flags &= ~0x80000000;
		EHero->COLList[2].Flags &= ~0x80000000;
		EHero->COLList[3].Flags &= ~0x80000000;
		EHero->COLList[4].Flags &= ~0x80000000;
		EHero->COLList[5].Flags &= ~0x80000000;

		*river = *ERiver;
		*spout0 = *ESpout0;
		*spout1 = *ESpout1;
		*spout2 = *ESpout2;
		*spout3 = *ESpout3;
		*pot = *EPot;

		DataArray(NJS_VECTOR, ChaoPos, 0x01366260, 16 * 3);

		ChaoPos[16] = { 31.1f, 32.9f, -48.3f };              // Tip of Diving Board
		ChaoPos[17] = { -69.0f, 0.3f, 10.0f };               // Just outside Entrance
		ChaoPos[18] = { 76.7f, 1.7f, -80.4f };               // In front of the fountain
 		ChaoPos[19] = { -97.4f, 16.5f, -101.6f };            // Hill beside gazebo
		ChaoPos[20] = { -3.0f, 5.2f, 62.8f };                // Bridge Spawn
		ChaoPos[21] = { -12.8f, 0.0f, -16.9f };              // Ground: In front of Arch
		ChaoPos[22] = { -30.1f, 8.0f, 36.5f };               // Ground: Left of Spawn
		ChaoPos[23] = { -2.8f, 7.0f, -64.6f };               // Under Arch
		ChaoPos[24] = { 64.8f, 0.0f, 88.9f };                // Across River
		ChaoPos[25] = { -33.3f, 32.9f, -90.0f };             // In Gazebo 1
		ChaoPos[26] = { -20.9f, 32.9f, -72.0f };             // In Gazebo 2
		ChaoPos[27] = { 108.2f, 1.61f, -72.5f };             // Beside Fountain
		ChaoPos[28] = { 107.3f, 0.0f, 88.1f };               // Near Transporter
		ChaoPos[30] = { 48.1f, 0.0f, 36.8f };                // Next to River
		ChaoPos[31] = { 102.4f, 58.5f, -131.5f };            // On top of the Emblem

		bool ndrown = config->getBool("GenSet", "NoDrown", false);
		if (ndrown)
		{
			ChaoPos[29] = { 107.3f, 0.0f, 88.1f };
		}
		else
		{
			ChaoPos[29] = { 62.4f, -2.0f, -35.7f };              // Swim
		}

		DataArray(NJS_VECTOR, TreePos, 0x01366B30, 30);

		TreePos[10] = { -104.69f, 1.0f, 10.15f };            // Normal Tree 1 (Near Entrance)
		TreePos[11] = { 89.11f, 1.0f, 63.58f };              // Normal Tree 2 (Over River)
		TreePos[12] = { -114.68f, 15.58f, -111.89f };        // Normal Tree 3 (On the Hill)
		TreePos[13] = { -28.98f, 1.0f, -6.37f };             // Custom Tree 1 (Nearest Gazebo)
		TreePos[14] = { -30.15f, 15.15f, -126.83f };         // Custom Tree 2 (Near Entrance)
		TreePos[15] = { 54.57f, 1.0f, 109.94f };             // Custom Tree 3 (Over River)
		TreePos[16] = { 82.21f, 1.0f, 9.123f };              // Custom Tree 4 (Over River, Near River)

		WriteData((void*)0x01363530, eHeroWaypoints, 5120);

		//WriteData((void*)0x01362130, eHeroWaypoints, 5120); //Chao
		//WriteData((void*)0x01364930, eHeroWaypoints, 5120); //Dark
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}