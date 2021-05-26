/*
	XmbControl
	Copyright (C) 2011, Total_Noob
	Copyright (C) 2011, Frostegater
	Copyright (C) 2011, codestation

	main.c: XmbControl main code
	
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
#include <nidresolver.h>
#include <main.h>
#include <utils.h>
#include <en_tnsettings.h>
#include <string.h>
#include <stdio.h>

/* Include translates */
#include <ja_translate.h>
#include <en_translate.h>
#include <fr_translate.h>
#include <es_translate.h>
#include <de_translate.h>
#include <it_translate.h>
#include <nl_translate.h>
#include <pt_translate.h>
#include <ru_translate.h>
#include <ko_translate.h>
#include <ch1_translate.h>
#include <ch2_translate.h>

typedef struct
{
	char *name;
	char *on_off[3];
	char *items[5];
	char *options[18];
	char *options_sub[18];
	char *isodriver[5];
	char *usbdevice[6];
	char *regions[14];
	char *msspeedup[8];
	char *buttonassign;
	char *buttonassign_sub;
	char *advanced[9];
	char *advanced_sub[9];
	char *cpuspeed[2];
	char *cpuspeed_sub[2];
	char *speed[11];
} StringContainer;

StringContainer string;

#define N_STRINGS ((sizeof(string) / sizeof(char **)))

typedef struct
{
	int mode;
	int negative;
	char *item;
	char *sub;
} GetItem;

GetItem GetItemes[] =
{
	{ 7, 0, "msg_isodriver", "msg_isodriver_sub" },
	{ 2, 0, "msg_usbdevice", "msg_usbdevice_sub" },
	{ 3, 0, "msg_fakeregion", "msg_fakeregion_sub" },
	{ 8, 0, "msg_skipcoldboot", "msg_skipcoldboot_sub" },
	{ 8, 0, "msg_skipgameboot", "msg_skipgameboot_sub" },
	{ 8, 0, "msg_spoofversion", "msg_spoofversion_sub" },
	{ 9, 0, "msg_spoofmacaddr", "msg_spoofmacaddr_sub" },
	{ 8, 1, "msg_flashprotect", "msg_flashprotect_sub" },
	{ 8, 0, "msg_hidepic0pic1", "msg_hidepic0pic1_sub" },
	{ 8, 0, "msg_hidecorrupticons", "msg_hidecorrupticons_sub" },
	{ 8, 1, "msg_usenetupd", "msg_usenetupd_sub" },
	{ 8, 0, "msg_autoruneboot", "msg_autoruneboot_sub" },
	{ 8, 0, "msg_umdvideopatch", "msg_umdvideopatch_sub" },
	{ 8, 0, "msg_slimcolors", "msg_slimcolors_sub" },
	{ 8, 0, "msg_usbcharge", "msg_usbcharge_sub" },
	{ 8, 1, "msg_usevshmenu", "msg_usevshmenu_sub" },
	{ 8, 0, "msg_useversion", "msg_useversion_sub" },
	{ 6, 0, "msg_msspeedup", "msg_msspeedup_sub" },
};

GetItem GetItemes2[] =
{
	{ 8, 1, "msg_opnssmp", "msg_opnssmp_sub" },
	{ 8, 0, "msg_pboot", "msg_pboot_sub" },
	{ 8, 0, "msg_passwordcontrol", "msg_passwordcontrol_sub" },
	{ 8, 1, "msg_xmbplugins", "msg_xmbplugins_sub" },
	{ 8, 1, "msg_gameplugins", "msg_gameplugins_sub" },
	{ 8, 1, "msg_popsplugins", "msg_popsplugins_sub" },
	{ 8, 0, "msg_osklimitincrease", "msg_osklimitincrease_sub" },
	{ 8, 0, "msg_extendedosk", "msg_extendedosk_sub" },
	{ 8, 0, "msg_hideumdupdate", "msg_hideumdupdate_sub" },
};

GetItem GetItemes3[] =
{
	{ 1, 0, "msg_cpuclockxmb", "msg_cpuclockxmb_sub" },
	{ 1, 0, "msg_cpuclockgame", "msg_cpuclockgame_sub" },
};

#define N_ITEMS (sizeof(GetItemes) / sizeof(GetItem))
#define N_ITEMS2 (sizeof(GetItemes2) / sizeof(GetItem))
#define N_ITEMS3 (sizeof(GetItemes3) / sizeof(GetItem))

char *button_options[] = { "O", "X" };

char *items[] =
{
	"msgtop_sysconf_configuration",
	"msgtop_sysconf_advanced",
	"msgtop_sysconf_cpu",
	"msgtop_sysconf_standart",
	"msgtop_sysconf_plugins",
};

enum PluginMode
{
	PluginModeVSH = 0,
	PluginModeGAME = 1,
	PluginModePOPS
};

char *plugin_texts_ms0[] =
{
	"ms0:/seplugins/vsh.txt",
	"ms0:/seplugins/game.txt",
	"ms0:/seplugins/pops.txt"
};

char *plugin_texts_ef0[] =
{
	"ef0:/seplugins/vsh.txt",
	"ef0:/seplugins/game.txt",
	"ef0:/seplugins/pops.txt"
};

#define MAX_PLUGINS_CONST 100

typedef struct
{
	char *path;
	char *plugin;
	int activated;
	int mode;
} Plugin;

Plugin *table = NULL;
int count = 0;

SceUID block_id;
wchar_t *copied_string = NULL;

