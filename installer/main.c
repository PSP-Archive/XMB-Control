/*
	XmbControl Inslaller on VLFlib
	Copyright (C) 2011, Dark_Alex
	Copyright (C) 2011, Frostegater

	main.c: XmbControl installer main code
	
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

/*
	PIC1 Copyright(C) www.freecore.com
	This is not my site..
*/

#include <pspsdk.h>
#include <pspkernel.h>
#include <vlf.h>
#include <string.h>
#include <stdio.h>
#include <kubridge.h>
#include <psputility_sysparam.h>
#include "main.h"

#ifndef TEST
#include "xmbcontrolprx.h"
#endif

#include "sys_version.h"
#include "sys_mac_address.h"
#include "xmbctrlini.h"

PSP_MODULE_INFO("XMBControl_Installer", 0, 1, 5);
PSP_MAIN_THREAD_ATTR(0);

void Install();
void TextEditor();
void Credits_Changelog(int changelog);
void SysInfo();
void MainMenu(int sel, int start);
int OnTextEditorScreenUp(void *param);
int OnTextEditorScreenDown(void *param);

int devkit, point;
u16 macaddrfd[64], fwverfd[64];
int focus, psp_model;
char *path[11];
int enter_button = 0;
extern int currdev;
extern char *cpath;
int plus;

#define FW(f) ((((f >> 8) & 0xF) << 24) | (((f >> 4) & 0xF) << 16) | ((f & 0xF) << 8) | 0x10)

#define NUM_DEL_ITEMS_CREDITS_CHANGELOG 1
VlfText text_credits_changelog[NUM_DEL_ITEMS_CREDITS_CHANGELOG];

#define NUM_DEL_ITEMS_TEXTEDITOR 3
VlfText text_texteditor[NUM_DEL_ITEMS_TEXTEDITOR];

VlfText title_text;
VlfPicture title_pic;
VlfInputBox fwversion, macaddr;

#define ENTER_CIRCLE 0

int ReadFile(char *file, void *buf, int size, int seek)
{
    SceUID fd = sceIoOpen(file, PSP_O_RDONLY, 0);
	
	int read = -1;

	if(fd >= 0)
    {
		sceIoLseek(fd, seek, PSP_SEEK_SET);
		read = sceIoRead(fd, buf, size);
		sceIoClose(fd);
	}

    return read;
}

int WriteFile(char *file, void *buf, int size, int seek)
{
    SceUID fd = sceIoOpen(file, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);

	int write = -1;

    if(fd >= 0)
    {
		sceIoLseek(fd, seek, PSP_SEEK_SET);
        write = sceIoWrite(fd, buf, size);
        sceIoClose(fd);
    }

	return write;
}

static u8 g_buf[64*1024] __attribute__((aligned(64)));
int copy_file(const char *src, const char *dst)
{
	SceUID fd = -1, fdw = -1;
	int ret;

	ret = sceIoOpen(src, PSP_O_RDONLY, 0777);

	if(ret < 0)
		goto error;

	fd = ret;
	ret = sceIoOpen(dst, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);

	if(ret < 0)
		goto error;

	fdw = ret;
	ret = sizeof(g_buf);
	ret = sceIoRead(fd, g_buf, ret);

	while(ret > 0)
	{
		ret = sceIoWrite(fdw, g_buf, ret);
		if (ret < 0)
			goto error;

		ret = sceIoRead(fd, g_buf, ret);
	}

	if(ret < 0)
		goto error;

	sceIoClose(fd);
	sceIoClose(fdw);

	return 0;

error:
	sceIoClose(fd);
	sceIoClose(fdw);

	return ret;
}

int OnCredits_Changelog(int enter)
{
	if(!enter)
	{
		int i;
		for(i = 0; i < NUM_DEL_ITEMS_CREDITS_CHANGELOG; i++)
			vlfGuiRemoveText(text_credits_changelog[i]);

		if(enter_button != ENTER_CIRCLE) /* CROSS */
			vlfGuiCancelBottomDialog();

		MainMenu(point, 0);		
	}

	return VLF_EV_RET_NOTHING;
}

