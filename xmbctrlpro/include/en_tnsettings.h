#ifndef __EN_TNSETTINGS_H__
#define __EN_TNSETTINGS_H__

char *en_tnsettings[] =
{
	/* Title */
	"PRO Settings",

	/*Main menu*/
	"Configuration",
	"Advanced",
	"CPU Speed",
	"Standard",
	"Plugins",

	/*[1] Configuration sub menu*/
	"UMD/ISO Driver",
	"USB Device",
	"Fake Region",
	"Spoof Firmware Version From File",
	"Spoof MAC Adress",
	"Skip Gameboot",
	"Skip Coldboot",
	"Use Custom Update Server",
	"Flash Protect",
	"Htmlviewer Custom Save location",
	"USB Charge",
	"Slim Colors",
	"Use version.txt from ../seplugins/",
	"Use usbversion.txt from ../seplugins/",
	"Hide PIC0.PNG and PIC1.PNG",
	"Prevent Hibernation Delition",
	/**sub*/ //Thanks to The Z 
	"Different Drivers for ISO/CSO/UMD Games.",
	"Conects the MS, Flash0-3 or UMD Drive to USB.",
	"For viewing Video-UMDs from other Countries.",
	"Spoofs Firmware under system information.",
	"Spoofs your MAC Adress under System information.",
	"Skips the Gameboot Logo.",
	"Skips the Sony Boot Logo.",
	"For downloading new PRO CFW releases.",
	"Disables the ability to write in Flash0.",
	"Patches the Browser for saving files in /ISO.",
	"Charges the PSP even if the PSP is not in USB Mode.",
	"Enables the Slim Colors in the XMB.",
	"Fakes the installed version of your PSP.",
	"For connecting PSP to PS3.",
	"Hides the Icon and the Background of Games.",
	"Prevents the deletion of the paused Game.",

	/*[1] UMD/ISO/CSO Drivers*/
	"Normal",
	"M33 driver",
	"Sony NP9660",
	"Inferno",
	
	/*[1] USB Device menu*/
	"Memory Stick",
	"UMD Disc",
	
	/*[1] Regions menu*/
	"Default",
	"Japan",
	"America",
	"Europe",
	"Korea",
	"England",
	"Mexico",
	"Australia",
	"Hong Kong",
	"Taiwan",
	"Russia",
	"China",
	"Debug I",
	"Debug II",
	
	/*[1] Memory Stick speed up menu*/
	"Never",
	"POPS",
	"GAME",
	"VSH",
	"POPS & GAME",
	"GAME & VSH",
	"VSH & POPS",
	"Always",
	
	/*[1]Last item add in Configuration menu*/
	"Button Assign",
	/**sub*/ //Thanks to The Z 
	"Swaps the confirm and back Button.",


	/*[2] Advanced sub menu*/
	"Password Control",
	"XMB Plugins",
	"GAME Plugins",
	"POPS Plugins",
	"NoDRM Engine",
	"Memory Stick Speed Up",
	"Hide CFW Files",
	"Block Analog Input in Game",
	"Allow Non-latin1 ISO Filename",
	"OSK Limit Increase Characters",
	"Old Plugin Support",
	"Hide UMD Update",
	"Use ISO Cache",
	"ISO Cache Size",
	"ISO Cache Number",
	"ISO Cache Policy",
	"Use Copy/Paste Text in OSK",
	/**sub*/ //Thanks to The Z 
	"Activates Password Control for accessing the XMB.",
	"Activates Plugins for the XMB.",
	"Activates Plugins for PSP Games and Homebrews.",
	"Activates Plugins for PS1 Games.",
	"Lets you use content from an inactive PSN Account.",
	"Speeds up the reading speed of the Memory Stick.",
	"Blocks the ability to delete CFW Folders by Games.",
	"Disables the Analog-Pad in Games.",
	"Allows special Characters for ISOs.",
	"Increases the OSK Charecters in Internet Browser",
	"Increases the Plugin Support for old Plugins.",
	"Hides the UMD Update from UMDs.",
	"Enables caching for ISO files.",
	"For adjusting the ISO Cache Size.",
	"For adjusting the ISO Cache Number.",
	"For adjusting the ISO Cache Policy.",
	"Analog Left + Enter, Copy. Analog Right + Enter, Paste.",
	
	/*ISO Cache Policy sub menu*/
	"LRU",
	"Random",

	/*[3] CPU Speed sub menu*/
	"CPU Clock XMB",
	"CPU Clock Game",
	/**sub*/
	"Adjust how fast the PSP is working in the XMB.",
	"Adjust how fast the PSP is working in Games.",
	
	/*CPU Speed "Default"*/
	"Default",
	
	/*Yes-No... From file*/
	"Off",
	"On",
	"From file",
};

#endif