int (* ReleaseText)();
int (* WriteLetter)(int sel);
int (* AddVshItem)(void *a0, int topitem, SceVshItem *item);
SceSysconfItem *(*GetSysconfItem)(void *a0, void *a1);
int (* ExecuteAction)(int action, int action_arg);
int (* UnloadModule)(int skip);
int (* OnXmbPush)(void *arg0, void *arg1);
int (* OnXmbContextMenu)(void *arg0, void *arg1);

void (* LoadStartAuth)();
int (* auth_handler)(int a0);
void (* OnRetry)();

void (* AddSysconfItem)(u32 *option, SceSysconfItem **item);
void (* OnInitMenuPspConfig)();

int(* umdIoOpen)(PspIoDrvFileArg* arg, const char* file, int flags, SceMode mode);

extern int GetPlugin(char *buf, int size, char *str, int *activated);
extern int ReadLine(SceUID fd, char *str);
extern int utf8_to_unicode(wchar_t *dest, char *src);

u32 sysconf_unk, sysconf_option;

int is_tn_config = 0;
int unload = 0;

u32 backup[4];
int context_mode = 0;

int num2, useumdvideo;

char user_buffer[128];

int len_array;
int param_array;

int osk_param;

STMOD_HANDLER previous;
TNConfig config;
Config2 config2;

int psp_model, i;

int startup = 1;

SceContextItem *context;
SceVshItem *new_item;
void *xmb_arg0, *xmb_arg1;

int cpu_list[] = { 0, 20, 75, 100, 133, 166, 200, 222, 266, 300, 333 };
int bus_list[] = { 0, 10, 37, 50, 66, 83, 100, 111, 133, 150, 166 };

#define N_CPUS (sizeof(cpu_list) / sizeof(int))
#define N_BUSS (sizeof(bus_list) / sizeof(int))