int OnTextEditor(int enter)
{
	if(!enter)
	{
		int i;
		for(i = 0; i < NUM_DEL_ITEMS_TEXTEDITOR; i++)
			vlfGuiRemoveText(text_texteditor[i]);
		
		vlfGuiRemoveInputBox(fwversion);
		vlfGuiRemoveInputBox(macaddr);
		vlfGuiRemoveEventHandler(OnTextEditorScreenUp);
		vlfGuiRemoveEventHandler(OnTextEditorScreenDown);

		vlfGuiCancelBottomDialog();
		MainMenu(1, 0);		
	}
	else if(focus == 2)
	{
		vlfGuiGetInputBoxText(fwversion, fwverfd);
		vlfGuiGetInputBoxText(macaddr, macaddrfd);
		
		int sizecorrect(u16 fd[])
		{
			int i = 0;
			while(1)
				if(fd[i++] == 0)
					return (i - 1) * 2;
		}

		int write = WriteFile(path[7], fwverfd, sizecorrect(fwverfd), 0x2);
		write += WriteFile(path[6], macaddrfd, sizecorrect(macaddrfd), 0x2);
		
		if(write >= 0)
			vlfGuiMessageDialog("Saved...", VLF_MD_TYPE_NORMAL | VLF_MD_BUTTONS_NONE);
	}

	return VLF_EV_RET_NOTHING;
}

int install_thread(SceSize args, void *argp)
{
	char buffer[1024];	
	memset(buffer, 0, sizeof(buffer));

	int size = ReadFile(path[2], buffer, sizeof(buffer), 0);

	if(sceIoDopen(path[0]) < 0)
		sceIoMkdir(path[0], 0777);
				
	if(sceIoDopen(path[1]) < 0)
		sceIoMkdir(path[1], 0777);
		
	if(sceIoDopen(path[8]) < 0)
		sceIoMkdir(path[8], 0777);
		
	if(sceIoDopen(path[9]) < 0)
		sceIoMkdir(path[9], 0777);
			
	if(size < 0)
		WriteFile(path[2], path[3], 39, 0);
	else
	{
		if(!strstr(buffer, path[4]))
		{
			strcat(buffer, path[plus ? 3 : 5]);
			WriteFile(path[2], buffer, size + (plus ? 39 : 41), 0);
		}
	}
#ifndef TEST			
	WriteFile(path[4], (void *)xmbcontrolprx, size_xmbcontrolprx, 0);
#endif		

	WriteFile(path[10], (void *)xmbctrlini, size_xmbctrlini, 0);

	if(sceIoOpen(path[6], PSP_O_RDONLY, 777) < 0)
		WriteFile(path[6], sys_mac_std, size_sys_mac_std, 0);

	if(sceIoOpen(path[7], PSP_O_RDONLY, 777) < 0)
		WriteFile(path[7], sys_ver_std, size_sys_ver_std, 0);
	
	static char *language[] = { "ja", "en", "fr", "es", "de", "it", "nl", "pt", "ru", "ko", "ch1", "ch2" };
	int i;
	char srcbuf[256], dstbuf[256];
	for(i = 0; i < 12; i++)
	{
		sprintf(srcbuf, "%s/mecfw_translates/%s_mesettings.txt", cpath, language[i]);
		sprintf(dstbuf, "%s0:/seplugins/xmbcontrol/mecfw_translates/%s_mesettings.txt", currdev != MEMORY_STICK ? "ef" : "ms", language[i]);
		if(sceIoOpen(srcbuf, PSP_O_RDONLY, 777) > 0/* && sceIoOpen(dstbuf, PSP_O_RDONLY, 777) < 0*/)
			copy_file(srcbuf, dstbuf);
	}
	
	for(i = 0; i < 12; i++)
	{
		sprintf(srcbuf, "%s/procfw_translates/%s_prosettings.txt", cpath, language[i]);
		sprintf(dstbuf, "%s0:/seplugins/xmbcontrol/procfw_translates/%s_prosettings.txt", currdev != MEMORY_STICK ? "ef" : "ms", language[i]);
		if(sceIoOpen(srcbuf, PSP_O_RDONLY, 777) > 0/* && sceIoOpen(dstbuf, PSP_O_RDONLY, 777) < 0*/)
			copy_file(srcbuf, dstbuf);
	}

	return sceKernelExitDeleteThread(0);
}

