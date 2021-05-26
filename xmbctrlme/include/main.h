#ifndef __MAIN_H__
#define __MAIN_H__

#define MAKE_CALL(a, f) _sw(0x0C000000 | (((u32)(f) >> 2) & 0x03FFFFFF), a);
#define REDIRECT_FUNCTION(a, f) _sw(0x08000000 | (((u32)(f) >> 2) & 0x03FFFFFF), a); _sw(0, a + 4);
#define MAKE_DUMMY_FUNCTION0(a) _sw(0x03E00008, a); _sw(0x00001021, a + 4);
#define FW(f) ((((f >> 8) & 0xF) << 24) | (((f >> 4) & 0xF) << 16) | ((f & 0xF) << 8) | 0x10)
#define JAL_OPCODE	0x0C000000
#define J_OPCODE	0x08000000
#define SC_OPCODE	0x0000000C
#define JR_RA		0x03e00008

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
/*1*/	int magic;
/*2*/	int hidecorrupticons;
/*3*/	int skipcoldboot;
/*4*/	int hidemacaddress;
/*5*/	int unk;
/*6*/	int skipgameboot;
/*7*/	int autoruneboot;
/*8*/	int isodriver;	
/*9*/	int notpboot;
/*10*/	int vshcpuspeed;
/*11*/	int vshbusspeed;
/*12*/	int gamecpuspeed;
/*13*/	int gamebusspeed;
/*14*/	int fakeregion;
/*15*/	int opnssmp;
/*16*/	int flashprotect;
/*17*/	int usbdevice;
/*18*/	int usevshmenu;
/*19*/	int slimcolors_usbcharge;
/*20*/	int usenetupd;
/*21*/	int hidepic0pic1;
/*22*/	int xmbplugins;
/*23*/	int gameplugins;
/*24*/	int popsplugins;
/*25*/	int useversion;
/*26*/	int msspeed;
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

int vctrlVSHExitVshMenu(TNConfig *);

int patch[34];

int devkit;

#endif