void ClearCaches()
{
	sceKernelDcacheWritebackAll();
	kuKernelIcacheInvalidateAll();
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

int flash_file(char *file, void *file_name,  int file_size)
{
	int written;
	SceUID fd;
    fd = sceIoOpen(file, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC , 511 );
    written = sceIoWrite(fd, file_name, file_size);
    sceIoClose(fd);

    return 0;
}

__attribute__((noinline)) int GetCPUSpeed(int cpu)
{
	int i;
	for(i = 0; i < N_CPUS; i++) if(cpu_list[i] == cpu) return i;
	return 0;
}

__attribute__((noinline)) int GetBUSSpeed(int bus)
{
	int i;
	for(i = 0; i < N_BUSS; i++) if(bus_list[i] == bus) return i;
	return 0;
}

int SkipItems(int i)
{
	if(!((psp_model == 4 &&  i == 11) || (psp_model != 4 && i == 12) || (psp_model &&  i == 13) || (!psp_model &&  i == 14)))
		return 1;
		
	return 0;
}

int readPlugins(int mode, int cur_n, Plugin *plugin_table)
{
	char temp_path[64];
	int res = 0, i = cur_n;
	
	SceUID fd = sceIoOpen(plugin_texts_ms0[mode], PSP_O_RDONLY, 0);
	if(current_device() == 1)
	{
		fd = sceIoOpen(plugin_texts_ef0[mode], PSP_O_RDONLY, 0);
		if(fd < 0) return i;
	}

	char *buffer = (char *)sce_paf_private_malloc(1024);
	int size = sceIoRead(fd, buffer, 1024);
	char *p = buffer;

	do
	{
		sce_paf_private_memset(temp_path, 0, sizeof(temp_path));

		res = GetPlugin(p, size, temp_path, &plugin_table[i].activated);
		if(res > 0)
		{
			if(plugin_table[i].path) sce_paf_private_free(plugin_table[i].path);
			plugin_table[i].path = (char *)sce_paf_private_malloc(sce_paf_private_strlen(temp_path) + 1);
			sce_paf_private_strcpy(plugin_table[i].path, temp_path);

			if(plugin_table[i].plugin) sce_paf_private_free(plugin_table[i].plugin);
			table[i].plugin = (char *)sce_paf_private_malloc(64);
			sce_paf_private_sprintf(table[i].plugin, "plugin_%08X", (u32)i);

			plugin_table[i].mode = mode;

			size -= res;
			p += res;
			i++;
		}
	} while(res > 0);

	sce_paf_private_free(buffer);
	sceIoClose(fd);

	return i;
}

void writePlugins(int mode, Plugin *plugin_table, int count)
{	
	SceUID fd = sceIoOpen(plugin_texts_ms0[mode], PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	if(current_device() == 1) fd = sceIoOpen(plugin_texts_ef0[mode], PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);

	if(fd >= 0)
	{
		int i;
		for(i = 0; i < count; i++)
			if(plugin_table[i].mode == mode)
			{
				sceIoWrite(fd, plugin_table[i].path, sce_paf_private_strlen(plugin_table[i].path));
				sceIoWrite(fd, " ", sizeof(char));

				sceIoWrite(fd, (plugin_table[i].activated == 1 ? "1" : "0"), sizeof(char));

				if(i != (count - 1)) sceIoWrite(fd, "\r\n", 2 * sizeof(char));
			}
		
		sceIoClose(fd);
	}
}

int readPluginTable()
{
	if(!table)
	{
		table = sce_paf_private_malloc(MAX_PLUGINS_CONST * sizeof(Plugin));
		sce_paf_private_memset(table, 0, MAX_PLUGINS_CONST * sizeof(Plugin));
	}

	count = readPlugins(PluginModeVSH, 0, table);
	count = readPlugins(PluginModeGAME, count, table);
	count = readPlugins(PluginModePOPS, count, table);

	if(count <= 0)
	{
		if(table)
		{
			sce_paf_private_free(table);
			table = NULL;
		}

		return 0;
	}

	return 1;
}

int LoadTextLanguage(int new_id)
{
	static char *language[] = { "ja", "en", "fr", "es", "de", "it", "nl", "pt", "ru", "ko", "ch1", "ch2" };

	int id;
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &id);

	if(new_id >= 0)
	{
		if(new_id == id) return 0;
		id = new_id;
	}

	char file[256];
	sce_paf_private_sprintf(file, "%s0:/seplugins/xmbcontrol/mecfw_translates/%s_mesettings.txt", current_device() == 1 ? "ef" : "ms", language[id]);

	SceUID fd = sceIoOpen(file, PSP_O_RDONLY, 777);

	sprintf(file, "flash1:/%s_mesettings.txt", language[id]);
	
	if(fd < 0)
	{
		switch(id)
		{
			case 0:
				flash_file(file, ja_translate, size_ja_translate);
				break;
				
			case 1:
				flash_file(file, en_translate, size_en_translate);
				break;
				
			case 2:
				flash_file(file, fr_translate, size_fr_translate);
				break;
				
			case 3:
				flash_file(file, es_translate, size_es_translate);
				break;
				
			case 4:
				flash_file(file, de_translate, size_de_translate);
				break;
				
			case 5:
				flash_file(file, it_translate, size_it_translate);
				break;
				
			case 6:
				flash_file(file, nl_translate, size_nl_translate);
				break;
				
			case 7:
				flash_file(file, pt_translate, size_pt_translate);
				break;
				
			case 8:
				flash_file(file, ru_translate, size_ru_translate);
				break;
				
			case 9:
				flash_file(file, ko_translate, size_ko_translate);
				break;
				
			case 10:
				flash_file(file, ch1_translate, size_ch1_translate);
				break;
				
			case 11:
				flash_file(file, ch2_translate, size_ch2_translate);
				break;
		}
		
		fd = sceIoOpen(file, PSP_O_RDONLY, 777);
	}

	if(fd >= 0)
	{
		/* Skip UTF8 magic */
		u32 magic;
		sceIoRead(fd, &magic, sizeof(magic));
		sceIoLseek(fd, (magic & 0xFFFFFF) == 0xBFBBEF ? 3 : 0, PSP_SEEK_SET);
	}

	char line[2048];

	int i;
	int j = 0;
	for(i = 0; i < N_STRINGS; i++)
	{
		sce_paf_private_memset(line, 0, sizeof(line));

		if(&((char **)&string)[i] >= &string.speed[1] && &((char **)&string)[i] <= &string.speed[N_CPUS])
			sce_paf_private_sprintf(line, "%d/%d", cpu_list[&((char **)&string)[i] - &*string.speed], bus_list[&((char **)&string)[i] - &*string.speed]);
		else if(&((char **)&string)[i] >= &string.usbdevice[1] && &((char **)&string)[i] <= &string.usbdevice[4])
			sce_paf_private_sprintf(line, "Flash %d", &((char **)&string)[i] - &*string.usbdevice - 1);
		else
		{
			if(fd >= 0)
				ReadLine(fd, line);
			else
			{
				sce_paf_private_strcpy(line, en_tnsettings[j]);
				j++;
			}
		}

		if(((char **)&string)[i]) sce_paf_private_free(((char **)&string)[i]);

		((char **)&string)[i] = sce_paf_private_malloc(sce_paf_private_strlen(line) + 1);

		sce_paf_private_strcpy(((char **)&string)[i], line);
	}

	if(fd >= 0) sceIoClose(fd);
	
	sceIoRemove(file);
	
	return 1;
}

int AddVshItemPatched(void *a0, int topitem, SceVshItem *item)
{
	if(sce_paf_private_strcmp(item->text, "msgtop_sysconf_console") == 0)
	{
		if(config2.passwordcontrol)
			LoadStartAuth();
		else
			startup = 0;
		
		LoadTextLanguage(-1);

		new_item = (SceVshItem *)sce_paf_private_malloc(sizeof(SceVshItem));
		sce_paf_private_memcpy(new_item, item, sizeof(SceVshItem));

		new_item->id = patch[33]; //information board id (last item id)
		new_item->action_arg = sysconf_tnconfig_action_arg;
		new_item->play_sound = 0;
#ifdef ITEM
		sce_paf_private_strcpy(new_item->text, "msgtop_sysconf_tnconfig");
#endif

		context = (SceContextItem *)sce_paf_private_malloc((5 * sizeof(SceContextItem)) + 1);

		AddVshItem(a0, topitem, new_item);
	}
#ifndef ITEM
	else
		return AddVshItem(a0, topitem, item);
#else
	return AddVshItem(a0, topitem, item);
#endif
	
	return 0;
}

int OnXmbPushPatched(void *arg0, void *arg1)
{
	xmb_arg0 = arg0;
	xmb_arg1 = arg1;
	return OnXmbPush(arg0, arg1);
}

int OnXmbContextMenuPatched(void *arg0, void *arg1)
{
	new_item->context = NULL;
	return OnXmbContextMenu(arg0, arg1);
}

int ExecuteActionPatched(int action, int action_arg)
{
	int old_is_tn_config = is_tn_config;

	if(action == sysconf_console_action)
	{
		if(action_arg == sysconf_tnconfig_action_arg)
		{
			sctrlSEGetConfig(&config);
			int n = readPluginTable() + 
#ifndef ITEM		
			4;
#else
			3;
#endif

			sce_paf_private_memset(context, 0, (5 * sizeof(SceContextItem)) + 1);

			int i;
			for(i = 0; i < n; i++)
			{
#ifdef ITEM
				sce_paf_private_strcpy(context[i].text, items[i == 3 ? i + 1 : i]);
#else
				sce_paf_private_strcpy(context[i].text, items[i]);
#endif
				context[i].play_sound = 1;
				context[i].action = 0x80000;
				context[i].action_arg = i + 1;
			}

			new_item->context = context;

			OnXmbContextMenu(xmb_arg0, xmb_arg1);
			return 0;
		}

		is_tn_config = 0;
	}
	else if(action == 0x80000)
	{
		is_tn_config = action_arg;
		action = sysconf_console_action;
		action_arg = sysconf_console_action_arg;
	}

	if(old_is_tn_config != is_tn_config)
	{
		sce_paf_private_memset(backup, 0, sizeof(backup));
		context_mode = 0;

		unload = 1;
	}

	return ExecuteAction(action, action_arg);
}

int UnloadModulePatched(int skip)
{
	if(unload)
	{
		skip = -1;
		unload = 0;
	}
	return UnloadModule(skip);
}

void AddSysconfContextItem(char *text, char *subtitle, char *regkey)
{
	SceSysconfItem *item = (SceSysconfItem *)sce_paf_private_malloc(sizeof(SceSysconfItem));

	item->id = 5;
	item->unk = (u32 *)sysconf_unk;
	item->regkey = regkey;
	item->text = text;
	item->subtitle = subtitle;
	item->page = "page_psp_config_umd_autoboot";

	((u32 *)sysconf_option)[2] = 1;

	AddSysconfItem((u32 *)sysconf_option, &item);
}

void OnInitMenuPspConfigPatched()
{
	if(is_tn_config == 1)
	{
		if(((u32 *)sysconf_option)[2] == 0)
		{
			int i;
			for(i = 0; i < N_ITEMS; i++)
				if(SkipItems(i))
					AddSysconfContextItem(GetItemes[i].item, GetItemes[i].sub, GetItemes[i].item);
			
			AddSysconfContextItem("msg_buttonassign", "msg_buttonassign_sub", "/CONFIG/SYSTEM/XMB/button_assign");
		}
	}
	else if(is_tn_config == 2)
	{
		if(((u32 *)sysconf_option)[2] == 0)
		{
			int i;
			for(i = 0; i < num2; i++)
				AddSysconfContextItem(GetItemes2[i].item, GetItemes2[i].sub, GetItemes2[i].item);
		}
	}
	else if(is_tn_config == 3)
	{
		if(((u32 *)sysconf_option)[2] == 0)
		{
			int i;
			for(i = 0; i < N_ITEMS3; i++)
				AddSysconfContextItem(GetItemes3[i].item, GetItemes3[i].sub, GetItemes3[i].item);
		}
	}
#ifndef ITEM
	else if(is_tn_config == 5)
#else
	else if(is_tn_config == 4)
#endif
	{
		if(((u32 *)sysconf_option)[2] == 0)
		{
			int i;
			for(i = 0; i < count; i++)
			{
				char *subtitle = NULL;
				switch(table[i].mode)
				{
					case PluginModeVSH:
						subtitle = "msg_pluginmode_vsh";
						break;

					case PluginModeGAME:
						subtitle = "msg_pluginmode_game";
						break;

					case PluginModePOPS:
						subtitle = "msg_pluginmode_pops";
						break;
				}

				AddSysconfContextItem(table[i].plugin, subtitle, table[i].plugin);
			}
		}
	}
	else
		OnInitMenuPspConfig();
}

SceSysconfItem *GetSysconfItemPatched(void *a0, void *a1)
{
	SceSysconfItem *item = GetSysconfItem(a0, a1);

	if(is_tn_config == 1)
	{
		int i;
		for(i = 0; i < N_ITEMS; i++)
			if(sce_paf_private_strcmp(item->text, GetItemes[i].item) == 0)
				if(SkipItems(i))
					context_mode = GetItemes[i].mode;

		if(sce_paf_private_strcmp(item->text, "msg_buttonassign") == 0)
			context_mode = 4;
		else if(sce_paf_private_strcmp(item->text, "msg_gamefolder") == 0)
			context_mode = 5;
	}
	else if(is_tn_config == 2)
	{
		int i;
		for(i = 0; i < num2; i++)
			if(sce_paf_private_strcmp(item->text, GetItemes2[i].item) == 0)
				context_mode = GetItemes2[i].mode;
	}
	else if(is_tn_config == 3)
	{
		int i;
		for(i = 0; i < N_ITEMS3; i++)
			if(sce_paf_private_strcmp(item->text, GetItemes3[i].item) == 0)
				context_mode = GetItemes3[i].mode;
	}
#ifndef ITEM
	else if(is_tn_config == 5)
#else
	else if(is_tn_config == 4)
#endif
		context_mode = 8;

	return item;
}

wchar_t *scePafGetTextPatched(void *a0, char *name)
{
	if(name)
	{
		if(is_tn_config == 1)
		{
			int i;
			for(i = 0; i < N_ITEMS; i++)
			{
				if(sce_paf_private_strcmp(name, GetItemes[i].item) == 0)
				{
					utf8_to_unicode((wchar_t *)user_buffer, string.options[i]);
					return (wchar_t *)user_buffer;
				}
				else if(sce_paf_private_strcmp(name, GetItemes[i].sub) == 0)
				{
					utf8_to_unicode((wchar_t *)user_buffer, string.options_sub[i]);
					return (wchar_t *)user_buffer;
				}
			}
		}
		else if(is_tn_config == 2)
		{
			int i;
			for(i = 0; i < N_ITEMS2; i++)
			{
				if(sce_paf_private_strcmp(name, GetItemes2[i].item) == 0)
				{
					utf8_to_unicode((wchar_t *)user_buffer, string.advanced[i]);
					return (wchar_t *)user_buffer;
				}
				else if(sce_paf_private_strcmp(name, GetItemes2[i].sub) == 0)
				{
					utf8_to_unicode((wchar_t *)user_buffer, string.advanced_sub[i]);
					return (wchar_t *)user_buffer;
				}
			}
		}
		else if(is_tn_config == 3)
		{
			int i;
			for(i = 0; i < N_ITEMS3; i++)
			{
				if(sce_paf_private_strcmp(name, GetItemes3[i].item) == 0)
				{
					utf8_to_unicode((wchar_t *)user_buffer, string.cpuspeed[i]);
					return (wchar_t *)user_buffer;
				}
				else if(sce_paf_private_strcmp(name, GetItemes3[i].sub) == 0)
				{
					utf8_to_unicode((wchar_t *)user_buffer, string.cpuspeed_sub[i]);
					return (wchar_t *)user_buffer;
				}
			}
		}
#ifndef ITEM
	else if(is_tn_config == 5)
#else
	else if(is_tn_config == 4)
#endif
		{
			if(sce_paf_private_strncmp(name, "plugin_", 7) == 0)
			{
				u32 i = sce_paf_private_strtoul(name + 7, NULL, 16);

				char file[64];
				sce_paf_private_strcpy(file, table[i].path);

				char *p = sce_paf_private_strrchr(table[i].path, '/');
				if(p)
				{
					char *p2 = sce_paf_private_strchr(p + 1, '.');
					if(p2)
					{
						int len = (int)(p2 - (p + 1));
						sce_paf_private_strncpy(file, p + 1, len);
						file[len] = '\0';
					}
				}

				utf8_to_unicode((wchar_t *)user_buffer, file);
				return (wchar_t *)user_buffer;
			}
		}

		if(sce_paf_private_strcmp(name, "msgtop_sysconf_tnconfig") == 0)
		{
			utf8_to_unicode((wchar_t *)user_buffer, string.name);
			return (wchar_t *)user_buffer;
		}
		else if(sce_paf_private_strcmp(name, "msgtop_sysconf_configuration") == 0)
		{
			utf8_to_unicode((wchar_t *)user_buffer, string.items[0]);
			return (wchar_t *)user_buffer;
		}
		else if(sce_paf_private_strcmp(name, "msgtop_sysconf_advanced") == 0)
		{
			utf8_to_unicode((wchar_t *)user_buffer, string.items[1]);
			return (wchar_t *)user_buffer;
		}
		else if(sce_paf_private_strcmp(name, "msgtop_sysconf_cpu") == 0)
		{
			utf8_to_unicode((wchar_t *)user_buffer, string.items[2]);
			return (wchar_t *)user_buffer;
		}
		else if(sce_paf_private_strcmp(name, "msgtop_sysconf_standart") == 0)
		{
			utf8_to_unicode((wchar_t *)user_buffer, string.items[3]);
			return (wchar_t *)user_buffer;
		}
		else if(sce_paf_private_strcmp(name, "msgtop_sysconf_plugins") == 0)
		{
			utf8_to_unicode((wchar_t *)user_buffer, string.items[4]);
			return (wchar_t *)user_buffer;
		}
		else if(sce_paf_private_strcmp(name, "msg_buttonassign") == 0)
		{
			utf8_to_unicode((wchar_t *)user_buffer, string.buttonassign);
			return (wchar_t *)user_buffer;
		}
		else if(sce_paf_private_strcmp(name, "msg_buttonassign_sub") == 0)
		{
			utf8_to_unicode((wchar_t *)user_buffer, string.buttonassign_sub);
			return (wchar_t *)user_buffer;
		}
		else if(sce_paf_private_strcmp(name, "msg_pluginmode_vsh") == 0)
			return L"[VSH]";
		else if(sce_paf_private_strcmp(name, "msg_pluginmode_game") == 0)
			return L"[GAME]";
		else if(sce_paf_private_strcmp(name, "msg_pluginmode_pops") == 0)
			return L"[POPS]";
	}

	wchar_t *res = scePafGetText(a0, name);

	return res;
}

int vshGetRegistryValuePatched(u32 *option, char *name, void *arg2, int size, int *value)
{
	if(name)
	{
		if(is_tn_config == 1)
		{
			int configs[] =
			{
				config.isodriver,
				config.usbdevice,
				config.fakeregion,
				config.skipcoldboot,
				config.skipgameboot,
				config2.spoofversion,
				config.hidemacaddress,
				!config.flashprotect,
				config.hidepic0pic1,
				config.hidecorrupticons,
				!config.usenetupd,
				config.autoruneboot,
				config.autoruneboot,
				config.slimcolors_usbcharge,
				config.slimcolors_usbcharge,
				!config.usevshmenu,
				config.useversion,
				config.msspeed,
			};
			
			int i;
			for(i = 0; i < N_ITEMS; i++)
			{
				if(sce_paf_private_strcmp(name, GetItemes[i].item) == 0)
				{	
					if(SkipItems(i))
					{
						context_mode = GetItemes[i].mode;
						*value = configs[i];
					}

					return 0;
				}
			}

			if(sce_paf_private_strcmp(name, "/CONFIG/SYSTEM/XMB/button_assign") == 0)
				context_mode = 4;
			else if(sce_paf_private_strcmp(name, "msg_gamefolder") == 0)
				context_mode = 5;
		}
		else if(is_tn_config == 2)
		{
			int configs[] =
			{
				!config.opnssmp,
				config.notpboot,
				config2.passwordcontrol,
				!config.xmbplugins,
				!config.gameplugins,
				!config.popsplugins,
				config2.osklimitincrease,
				config2.extendedosk,
				config2.hideumdupdate,
			};

			int i;
			for(i = 0; i < num2; i++)
			{
				if(sce_paf_private_strcmp(name, GetItemes2[i].item) == 0)
				{
					context_mode = GetItemes2[i].mode;
					*value = configs[i];
					return 0;
				}
			}
		}
		else if(is_tn_config == 3)
		{
			int configs[] =
			{
				GetCPUSpeed(config.vshcpuspeed),
				GetCPUSpeed(config.gamecpuspeed),
			};
			
			int i;
			for(i = 0; i < N_ITEMS3; i++)
			{
				if(sce_paf_private_strcmp(name, GetItemes3[i].item) == 0)
				{
					context_mode = GetItemes3[i].mode;
					*value = configs[i];
					return 0;
				}
				else if(sce_paf_private_strcmp(name, "msg_cpuclockxmb") == 0 || sce_paf_private_strcmp(name, "msg_cpuclockgame") == 0)
					context_mode = 1;
			}
		}
#ifndef ITEM
	else if(is_tn_config == 5)
#else
	else if(is_tn_config == 4)
#endif
		{
			if(sce_paf_private_strncmp(name, "plugin_", 7) == 0)
			{
				u32 i = sce_paf_private_strtoul(name + 7, NULL, 16);
				context_mode = 8;
				*value = table[i].activated;
				return 0;
			}
		}
	}

	return vshGetRegistryValue(option, name, arg2, size, value);
}

int vshSetRegistryValuePatched(u32 *option, char *name, int size, int *value)
{
	if(name)
	{
		if(is_tn_config == 1)
		{
			static int *configs[] =
			{
				&config.isodriver,
				&config.usbdevice,
				&config.fakeregion,
				&config.skipcoldboot,
				&config.skipgameboot,
				&config2.spoofversion,
				&config.hidemacaddress,
				&config.flashprotect,
				&config.hidepic0pic1,
				&config.hidecorrupticons,
				&config.usenetupd,
				&config.autoruneboot,
				&config.autoruneboot,
				&config.slimcolors_usbcharge,
				&config.slimcolors_usbcharge,
				&config.usevshmenu,
				&config.useversion,
				&config.msspeed,
			};

			int i;
			for(i = 0; i < N_ITEMS; i++)
			{
				if(sce_paf_private_strcmp(name, GetItemes[i].item) == 0)
				{
					if(SkipItems(i))
					{
						*configs[i] = GetItemes[i].negative ? !(*value) : *value;
					
						sctrlSESetConfig(&config);
						sctrlSE2SetConfig(&config2);
						vctrlVSHExitVSHMenu(&config);
					}

					return 0;
				}
			}
		}
		else if(is_tn_config == 2)
		{
			static int *configs[] =
			{
				&config.opnssmp,
				&config.notpboot,
				&config2.passwordcontrol,
				&config.xmbplugins,
				&config.gameplugins,
				&config.popsplugins,
				&config2.osklimitincrease,
				&config2.extendedosk,
				&config2.hideumdupdate,
			};

			int i;
			for(i = 0; i < num2; i++)
			{
				if(sce_paf_private_strcmp(name, GetItemes2[i].item) == 0)
				{
					*configs[i] = GetItemes2[i].negative ? !(*value) : *value;

					sctrlSESetConfig(&config);
					sctrlSE2SetConfig(&config2);
					vctrlVSHExitVSHMenu(&config);
					return 0;
				}
			}
		}
		else if(is_tn_config == 3)
		{
			static int *configs[] =
			{
				&config.vshcpuspeed,
				&config.gamecpuspeed,
			};
			
			int i;
			for(i = 0; i < N_ITEMS3; i++)
				if(sce_paf_private_strcmp(name, GetItemes3[i].item) == 0)
				{
					if(sce_paf_private_strcmp(name, "msg_cpuclockxmb") == 0)
					{
						*configs[i] = cpu_list[*value];
						config.vshbusspeed = bus_list[*value];
					}
					else if(sce_paf_private_strcmp(name, "msg_cpuclockgame") == 0)
					{
						*configs[i] = cpu_list[*value];
						config.gamebusspeed = bus_list[*value];
					}

					sctrlSESetConfig(&config);
					vctrlVSHExitVSHMenu(&config);
					return 0;
				}
		}
#ifndef ITEM
	else if(is_tn_config == 5)
#else
	else if(is_tn_config == 4)
#endif
		{
			if(sce_paf_private_strncmp(name, "plugin_", 7) == 0)
			{
				u32 i = sce_paf_private_strtoul(name + 7, NULL, 16);
				table[i].activated = *value;
				writePlugins(table[i].mode, table, count);
				return 0;
			}
		}

		if(sce_paf_private_strcmp(name, "/CONFIG/SYSTEM/XMB/language") == 0)
			LoadTextLanguage(*value);
	}

	return vshSetRegistryValue(option, name, size, value);
}

void HijackContext(SceRcoEntry *src, char **options, int n)
{
	SceRcoEntry *plane = (SceRcoEntry *)((u32)src + src->first_child);
	SceRcoEntry *mlist = (SceRcoEntry *)((u32)plane + plane->first_child);
	u32 *mlist_param = (u32 *)((u32)mlist + mlist->param);

	/* Backup */
	if(backup[0] == 0 && backup[1] == 0 && backup[2] == 0 && backup[3] == 0)
	{
		backup[0] = mlist->first_child;
		backup[1] = mlist->child_count;
		backup[2] = mlist_param[16];
		backup[3] = mlist_param[18];
	}

	if(context_mode)
	{
		SceRcoEntry *base = (SceRcoEntry *)((u32)mlist + mlist->first_child);

		SceRcoEntry *item = (SceRcoEntry *)sce_paf_private_malloc(base->next_entry * n);
		u32 *item_param = (u32 *)((u32)item + base->param);

		mlist->first_child = (u32)item - (u32)mlist;
		mlist->child_count = n;
		mlist_param[16] = 13;
		mlist_param[18] = 6;

		int i;
		for(i = 0; i < n; i++)
		{
			sce_paf_private_memcpy(item, base, base->next_entry);

			item_param[0] = 0xDEAD;
			item_param[1] = (u32)options[i];

			if(i != 0) item->prev_entry = item->next_entry;
			if(i == n - 1) item->next_entry = 0;

			item = (SceRcoEntry *)((u32)item + base->next_entry);
			item_param = (u32 *)((u32)item + base->param);
		}
	}
	else
	{
		/* Restore */
		mlist->first_child = backup[0];
		mlist->child_count = backup[1];
		mlist_param[16] = backup[2];
		mlist_param[18] = backup[3];
	}

	sceKernelDcacheWritebackAll();
}

int PAF_Resource_GetPageNodeByID_Patched(void *resource, char *name, SceRcoEntry **child)
{
	int res = PAF_Resource_GetPageNodeByID(resource, name, child);

	if(name)
	{
		if(is_tn_config == 1 || is_tn_config == 2 || is_tn_config == 3 ||
#ifndef ITEM
		is_tn_config == 5)
#else
		is_tn_config == 4)
#endif
			if(sce_paf_private_strcmp(name, "page_psp_config_umd_autoboot") == 0)
				switch(context_mode)
				{
					case 0:
						HijackContext(*child, NULL, 0);
						break;

					case 1:
						HijackContext(*child, string.speed, sizeof(string.speed) / sizeof(char *));
						break;

					case 2:
						HijackContext(*child, string.usbdevice, sizeof(string.usbdevice) / sizeof(char *));
						break;

					case 3:
						HijackContext(*child, string.regions, sizeof(string.regions) / sizeof(char *));
						break;

					case 4:
						HijackContext(*child, button_options, sizeof(button_options) / sizeof(char *));
						break;
						
					case 6:
						HijackContext(*child, string.msspeedup, sizeof(string.msspeedup) / sizeof(char *));
						break;
						
					case 7:
						HijackContext(*child, string.isodriver, sizeof(string.isodriver) / sizeof(char *));
						break;
						
					case 8:
						HijackContext(*child, string.on_off, sizeof(string.on_off) / sizeof(char *) - 1);
						break;
						
					case 9:
						HijackContext(*child, string.on_off, sizeof(string.on_off) / sizeof(char *));
						break;
				}
	}

	return res;
}