void installing()
{
	SceUID install_thid = sceKernelCreateThread("install_thread", install_thread, 0x18, 0x10000, 0, NULL);// Dark_Alex's antifreeze method.
	if(install_thid >= 0)
		sceKernelStartThread(install_thid, 4, NULL);
		
	vlfGuiMessageDialog("Done...", VLF_MD_TYPE_NORMAL | VLF_MD_BUTTONS_NONE);
}

int menu_sel(int sel)
{
	int read;

	switch (sel)
	{
		case 0:
			installing();
			break;

		case 1:
			read = ReadFile(path[7], &fwverfd, sizeof(fwverfd), 0x2);
			read += ReadFile(path[6], &macaddrfd, sizeof(macaddrfd), 0x2);
			
			if(read >= 0)
				TextEditor();
			else
			{
				vlfGuiMessageDialog("Not found text files...", VLF_MD_TYPE_NORMAL | VLF_MD_BUTTONS_NONE);
				break;
			}
				
			return VLF_EV_RET_REMOVE_HANDLERS | VLF_EV_RET_REMOVE_OBJECTS;

		case 2:
			point = 2;
			Credits_Changelog(1);
			return VLF_EV_RET_REMOVE_HANDLERS | VLF_EV_RET_REMOVE_OBJECTS;

		case 3:
			point = 3;
			Credits_Changelog(0);
			return VLF_EV_RET_REMOVE_HANDLERS | VLF_EV_RET_REMOVE_OBJECTS;
		
		case 4:
			sceKernelExitGame();
			break;
	}
	
	return VLF_EV_RET_NOTHING;
}

void MainMenu(int sel, int start)
{	
	if(!start)
	{
		vlfGuiRemoveText(title_text);
		vlfGuiRemovePicture(title_pic);
	}

	title_text = vlfGuiAddText(1, 1, "XMB Control ver.1.5.5");
	title_pic = vlfGuiAddPictureResource("ps3scan_plugin.rco", "tex_infobar_icon", 4, -2);
	vlfGuiSetTitleBar(title_text, title_pic, 1, 1);

	char *items[] = { "Install XMB Control", "Edit Text Files", "Changelog", "Credits", "Exit" };

	vlfGuiCentralMenu(sizeof(items) / sizeof(char *), items, sel, menu_sel, 0, 0);

	if(enter_button == ENTER_CIRCLE)
	{
		vlfGuiCustomBottomDialog(NULL, "Enter", 1, 0, VLF_DEFAULT, NULL);
		vlfGuiCancelBottomDialog();
	}

	vlfGuiCustomBottomDialog(NULL, "Enter", 1, 0, VLF_DEFAULT, NULL);
}

int OnTextEditorScreenUp(void *param)
{
	switch(focus)
	{
		case 1:
			focus = 0;
			vlfGuiSetInputBoxFocus(fwversion, 1);
			vlfGuiSetInputBoxFocus(macaddr, 0);
			break;
		
		case 2:
			focus = 1;
			vlfGuiSetInputBoxFocus(macaddr, 1);
			vlfGuiRemoveTextFocus(text_texteditor[2], 1);
			break;
	}
	
	return VLF_EV_RET_NOTHING;
}

int OnTextEditorScreenDown(void *param)
{
	switch(focus)
	{
		case 0:
			focus = 1;
			vlfGuiSetInputBoxFocus(fwversion, 0);
			vlfGuiSetInputBoxFocus(macaddr, 1);
			break;
		
		case 1:
			focus = 2;
			vlfGuiSetInputBoxFocus(macaddr, 0);
			vlfGuiSetTextFocus(text_texteditor[2]);
			break;
	}
	
	return VLF_EV_RET_NOTHING;
}

