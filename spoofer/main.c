/*
	Spoofer for XmbControl
	Copyright (C) 2011, Total_Noob
	Copyright (C) 2011, Frostegater
	Copyright (C) 2011, codestation
	Copyright (C) 2011, plum

	main.c: main code of spoofer for XmbControl
	
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

#include <pspkernel.h>
#include <systemctrl.h>
#include <psputilsforkernel.h>
#include <systemctrl.h>
#include <psppower.h>
#include <psprtc.h>
#include <stdio.h>
#include <string.h>

PSP_MODULE_INFO("spoofer_for_xmbctrl", 0x0007, 1, 5);

#define MAKE_CALL(a, f) _sw(0x0C000000 | (((u32)(f) >> 2) & 0x03FFFFFF), a);

STMOD_HANDLER previous;
int mac_flag;
int ver_flag;
int henid;

wchar_t *mac_buf;
wchar_t *ver_buf;
void *malloc_p(SceSize size);
int CheckEof(SceUID fd);

#define memcpy my_memcpy

void (*scePafFormatText)(void *a0, void *a1, void *a2, void *s3, void *t0, void *t1, void *t2, void *t3);

void *my_memcpy(void *dest, const void *src, int n)
{
	char *s1;
	char *s2;

	for(s1 = (char *)dest, s2 = (char *)src; n > 0; s1++, s2++, n--)
		*s1 = *s2;

	return dest;
}

wchar_t *wcscpy(wchar_t *dest, const wchar_t *src)
{
	wchar_t *s1;
	wchar_t *s2;

	if(!dest || !src)
		return NULL;

	for(s1 = (wchar_t *)dest, s2 = (wchar_t *)src; (*s1 = *s2) != L'\0'; s1++, s2++);
	return dest;
}

void *malloc_p(SceSize size)
{
	int memid;
	void *buf;

	if(size <= 0)
		size = 1;

	memid = sceKernelAllocPartitionMemory(2, "block", 0, size + 4, NULL);

	if(memid < 0)
		return NULL;

	buf = sceKernelGetBlockHeadAddr(memid);

	if(buf == NULL)
		return NULL;

	my_memcpy(buf, &memid, 4);
	return buf + 4;
}

int CheckEof(SceUID fd)
{
	wchar_t ch;

	if(sceIoRead(fd, &ch, 2) == 2)
	{
		sceIoLseek(fd, -2, PSP_SEEK_CUR);
		return 0;
	}

	return 1;
}

int ReadTxt(const char *file, int flag)
{
	SceUID fd;
	int i, size;
	u16 bom;
	wchar_t *ptr = NULL;

	if(flag > 1)
		return 0;

	fd = sceIoOpen(file, PSP_O_RDONLY, 0777);
	if(fd < 0)
		return 0;

	if(CheckEof(fd) != 0)
		goto READ_ERROR;

	size = sceIoLseek(fd, 0, PSP_SEEK_END);
	sceIoLseek(fd, 0, PSP_SEEK_SET);

	sceIoRead(fd, &bom, sizeof(u16));
	if(bom != 0xFEFF)
		goto READ_ERROR;

	size -= sizeof(u16);
	size /= 2;

	if(flag == 0)
	{
		mac_buf = malloc_p(size + 2);
		if(!mac_buf)
			goto READ_ERROR;

		ptr = mac_buf;
	}
	else if(flag == 1)
	{
		ver_buf = malloc_p(size + 2);
		if(!ver_buf)
			goto READ_ERROR;

		ptr = ver_buf;
	}

	for(i = 0; i < size; i++)
		sceIoRead(fd, &ptr[i], 2);

	ptr[size] = L'\0';

	sceIoClose(fd);
	return 1;

READ_ERROR:

	sceIoClose(fd);
	return 0;
}

int sce_paf_private_swprintfPatched(wchar_t *buf, int n, const wchar_t *format, ...)
{
	wcscpy(buf, mac_buf);
	return 0;
}

void scePafFormatTextPatched(void *a0, void *a1, void *a2, void *a3, void *t0, void *t1, void *t2, void *t3)
{
	scePafFormatText(a0, ver_buf, a2, a3, t0, t1, t2, t3);
	return;
}

void PatchMacAddressText(SceModule2 *mod, int flag)
{
	u32 text_addr = mod->text_addr;
	u32 text_end = mod->text_addr + mod->text_size;

	if(flag == 0)
		return;

	for(; text_addr < text_end; text_addr += 4)
	{
		if(_lw(text_addr) == 0x24070006)
		{
			text_addr += 0x30;
			break;
		}
	}

	MAKE_CALL(text_addr, sce_paf_private_swprintfPatched);
	return;
}

void PatchFWVersionText(SceModule2 *mod, int flag)
{
	u32 text_addr = mod->text_addr;
	u32 text_end = mod->text_addr + mod->text_size;

	if(flag == 0)
		return;

	for(; text_addr < text_end; text_addr += 4)
	{
		if(_lw(text_addr) == 0x34C600C9 && _lw(text_addr + 8) == 0x00000000)
		{
			text_addr += 0x48;
			break;
		}
	}

	MAKE_CALL(text_addr, scePafFormatTextPatched);
	return;
}


void get_scePafFormatText(SceModule2 *mod)
{
	u32 text_addr = mod->text_addr;
	u32 text_end = mod->text_addr + mod->text_size;

	for(; text_addr < text_end; text_end -= 4)
	{
		if(_lw(text_end) == 0x8FA30064)
		{
			text_end -= 0x14;
			break;
		}
	}

	scePafFormatText = (void *)text_end;
	return;
}

void kuKernelIcacheInvalidateAll(void)
{
	int k1 = pspSdkSetK1(0);
	sceKernelIcacheInvalidateAll();
	pspSdkSetK1(k1);
}

void ClearCaches(void)
{
	sceKernelDcacheWritebackAll();
	kuKernelIcacheInvalidateAll();
}

int OnModuleStart(SceModule2 *mod)
{
	if(strcmp(mod->modname, "scePaf_Module") == 0)
		get_scePafFormatText(mod);
	else if(strcmp(mod->modname, "sysconf_plugin_module") == 0)
	{
		char out[256];
		SceUID fd = sceIoOpen("flash1:/config.xmb", PSP_O_RDONLY, 777);
		if(fd > 0)
		{
			int size = sceIoLseek(fd, 0, PSP_SEEK_END);
			sceIoLseek(fd, 0, PSP_SEEK_SET);
			sceIoRead(fd, &out, size);
		}
		sceIoClose(fd);
	
		int veri = 0, maci = 0;
		if(henid != 0x00001000)
		{
			if(out[0x08] == 1)
				veri = 1;
				
			char outse[256];
			fd = sceIoOpen("flash1:/config.se", PSP_O_RDONLY, 777);
			if(fd > 0)
			{
				int size = sceIoLseek(fd, 0, PSP_SEEK_END);
				sceIoLseek(fd, 0, PSP_SEEK_SET);
				sceIoRead(fd, &outse, size);
			}
			sceIoClose(fd);
			
			if(outse[0x14] == 2)
				maci = 1;
		}
		else
		{
			if(out[0x18] == 1)
				veri = 1;
				
			char outme[256];
			fd = sceIoOpen("flash1:/config.me", PSP_O_RDONLY, 777);
			if(fd > 0)
			{
				int size = sceIoLseek(fd, 0, PSP_SEEK_END);
				sceIoLseek(fd, 0, PSP_SEEK_SET);
				sceIoRead(fd, &outme, size);
			}
			sceIoClose(fd);
			
			if(outme[0x0C] == 2)
				maci = 1;
		}
		
		if(maci)
			PatchMacAddressText(mod, mac_flag);
		if(veri)
			PatchFWVersionText(mod, ver_flag);

		ClearCaches();
	}

	return previous ? previous(mod) : 0;
}

int current_device()
{
	char out[256];
	SceUID fd = sceIoOpen("flash1:/device.xmb", PSP_O_RDONLY, 777);
	if(fd > 0)
	{
		int size = sceIoLseek(fd, 0, PSP_SEEK_END);
		sceIoLseek(fd, 0, PSP_SEEK_SET);
		sceIoRead(fd, &out, size);
	}
	sceIoClose(fd);
	
	if(out[0] == 'm')
		return 0;
	else if(out[0] == 'e')
		return 1;
		
	return -1;
}

int module_start(SceSize args, void *argp)
{
	if(current_device() < 0)
		return -1;

	char mac[512], ver[512];
	henid = sctrlHENGetVersion();
	
	sprintf(mac, "%s0:/seplugins/xmbcontrol/sys_mac_address.txt", current_device() == 1 ? "ef" : "ms");
	sprintf(ver, "%s0:/seplugins/xmbcontrol/sys_version.txt", current_device() == 1 ? "ef" : "ms");

	mac_flag = ReadTxt(mac, 0);
	ver_flag = ReadTxt(ver, 1);

	previous = sctrlHENSetStartModuleHandler(OnModuleStart);

	return 0;
}