int PAF_Resource_ResolveRefWString_Patched(void *resource, u32 *data, int *a2, char **string, int *t0)
{
	if(data[0] == 0xDEAD)
	{
		utf8_to_unicode((wchar_t *)user_buffer, (char *)data[1]);
		*(wchar_t **)string = (wchar_t *)user_buffer;
		return 0;
	}

	return PAF_Resource_ResolveRefWString(resource, data, a2, string, t0);
}

int auth_handler_new(int a0)
{
	startup = a0;
	return auth_handler(a0);
}

int OnInitAuthPatched(void *a0, int (* handler)(), void *a2, void *a3, int (* OnInitAuth)())
{
	return OnInitAuth(a0, startup ? auth_handler_new : handler, a2, a3);
}

int sceVshCommonGuiBottomDialogPatched(void *a0, void *a1, void *a2, int (* cancel_handler)(), void *t0, void *t1, int (* handler)(), void *t3)
{
	return sceVshCommonGuiBottomDialog(a0, a1, a2, startup ? OnRetry : cancel_handler, t0, t1, handler, t3);
}

void PatchVshMain(u32 text_addr)
{
	if(psp_model != 4)
	{
		/* Hide UMD Update Icon */
		int umdIoOpenPatched(PspIoDrvFileArg* arg, const char* file, int flags, SceMode mode)
		{
			int res;
			res = umdIoOpen(arg, file, flags, mode);
			if(strcmp(file, "/PSP_GAME/SYSDIR/UPDATE/PARAM.SFO") == 0 && config2.hideumdupdate)
				res = -1;

			return res;
		}
		
		PspIoDrv* umddrv = sctrlHENFindDriver("isofs");
		umdIoOpen = umddrv->funcs->IoOpen;
		umddrv->funcs->IoOpen = umdIoOpenPatched;
	}

	AddVshItem = (void *)text_addr + patch[0];
	ExecuteAction = (void *)text_addr + patch[1];
	UnloadModule = (void *)text_addr + patch[2];
	OnXmbContextMenu = (void *)text_addr + patch[3];
	OnXmbPush = (void *)text_addr + patch[4];
	LoadStartAuth = (void *)text_addr + patch[5];
	auth_handler = (void *)text_addr + patch[6];

	MAKE_CALL(text_addr + patch[7], AddVshItemPatched);
	MAKE_CALL(text_addr + patch[8], ExecuteActionPatched);
	MAKE_CALL(text_addr + patch[9], ExecuteActionPatched);
	MAKE_CALL(text_addr + patch[10], UnloadModulePatched);

	_sw(0x8C48000C, text_addr + patch[11]); //lw $t0, 12($v0)
	MAKE_CALL(text_addr + patch[12], OnInitAuthPatched);

	REDIRECT_FUNCTION(text_addr + patch[13], scePafGetTextPatched);

	_sw((u32)OnXmbPushPatched, text_addr + patch[14]);
	_sw((u32)OnXmbContextMenuPatched, text_addr + patch[15]);

	ClearCaches();
}