void TextEditor()
{
	vlfGuiCustomBottomDialog("Back", NULL, 1, 0, VLF_DEFAULT, OnTextEditor);

	vlfGuiRemoveText(title_text);
	vlfGuiRemovePicture(title_pic);
	
	title_text = vlfGuiAddText(35, 1, "Edit Text Files");
	title_pic = vlfGuiAddPictureResource("ps3scan_plugin.rco", "tex_infobar_icon", 4, -2);
	vlfGuiSetTitleBar(title_text, title_pic, 1, 1);

	text_texteditor[0] = vlfGuiAddText(87, 45, "Firmware Version");
	text_texteditor[1] = vlfGuiAddText(87, 95, "MAC Address");
	text_texteditor[2] = vlfGuiAddText(240, 195, "Save");

	vlfGuiSetTextAlignment(text_texteditor[2], VLF_ALIGNMENT_CENTER);

	fwversion = vlfGuiAddInputBox("Firmware Version", 64, 65);
	macaddr = vlfGuiAddInputBox("MAC Address", 64, 115);
	
	vlfGuiSetInputBoxTextW(fwversion, fwverfd);
	vlfGuiSetInputBoxTextW(macaddr, macaddrfd);

	vlfGuiSetInputBoxFocus(macaddr, 0);
	focus = 0;

	vlfGuiAddEventHandler(PSP_CTRL_UP, 0, OnTextEditorScreenUp, NULL);
	vlfGuiAddEventHandler(PSP_CTRL_DOWN, 0, OnTextEditorScreenDown, NULL);

	int i;
	for(i = 0; i < NUM_DEL_ITEMS_TEXTEDITOR - 1; i++)
		vlfGuiSetTextFontSize(text_texteditor[i], 0.75f);
}

