#ifndef __MAIN_H__
#define __MAIN_H__

#define MAKE_CALL(a, f) _sw(0x0C000000 | (((u32)(f) >> 2) & 0x03FFFFFF), a);
#define REDIRECT_FUNCTION(a, f) _sw(0x08000000 | (((u32)(f) >> 2) & 0x03FFFFFF), a); _sw(0, a + 4);
#define MAKE_DUMMY_FUNCTION0(a) _sw(0x03E00008, a); _sw(0x00001021, a + 4);
#define FW(f) ((((f >> 8) & 0xF) << 24) | (((f >> 4) & 0xF) << 16) | ((f & 0xF) << 8) | 0x10)
#define PatchSyscall sctrlHENPatchSyscall
#define JAL_OPCODE 0x0C000000
#define J_OPCODE 0x08000000
#define SC_OPCODE 0x0000000C
#define JR_RA 0x03e00008

#define NOP	0x00000000

#define MAKE_JUMP(a, f) _sw(J_OPCODE | (((u32)(f) & 0x3FFFFFFF) >> 2), a); 
#define HIJACK_FUNCTION(a, f, ptr)	{ \
										static u32 patch_buffer[3]; \
										_sw(_lw(a + 0x00), (u32)patch_buffer + 0x00); \
										_sw(_lw(a + 0x04), (u32)patch_buffer + 0x08);\
										MAKE_JUMP((u32)patch_buffer + 0x04, a + 0x08); \
										REDIRECT_FUNCTION(a, f); \
										ptr = (void *)patch_buffer; \
									}

#define sysconf_console_id 4
#define sysconf_console_action 2
#define sysconf_console_action_arg 2

#define sysconf_tnconfig_action_arg 0x1000

typedef struct
{
	int magic;
	s16 isodriver;	
	s16 vshcpuspeed;
	s16 vshbusspeed;
	s16 gamecpuspeed;
	s16 gamebusspeed;
	s16 fakeregion;
	s16 usbdevice;
	s16 usbcharge;
	s16 hidemacaddress;
	s16 skipgameboot;
	s16 hidepic0pic1;
	s16 xmbplugins;
	s16 gameplugins;
	s16 popsplugins;
	s16 flashprotect;
	s16 skipcoldboot;
	s16 useversion;
	s16 usenetupd;
	s16 nodrm;
	s16 prevent_hib_del_go;
	s16 noanalog;
	s16 old_plugin_support;
	s16 htmlviewer_custom_save_location;
	s16 hidecfwfolders;
	s16 nonlatinisofilename;
	s16 msspeed;
	s16 slimcolors;
	s16 iso_cache;
	s16 iso_cache_size;
	s16 iso_cache_num;
	s16 iso_cache_policy;
	s16 useusbversion;
	s16 language;
} TNConfig;

typedef struct
{
	char text[48];
	int play_sound;
	int action;
	int action_arg;
} SceContextItem;

typedef struct
{
	int id;
	int relocate;
	int action;
	int action_arg;
	SceContextItem *context;
	char *subtitle;
	int unk;
	char play_sound;
	char memstick;
	char umd_icon;
	char image[4];
	char image_shadow[4];
	char image_glow[4];
	char text[0x25];
} SceVshItem;

typedef struct
{
	void *unk;
	int id;
	char *regkey;
	char *text;
	char *subtitle;
	char *page;
} SceSysconfItem;

typedef struct
{
	u8 id;
	u8 type;
	u16 unk1;
	u32 label;
	u32 param;
	u32 first_child;
	int child_count;
	u32 next_entry;
	u32 prev_entry;
	u32 parent;
	u32 unknown[2];
} SceRcoEntry;

int sce_paf_private_wcslen(wchar_t *);
int sce_paf_private_sprintf(char *, const char *, ...);
void *sce_paf_private_memcpy(void *, void *, int);
void *sce_paf_private_memset(void *, char, int);
int sce_paf_private_strlen(char *);
char *sce_paf_private_strcpy(char *, const char *);
char *sce_paf_private_strncpy(char *, const char *, int);
int sce_paf_private_strcmp(const char *, const char *);
int sce_paf_private_strncmp(const char *, const char *, int);
char *sce_paf_private_strchr(const char *, int);
char *sce_paf_private_strrchr(const char *, int);
int sce_paf_private_strpbrk(const char *, const char *);
int sce_paf_private_strtoul(const char *, char **, int);
void *sce_paf_private_malloc(int);
void sce_paf_private_free(void *);

wchar_t *scePafGetText(void *, char *);
int PAF_Resource_GetPageNodeByID(void *, char *, SceRcoEntry **);
int PAF_Resource_ResolveRefWString(void *, u32 *, int *, char **, int *);

int vshGetRegistryValue(u32 *, char *, void *, int , int *);
int vshSetRegistryValue(u32 *, char *, int , int *);

int sceVshCommonGuiBottomDialog(void *a0, void *a1, void *a2, int (* cancel_handler)(), void *t0, void *t1, int (* handler)(), void *t3);

int sctrlSEGetConfig(TNConfig *);
int sctrlSESetConfig(TNConfig *);
int vctrlVSHExitVSHMenu(TNConfig *);

int devkit;
int patch[32];

wchar_t *mac_buf;
wchar_t *ver_buf;

void PatchMacAddressText(SceModule2 *mod, int flag);
void PatchFWVersionText(SceModule2 *mod, int flag);
void get_scePafFormatText(SceModule2 *mod);
void MakePath(const char *argp, char *buf, int flag);
int ReadTxt(const char *file, int flag);

#endif