void PatchAuthPlugin(u32 text_addr)
{
	OnRetry = (void *)text_addr + patch[32];
	MAKE_CALL(text_addr + patch[29], sceVshCommonGuiBottomDialogPatched);
	ClearCaches();
}

void PatchSysconfPlugin(u32 text_addr)
{
	AddSysconfItem = (void *)text_addr + patch[16];
	GetSysconfItem = (void *)text_addr + patch[17];
	OnInitMenuPspConfig = (void *)text_addr + patch[18];

	sysconf_unk = text_addr + patch[19];
	sysconf_option = text_addr + patch[20];

	/* Allows more than 18 items */
	_sh(0xFF, text_addr + patch[21]);

	MAKE_CALL(text_addr + patch[22], vshGetRegistryValuePatched);
	MAKE_CALL(text_addr + patch[23], vshSetRegistryValuePatched);

	MAKE_CALL(text_addr + patch[24], GetSysconfItemPatched);

	REDIRECT_FUNCTION(text_addr + patch[25], PAF_Resource_GetPageNodeByID_Patched);
	REDIRECT_FUNCTION(text_addr + patch[26], PAF_Resource_ResolveRefWString_Patched);
	REDIRECT_FUNCTION(text_addr + patch[27], scePafGetTextPatched);

	_sw((u32)OnInitMenuPspConfigPatched, text_addr + patch[28]);

	ClearCaches();
}