void Credits_Changelog(int changelog)
{
	vlfGuiCustomBottomDialog("Back", NULL, 1, 0, VLF_DEFAULT, NULL);
	vlfGuiCancelBottomDialog();
	vlfGuiCustomBottomDialog("Back", NULL, 1, 0, VLF_DEFAULT, OnCredits_Changelog);
	
	vlfGuiRemoveText(title_text);
	vlfGuiRemovePicture(title_pic);
	
	title_text = vlfGuiAddText(35, 1, point != 2 ? "Credits" : "Changelog");
	title_pic = vlfGuiAddPictureResource("ps3scan_plugin.rco", "tex_infobar_icon", 4, -2);
	vlfGuiSetTitleBar(title_text, title_pic, 1, 1);

	char *crchtext;
	int x;
	if(changelog)
	{
		crchtext =
			"(v1.0)\n"
			" - Initial release.\n\n"
			"(v1.1)\n"
			" - Fixed translate XMB Control in PSPgo.\n"
			" - Added \"Use usbversion.txt from ../seplugins/\"\n"
			"    item in \"Configuration\" menu.\n"
			" - xx_prosettings.txt reading from seplugins/\n"
			"    xmbcontrol if this folder exists.\n\n"
			"(v1.2)\n"
			" - Added ME Freecore XMB Control.\n"
			" - Visual fixes.\n"
			" - Fixed crashing on PSPgo (ME).\n\n"
			"(v1.3)\n"
			" - Added \"OSK Limit Increase\" function.\n"
			" - Fixed xmbctrl ME translate path\n"
			"    (ef(ms)0:/seplugins/xmbcontrol/xx_mesettings.txt).\n"
			"    \"ms0:/\" priority.\n"
			" - Added custom translate \"On\" \"Off\" context in\n"
			"    \"Plugins\" menu in PRO xmbctrl.\n"
			" - Added firmware version and MAC address spoof\n" 
			"    function's in ME xmbctrl.\n"
			" - Fixed ME&PRO compatibility in second config.\n"
			" - Added custom translate \"On\" \"Off\" context\n"
			"    in ME xmbctrl.\n"
			" - Compressing PRX`s.\n"
			" - Fixed bug if you use more than 18 plug-in's in\n"
			"    6.20 (80108302 error).\n"
			" - Fixed IBID. You can use plugin in 6.60\n"
			"    without SensMe.\n\n"
			"(v1.4.x)\n"
			" - Fixed compatibility with 6.39 ME.\n"
			" - Fixed text patching in PRO xmbctrl.\n"
			" - Fixed spoof text reading in ME xmbctrl.\n"
			" - Fixed problem in \"Hide UMD Update\" item\n"
			"    in ME xmbctrl.\n"
			" - Added support special symbols in built-in\n"
			"    translate.\n"
			" - Added built-in translate's: Japanese, French,\n"
			"    Russian, Spanish, German, Russian, Chineses\n"
			"     Portuguese (Brasilian).\n"
			" - Fixed freezing if you select other language.\n"
			" - Fixed bugs in translate items: \"Slim Colors\" (all),\n"
			"    \"Use ISO Cache\".\n"
			" - Added \"Use UMD-Video Patch\" option for PSPgo\n"
			"    in PRO xmbctrl.\n"
			" - Increased number character's in translate\n"
			"    (one line) = 64.\n"
			" - Added Extended OSK plugin function.\n"
			" - Fixed bug in last icon in \"Settings\" bar on PSPgo.\n"
			" - Integrate \"System settings\" (\"Standard\" item\n"
			"    in main context).\n"
			" - Uncompressed PRO xmbctrl, because of bug in procfw.\n"
			" - \"Game Folder Homebrew\" => \"Flash Protect\"\n"
			"    in ME xmbctrl.\n"
			" - Blocked MAC spoof in PSP-1000 in ME xmbctrl.\n"
			" - Added 200/100 CPU clock item.\n"
			" - Added Edit Text Files function in installer.\n"
			" - Fixed show item's \"UMD Video Patch\" and\n"
			"   \"Autorun program at /PSP/GAME/BOOT/EBOOT.PBP\"\n"
			"     on any models. In ME xmbctrl.\n"
			" - Fixed bottom dialog's in installer\n"
			"    (Japanese keyboard layout).\n"
			" - Fixed select device on PSPgo in installer.\n\n"
			"(v1.5)\n"
			" - United ME xmbctrl and PRO xmbctrl in one PRX file.\n"
			"    Current: if you FW (L)ME, loaded ME xmbctrl,\n"
			"     else if PRO, loaded PRO xmbctrl.\n"
			" - Fixed reading of text files: translates, spoof.\n"
			"    For PSPgo.\n"
			" - Fixed spoof MAC address for PSP-1000 for ME xmbctrl.\n"
			" - Moved directory for translates:\n"
			"    '/seplugins/xmbcontrol/procfw_translates/' and\n"
			"     '/seplugins/xmbcontrol/mecfw_translates/'.\n\n"
			"(v1.5.5)\n"
			" - Fixed freezig because of long line in German translate.\n"
			"    For ME xmbctrl.\n"
			" - Added installing default translates when install\n"
			"    or update xmbctrl.\n"
			" - Added management adding a new item 'ME Settings'\n"
			"    and 'PRO Settings'. Use xmbctrl.ini.\n"
			" - Added Italian translation's.\n"
			" - Fixed bug with double transfer to the next line\n"
			"    when installing plugin.";
		x = 35;
	}
	else
	{
		crchtext =
			"XMB Control FreeCore\n"
			"Copyright(C) 2011, Frostegater\n\n"
			"   Credits:\n"
			"     - Bubbletune\n"// Base, GC
			"     - Total Noob\n"// Module in TN_HEN and initial work
			"     - HackMaN\n"// Module in TN_HEN and initial work with TN
			"     - Davee\n"// Davees spoofer
			"     - plum\n\n"// spoofer
			"   Thanks:\n"
			"     - The Z\n"// de/en translate
			"     - codestation\n"// 6.60 compatibility
			"     - Anonymous\n"// 6.35 some patches
			"     - djmati11\n"// Idea for porting and initial PRO releases
			"     - vit9696\n"// ICON0
			"     - PSPSDK Dev`s\n" // pspsdk
			"     - naki-the-curious\n"// ja translate
			"     - gdljjrod\n"// es translate
			"     - kalidor\n"// fr translate
			"     - wskuso\n"//ch1/ch2 translates
			"     - Yoti\n"// uo pspident
			"     - jas0nuk\n"// pspident
			"     - raing3\n"// psptool
			"     - Solidiego"// pt-br translate
			"     - stevealexanderames";// it translate

		x = 140;
	}
	text_credits_changelog[0] = vlfGuiAddText(x, 60, crchtext);		
	vlfGuiSetTextScrollBar(text_credits_changelog[0],  150);
	int i;
	for(i = 0; i < NUM_DEL_ITEMS_CREDITS_CHANGELOG; i++)
		vlfGuiSetTextFontSize(text_credits_changelog[i], 0.75f);
}

