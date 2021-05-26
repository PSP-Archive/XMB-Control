/*
	XmbControl Inslaller on VLFlib
	Copyright (C) 2011, Dark_Alex
	Copyright (C) 2011, Frostegater

	crt0.c: XmbControl installer base code
	
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
#include <stdio.h>

#include "vlf/vlf_prx.h"
#include "vlf/intra.h"
#include "vlf/iop.h"
#include <vlf.h>
#include "main.h"

extern int app_main(int argc, char *argv[]);
extern int WriteFile(char *file, void *buf, int size, int seek);
int currdev;

void LoadStartModuleBuffer(char *path_buf, char *buf, int size)
{
	SceUID mod, out;

	sceIoRemove(path_buf);
	out = sceIoOpen(path_buf, PSP_O_WRONLY|PSP_O_CREAT, 0777);
	sceIoWrite(out, buf, size);
	sceIoClose(out);

	mod = sceKernelLoadModule(path_buf, 0, NULL);
	mod = sceKernelStartModule(mod, 0, NULL, NULL, NULL);
}

char *cpath;

void del_external_modules()
{
	sceIoRemove("intraFont.prx");
	sceIoRemove("vlf.prx");
	sceIoRemove("iop.prx");
}

int current_device(void *argp)
{
	char *currpath = (char *)argp;

	switch(currpath[0])
	{
		case 'm': return MEMORY_STICK;	
		case 'e': return EXTENDED_FLASH;
		case 'f':
			if(currpath[5] == '0') return FLASH0;
			else if(currpath[5] == '1') return FLASH1;
			else if(currpath[5] == '2') return FLASH2;
			else if(currpath[5] == '3') return FLASH3;

		default:
			break;
	}

	return UNKNOWN_DEVICE;
}

int start_thread(SceSize args, void *argp)
{
	currdev = current_device(argp);

	cpath = (char *)argp;
	int last_trail = -1;
	int i;

	if(cpath)
		for (i = 0; cpath[i]; i++)
		{
			if (cpath[i] == '/')
				last_trail = i;
		}

	if(last_trail >= 0)
		cpath[last_trail] = 0;

	sceIoChdir(cpath);

	LoadStartModuleBuffer("intraFont.prx", (void *)intra, size_intra);
	LoadStartModuleBuffer("vlf.prx", (void *)vlf_prx, size_vlf_prx);
	LoadStartModuleBuffer("iop.prx", (void *)iop, size_iop);
		
	del_external_modules();
		
	vlfGuiInit(15000, app_main);
		
	return sceKernelExitDeleteThread(0);
}

int module_start(SceSize args, void *argp)
{
	SceUID thid = sceKernelCreateThread("start_thread", start_thread, 0x10, 0x4000, 0, NULL);
	if (thid < 0)
		return thid;

	sceKernelStartThread(thid, args, argp);
		
	return 0;
}