int WriteLetterPatched(int sel)
{
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	SceCtrlData pad;
	sceCtrlPeekBufferPositive(&pad, 1);

	if(pad.Lx == 0)
	{
		int len = _lw(_lw(osk_param) + len_array);
		u32 *array = (u32 *)(_lw(_lw(_lw(_lw(osk_param) + param_array) + 0x1E8) + 0x1C) + 0x34);
		wchar_t *string = (wchar_t *)array[0];

		if(copied_string)
		{
			sceKernelFreePartitionMemory(block_id);
			copied_string = NULL;
		}

		block_id = sceKernelAllocPartitionMemory(2, "", PSP_SMEM_Low, len * 2, NULL);

		if(block_id)
		{
			copied_string = sceKernelGetBlockHeadAddr(block_id);
			memset(copied_string, 0, len * 2);
			memcpy(copied_string, string, len * 2);
			copied_string[len] = 0;
		}

		return 0;
	}
	else if(pad.Lx == 255 && copied_string)
	{
		_sw((u32)copied_string, osk_param - 0x30);
		sceKernelDcacheWritebackAll();
		ReleaseText();

		return 0;
	}

	return WriteLetter(sel);
}

void PatchOSKExtendedOSK(u32 text_addr, SceModule2 *mod)
{
	int i;
	for(i = 0; i < mod->text_size; i += 4)
	{
		u32 addr = text_addr + i;

		if(_lw(addr) == 0x1440FFF8 && _lw(addr + 4) == 0x24060001)
		{
			len_array = (int)_lh(addr + 8);
			param_array = (int)_lh(addr + 0xC);
		}
		else if(_lw(addr) == 0x27BDFFB0 && _lw(addr + 8) == 0xAFB00040 && _lw(addr + 0x10) == 0x8E030008)
		{
			ReleaseText = (void *)addr;

			u32 high = (((int)_lh(addr + 0x4C)) << 16);
			u32 low = ((int)_lh(addr + 0x54));

			if(low >= 0x8000) high -= 0x10000;

			osk_param = high | low;
		}
		else if(_lw(addr) == 0x2C820015)
			HIJACK_FUNCTION(addr - 4, WriteLetterPatched, WriteLetter);
	}

	sceKernelDcacheWritebackAll();
}