int app_main(int argc, char *argv[])
{
	devkit = sceKernelDevkitVersion();
	psp_model = kuKernelGetModel();

#ifndef TEST
	if(devkit != FW(0x620) && devkit != FW(0x635) && devkit != FW(0x639) && devkit != FW(0x660))
	{
		char buf[256];
		char *chr = (char *)&devkit;// PSPident (c)jas0nuk/Yoti
		sprintf(buf, "Not supported firmware: %i.%i%i...", chr[3], chr[2], chr[1]);
		vlfGuiMessageDialog(buf, VLF_MD_TYPE_NORMAL | VLF_MD_BUTTONS_NONE);
		sceKernelExitGame();
	}
#endif

	if(psp_model == 4 && currdev != MEMORY_STICK)
	{
		path[0] = "ef0:/seplugins";
		path[1] = "ef0:/seplugins/xmbcontrol";
		path[2] = "ef0:/seplugins/vsh.txt";
		path[3] = "ef0:/seplugins/xmbcontrol/xmbctrl.prx 1";
		path[4] = "ef0:/seplugins/xmbcontrol/xmbctrl.prx";
		path[5] = "\r\nef0:/seplugins/xmbcontrol/xmbctrl.prx 1";
		path[6] = "ef0:/seplugins/xmbcontrol/sys_mac_address.txt";
		path[7] = "ef0:/seplugins/xmbcontrol/sys_version.txt";
		path[8] = "ef0:/seplugins/xmbcontrol/mecfw_translates";
		path[9] = "ef0:/seplugins/xmbcontrol/procfw_translates";
		path[10] = "ef0:/seplugins/xmbcontrol/xmbctrl.ini";
	}
	else
	{
		path[0] = "ms0:/seplugins";
		path[1] = "ms0:/seplugins/xmbcontrol";
		path[2] = "ms0:/seplugins/vsh.txt";
		path[3] = "ms0:/seplugins/xmbcontrol/xmbctrl.prx 1";
		path[4] = "ms0:/seplugins/xmbcontrol/xmbctrl.prx";
		path[5] = "\r\nms0:/seplugins/xmbcontrol/xmbctrl.prx 1";
		path[6] = "ms0:/seplugins/xmbcontrol/sys_mac_address.txt";
		path[7] = "ms0:/seplugins/xmbcontrol/sys_version.txt";
		path[8] = "ms0:/seplugins/xmbcontrol/mecfw_translates";
		path[9] = "ms0:/seplugins/xmbcontrol/procfw_translates";
		path[10] = "ms0:/seplugins/xmbcontrol/xmbctrl.ini";
	}

	char buf[2048];
	SceUID fd = sceIoOpen(path[2], PSP_O_RDONLY, 777);
	if(fd >= 0)
	{
		int sizefd = sceIoLseek(fd, 0, PSP_SEEK_END);
		sceIoLseek(fd, 0, PSP_SEEK_SET);
		sceIoRead(fd, &buf, sizefd);
		if(buf[sizefd - 1] == '\n')
			plus = 1;
		else
			plus = 0;
	}
	sceIoClose(fd);

	vlfGuiSetTitleBar(NULL, NULL, 1, 1);
	vlfGuiSystemSetup(1, 1, 1);
	sceUtilityGetSystemParamInt(9, &enter_button);
	MainMenu(0, 1);
	
	while(1)
		vlfGuiDrawFrame();
	
	return 0;
}