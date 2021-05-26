/*
	XmbControl
	Copyright (C) 2011, Total_Noob
	Copyright (C) 2011, Frostegater
	Copyright (C) 2011, codestation

	nidresolver.c: XmbControl code for relosving individual nids for any FW
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <pspsdk.h>
#include <pspkernel.h>
#include <psputility_sysparam.h>
#include <systemctrl.h>
#include <kubridge.h>
#include <conf.h>
#include <string.h>
#include <main.h>

PSP_MODULE_INFO("XmbControlMEFreecore", 0x0007, 1, 5);

typedef struct
{
	u32 patch620;
    u32 patch63x;
    u32 patch660;
}
patches;

typedef struct
{
	u32 nid620;
    u32 nid63x;
    u32 nid660;
}
nid;

patches patch_table[] =
{
	/* PatchVshMain */
	{ 0x21E18, 0x22608, 0x22648 },//0
	{ 0x16340, 0x169A8, 0x16A70 },//1
	{ 0x16734, 0x16D9C, 0x16E64 },//2
	{ 0x15D38, 0x163A0, 0x16468 },//3
	{ 0x16284, 0x168EC, 0x169B4 },//4
	{ 0x5D10 , 0x5DA0 , 0x5DA0  },//5
	{ 0x19C40, 0x1A2EC, 0x1A2D0 },//6
	{ 0x206F8, 0x20EBC, 0x20EFC },//7
	{ 0x1631C, 0x16984, 0x16A4C },//8
	{ 0x2FF8C, 0x30828, 0x308B4 },//9
	{ 0x16514, 0x16B7C, 0x16C44 },//10
	{ 0x5E44 , 0x5ED4 , 0x5ED4  },//11
	{ 0x5E48 , 0x5ED8 , 0x5ED8  },//12
	{ 0x3ECB0, 0x3F264, 0x3F1B0 },//13 
	{ 0x51EBC, 0x52E34, 0x530A8 },//14
	{ 0x51EC8, 0x52E40, 0x530B4 },//15
	
	/* PatchSysconfPlugin */
	{ 0x27918, 0x2828C, 0x286AC },//16
	{ 0x22F54, 0x23854, 0x23C74 },//17
	{ 0x1C3A0, 0x1CC08, 0x1D054 },//18
	{ 0x32608, 0x330F0, 0x335FC },//19
	{ 0x32ADC, 0x335BC, 0x33ACC },//20
	{ 0x28B8 , 0x29AC , 0x29AC  },//21
	{ 0x16C8 , 0x1714 , 0x1714  },//22
	{ 0x16EC , 0x1738 , 0x1738  },//23
	{ 0x2934 , 0x2A28 , 0x2A28  },//24
	{ 0x28DA4, 0x299E0, 0x29A90 },//25
	{ 0x28FFC, 0x29A98, 0x29B18 },//26
	{ 0x2901C, 0x29708, 0x299E0 },//27
	{ 0x2FA24, 0x30424, 0x30908 },//28
	
	/* PatchAuthPlugin */
	{ 0x13B4 , 0x13A0 , 0x13A0  },//29
	{ 0x18920, 0x18F3C, 0x192E0 },//30
	{ 0x18650, 0x18C6C, 0x19000 },//31
	{ 0x5C8  , 0x5C8  , 0x5C8   },//32
	{ 47     , 47     , 46      },//33
};

nid nids[] =
{
	{ 0x03A0E8C2, 0xDE69A6CD, 0xAF067FA2 }, // sce_paf_private_wcslen
	{ 0xE8473E80, 0x4C386F3C, 0xA138A376 }, // sce_paf_private_sprintf
	{ 0xF7A832C8, 0xDCE3B13E, 0xB4652CFE }, // sce_paf_private_memcpy
	{ 0xBF48C1FC, 0x5870455C, 0xD9E2D6E1 }, // sce_paf_private_memset
	{ 0x58189108, 0x49A72E5D, 0xD7DCB972 }, // sce_paf_private_strlen
	{ 0xE5A74996, 0x8F95CC01, 0x726DFBA9 }, // sce_paf_private_strcpy
	{ 0x4F487FBC, 0xD38E62C6, 0x706ABBFF }, // sce_paf_private_strncpy
	{ 0x4900119B, 0x1B952318, 0x4CF09BA2 }, // sce_paf_private_strcmp
	{ 0xE00E38F8, 0x9DF5623C, 0xE0B32AE8 }, // sce_paf_private_strncmp
	{ 0x3029535C, 0xFD6C776F, 0xC3B2D310 }, // sce_paf_private_strchr
	{ 0x42D04DD2, 0xCFC81D9F, 0x22420CC7 }, // sce_paf_private_strrchr
	{ 0x2C433251, 0x4853DF6E, 0x4CE9C8D7 }, // sce_paf_private_strpbrk
	{ 0x0C962B6E, 0x5612DE15, 0xA4B8A4E3 }, // sce_paf_private_strtoul
	{ 0xF0D98BD1, 0x9E9FFBFB, 0x5E909060 }, // sce_paf_private_malloc    
	{ 0xE0E8820F, 0xB0363C2E, 0xDB1612F1 }, // sce_paf_private_free
	{ 0xCB608DE5, 0x70082F6F, 0x3874A5F8 }, // scePafGetText
	{ 0x511991AE, 0xE03A5C26, 0x50DCB767 }, // PAF_Resource_GetPageNodeByID
	{ 0xC8C59436, 0xFDAFC9E9, 0x64A31513 }, // PAF_Resource_ResolveRefWString
	{ 0xE73C355B, 0x3A370539, 0x6C582683 }, // vshGetRegistryValue
	{ 0x2375A440, 0xCD3AF2EC, 0x858291A3 }, // vshSetRegistryValue
	{ 0xC2BEAB08, 0x51FC9D2D, 0x8D157BBD }, // sceVshCommonGuiBottomDialog
};

void Resolver()
{
	u32 stub_top = (u32) module_info.stub_top;
	u32 stub_end = (u32) module_info.stub_end;
	
	while (stub_top < stub_end)
	{
		/* Start Resolve NID`s */
		SceLibraryStubTable *stub = (SceLibraryStubTable *) stub_top;
		if (strcmp(stub->libname, "scePaf") == 0 
		|| strcmp(stub->libname, "vsh") == 0 
		|| strcmp(stub->libname, "sceVshCommonGui") == 0)
			for (u32 i = 0; i < stub->stubcount; i++)
				for (u32 x = 0; x < sizeof(nids) / sizeof(nid); x++)
					if (stub->nidtable[i] == nids[x].nid63x)
					{
						if (devkit == FW(0x620))
							stub->nidtable[i] = nids[x].nid620;
						else if (devkit > FW(0x620) && devkit < FW(0x640))
							stub->nidtable[i] = nids[x].nid63x;
						else if (devkit == FW(0x660))
							stub->nidtable[i] = nids[x].nid660;
					}
		stub_top += (u32)(stub->len * 4);
	}

	for(int u = 0; u < sizeof(patch_table) / sizeof(patches); u++)
	{
		if (devkit == FW(0x620))
			patch[u] = patch_table[u].patch620;
		else if (devkit > FW(0x620) && devkit < FW(0x640))
		{
			/* 6.35 OnXmbPush and OnXmbContext patches */
			if(devkit == FW(0x635) && (u == 14 || u == 15))
			{
				patch[14] = 0x52E3C;
				patch[15] = 0x52E48;
			}
			else
				patch[u] = patch_table[u].patch63x;
		}
		else if (devkit == FW(0x660))
			patch[u] = patch_table[u].patch660;
	}
}