int OnModuleStart(SceModule2 *mod)
{
	char *modname = mod->modname;
	u32 text_addr = mod->text_addr;

	if(sce_paf_private_strcmp(modname, "vsh_module") == 0)
		PatchVshMain(text_addr);
	else if(sce_paf_private_strcmp(modname, "sceVshAuthPlugin_Module") == 0)
		PatchAuthPlugin(text_addr);
	else if(sce_paf_private_strcmp(modname, "sysconf_plugin_module") == 0)
		PatchSysconfPlugin(text_addr);
	else if(config2.extendedosk && sce_paf_private_strcmp(modname, "sceVshOSK_Module") == 0)
		PatchOSKExtendedOSK(text_addr, mod);

	return previous? previous(mod) : 0;
}

int module_start(SceSize args, void *argp)
{
	psp_model = kuKernelGetModel();
	devkit = sceKernelDevkitVersion();

	if((devkit == FW(0x639) || devkit == FW(0x660)) && psp_model < 9)
	{
		sctrlSEGetConfig(&config);
		sctrlSE2GetConfig(&config2);

		if(psp_model == 4)
			num2 = N_ITEMS2-1;
		else
			num2 = N_ITEMS2;

		Resolver();

		previous = sctrlHENSetStartModuleHandler(OnModuleStart);
	}

	return 0;
}