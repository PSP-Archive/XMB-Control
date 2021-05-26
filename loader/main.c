/*
	XmbControl
	Copyright (C) 2011, Total_Noob
	Copyright (C) 2011, Frostegater
	Copyright (C) 2011, codestation

	main.c: main code of loader
	
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
#include <string.h>
#include <systemctrl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Files */
#include <ixmbctrlme.h>
#include <ixmbctrlpro.h>
#include <xmbctrlme.h>
#include <xmbctrlpro.h>
#include <spoofer.h>

/* minIni */
#include <minIni.h>
#include <minGlue.h>
#include <logger.h>

PSP_MODULE_INFO("xmbctrl_loader", 0x1006, 1, 5);

#define FW(f) ((((f >> 8) & 0xF) << 24) | (((f >> 4) & 0xF) << 16) | ((f & 0xF) << 8) | 0x10)
#define MAKE_JUMP(a, f) _sw(0x08000000 | (((u32)(f) & 0x0ffffffc) >> 2), a); 
#define REDIRECT_FUNCTION(a, f) { u32 address = a; _sw(0x08000000 | (((u32)(f) >> 2)  & 0x03ffffff), address);  _sw(0, address+4); }

char ini_path[128];
int devkit;
u32 *start_module_addr;
u32 start_module[3];
int (* StartModule)(SceUID modid, SceSize argsize, void *argp, int *status, SceKernelSMOption *option) = (void *)start_module;
int set[2];

void setlog(void *msg)
{
	char *path = "flash1:/device.xmb";
	sceIoRemove(path);
	SceUID fd = sceIoOpen(path, PSP_O_WRONLY | PSP_O_CREAT, 0777);
	if(fd > 0)
		sceIoWrite(fd, (char *)msg, strlen((char *)msg));

	sceIoClose(fd);
}

int cfg(char *category, char *fmt)
{
	return ini_getlhex(category, fmt, 0, ini_path);
}

void ClearCaches()
{
	sceKernelDcacheWritebackAll();
	sceKernelIcacheClearAll();
}

int LoadModuleBuffer(void *buf, int size)
{
	SceUID modid = sceKernelLoadModuleBuffer((void *)size, (SceSize)buf, 0, NULL);
	
	if(modid < 0)
		return modid;

	void (* pspKernelIcacheClearAll)(void) = (void *)(((_lw((u32)sceKernelIcacheClearAll) & ~0x08000000) << 2) | 0x80000000);

	return StartModule(modid, sizeof(pspKernelIcacheClearAll), &pspKernelIcacheClearAll, NULL, NULL);
}

int StartModulePatched(SceUID modid, SceSize argsize, void *argp, int *status, SceKernelSMOption *option)
{
	int henid = sctrlHENGetVersion();
	
	if(henid == 0x00001000 && (devkit == FW(0x639) || devkit == FW(0x660)))
	{
		if(set[0])
			LoadModuleBuffer(ixmbctrlme, size_ixmbctrlme);
		else
			LoadModuleBuffer(xmbctrlme, size_xmbctrlme);
	}
	else if(devkit >= FW(0x620) && devkit <= FW(0x660))
	{
		if(set[1])
			LoadModuleBuffer(ixmbctrlpro, size_ixmbctrlpro);
		else
			LoadModuleBuffer(xmbctrlpro, size_xmbctrlpro);
	}
	else
		return -1;
		
	LoadModuleBuffer(spoofer, size_spoofer);

	ClearCaches();

	return StartModule(modid, argsize, argp, status, option);
}

int module_start(SceSize args, void *argp)
{
	/* Make ini Path */
	strcpy(ini_path, argp);
	strrchr(ini_path, '/')[1] = 0;
	strcpy(ini_path + strlen(ini_path), "xmbctrl.ini");
	
	set[0] = cfg("XMBControl", "USE_NEW_ITEM_IN_ME_XMBCONTROL");
	set[1] = cfg("XMBControl", "USE_NEW_ITEM_IN_PRO_XMBCONTROL");

	int nid = 0x3FF74DF1; // FW 6.60
	devkit = sceKernelDevkitVersion();
	
	setlog(argp);
	
	if(devkit == FW(0x620))
		nid = 0xDF8FFFAB;
	else if(devkit == FW(0x635) || devkit == FW(0x639))
		nid = 0xE6BF3960;

	if(sceKernelInitKeyConfig() == PSP_INIT_KEYCONFIG_VSH)
	{
		start_module_addr = (u32 *)FindProc("sceModuleManager", "ModuleMgrForKernel", nid);
		
		if(start_module_addr)
		{
			start_module[0] = start_module_addr[0];
			MAKE_JUMP((u32)&start_module[1], &start_module_addr[2]);
			start_module[2] = start_module_addr[1];
			REDIRECT_FUNCTION((u32)start_module_addr, StartModulePatched);
			
			ClearCaches();

			return 0;
		}
	}
	
	return 1;
}