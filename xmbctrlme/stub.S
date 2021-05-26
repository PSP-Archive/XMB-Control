	.set noreorder
	
#include "pspstub.s"

	STUB_START "scePaf",0x00090011,0x00120005
	STUB_FUNC  0xDE69A6CD,sce_paf_private_wcslen
	STUB_FUNC  0x4C386F3C,sce_paf_private_sprintf
	STUB_FUNC  0xDCE3B13E,sce_paf_private_memcpy
	STUB_FUNC  0x5870455C,sce_paf_private_memset
	STUB_FUNC  0x49A72E5D,sce_paf_private_strlen
	STUB_FUNC  0x8F95CC01,sce_paf_private_strcpy
	STUB_FUNC  0xD38E62C6,sce_paf_private_strncpy
	STUB_FUNC  0x1B952318,sce_paf_private_strcmp
	STUB_FUNC  0x9DF5623C,sce_paf_private_strncmp
	STUB_FUNC  0xFD6C776F,sce_paf_private_strchr
	STUB_FUNC  0xCFC81D9F,sce_paf_private_strrchr
	STUB_FUNC  0x4853DF6E,sce_paf_private_strpbrk
	STUB_FUNC  0x5612DE15,sce_paf_private_strtoul
	STUB_FUNC  0x9E9FFBFB,sce_paf_private_malloc
	STUB_FUNC  0xB0363C2E,sce_paf_private_free
	STUB_FUNC  0x70082F6F,scePafGetText
	STUB_FUNC  0xE03A5C26,PAF_Resource_GetPageNodeByID
	STUB_FUNC  0xFDAFC9E9,PAF_Resource_ResolveRefWString
	STUB_END

	STUB_START "vsh",0x00090011,0x00020005
	STUB_FUNC  0x3A370539,vshGetRegistryValue
	STUB_FUNC  0xCD3AF2EC,vshSetRegistryValue
	STUB_END

	STUB_START	"sceVshCommonGui",0x00090011,0x00010005
	STUB_FUNC	0x51FC9D2D,sceVshCommonGuiBottomDialog
	STUB_END

	STUB_START "SystemCtrlForUser",0x40090000,0x00010005
	STUB_FUNC  0x1C90BECB,sctrlHENSetStartModuleHandler
	STUB_END
