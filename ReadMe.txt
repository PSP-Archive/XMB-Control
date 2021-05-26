--(FreeCore XMB Control ver.1.5.5)--
----------------------------------
:: Coryright(C) 2011, Frostegater ::

(Changelog:)

(v1)
[.] Initial release.

(v1.1)
[!] Fixed translate XMB Control in PSPgo.
[+] Added "Use usbversion.txt from ../seplugins/" item in "Configuration" menu.
[i] xx_prosettings.txt reading from seplugins/xmbcontrol if this folder exists.

(v1.2)
[+] Added ME Freecore xmbctrl
[!] Visual fixes.
[!] Fixed PSPgo crashing.

(v1.3)
[+] [ALL] Added "OSK Limit Increase" function.
[!] [ME]  Fixed xmbctrl ME translate path (ef(ms)0:/seplugins/xmbcontrol/xx_mesettings.txt). "ms0:/" priority.
[+] [PRO] Added custom translate "On" "Off" context in "Plugins" menu.
[+] [ME]  Added firmware version and MAC address spoof functions.
[!] [ALL] Fixed ME&PRO compatibility in second config.
[+] [ME]  Added custom translate "On" "Off" context.
[+] [ALL] Compressing PRX`s.
[!] [PRO] Fixed bug if you use more than 18 plug-ins in 6.20 (80108302 error).
[!] [ALL] Fixed IBID. You can use plugin in 6.60 without SensMe™.

(v1.4.x)
[!] [ME]  Fixed compatibility with 6.39 ME (spoofer).
[!] [PRO] Fixed text patching.
[!] [ME]  Fixed spoof text reading.
[!] [PRO] Fixed problem in select "Hide UMD Update" item.
[+] [ALL] Added support special symbols in built-in translates.
[+] [ALL] Added built-in translates: Japanese, French, Russian, Spanish, German, Russian, Chineses, Portuguese (Brasilian).
[!] [ALL] Fixed freezing if you select other language.
[!] [ALL] Fixed bugs in translate items: "Slim Colors" (all), "Use ISO Cache".
[+] [ME]  Added "Use UMD-Video Patch" option on PSPgo.
[+] [ALL] Increased number characters in translate (one line) = 64.
[+] [ALL] Added Extended OSK plugin function.
[!] [ALL] Fixed bug in last icon in "Settings" bar on PSPgo.
[+] [ALL] Integrate "System settings" ("Standard" context item).
[!] [PRO] Uncompressed plug-in, because of bug in procfw.
[!] [ME]  Remaked "Game Folder Homebrew" => "Flash Protect".
[!] [ME]  Blocked MAC spoof in PSP-1000.
[+] [ME]  Added 200/100 CPU clock item.
[+] [INS] Added Edit Text Files function in installer.
[!] [ME]  Fixed show items "UMD Video Patch" and "Autorun program at /PSP/GAME/BOOT/EBOOT.PBP" on any models.
[!] [INS] Fixed bottom dialog's in installer (Japanese keyboard layout).
[!] [INS] Fixed select device on PSPgo. Current: if installer situated in MemoryStick, xmbctrl installing in MemoryStick and if installer situated in Extended Flash, xmbctrl installing in Extended Flash.

(v1.5)
[+] [ALL] United ME xmbctrl and PRO xmbctrl in one PRX file. Current: if you FW (L)ME, loaded ME xmbctrl, else if PRO, loaded PRO xmbctrl.
[!] [ALL] Fixed reading of text files: translates, spoof. For PSPgo.
[+] [ME]  Fixed spoof MAC address for PSP-1000 (Fat).
[!] [ALL] Moved directory for translates: '/seplugins/xmbcontrol/procfw_translates/' and '/seplugins/xmbcontrol/mecfw_translates/'.

(v1.5.5)
[!] [ME]  Fixed freezig because of long line in German translate.
[+] [INS] Added installing default translates when install or update xmbctrl.
[+] [ALL] Added management adding a new item 'ME Settings' and 'PRO Settings'. Use xmbctrl.ini.
[+] [ALL] Added Italian translation's by stevealexanderames.
[!] [INS] Fixed bug with double transfer to the next line when installing plugin.

(Warning:)
- If you know korean or dutch language, please make and send me you translate on 'wololo.net' forum: 'http://wololo.net/talk/memberlist.php?mode=viewprofile&u=1826'.

(Translating XMB Control:)
 * Default translates situated in 'translates/' directory in archive with release. Copy folders from 'translates/' directory to 'seplugins/xmbcontrol/' for install drfault translates.
 * Translate en_prosettings or en_mesettings from seplugins/xmbconrol/.
 * Edit prefix "en" in name file en_mesettings or en_prosettings in:

 | ja  | Japanese
 | en  | English
 | fr  | French
 | es  | Spanish
 | de  | German
 | it  | Italian
 | nl  | Netherlands
 | pt  | Portuguese
 | ru  | Russian
 | ko  | Korean
 | ch1 | Chinese Simplified
 | ch2 | Chinese Traditional


(Tested on:)

  -PSP-1008
  -PSP-2008
  -PSP-3008
  -PSP-N1008 (PSPgo)


(Credits:)

[.] (Bubbletune) Base Code (GC).
[.] (Total_Noob & HackMaN) xmbctrl from 6.39 TN-A HEN & TN_HEN & extended OSK & Ultimate VSH Menu.
[.] (frostegater) adapting TN`s xmbctrl on all 6.xx CFW`s.
[.] (plum) spoofer 0.5fix.
[.] (Davee & Neur0n) Davee`s Unlimited Characters Spoofer.


(Thanks:)

[.] (The Z) German & English initial translation.
[.] (Anonymous) Patch OnXmbPush & OnXmbContext on 6.35 FW.
[.] (djmati11) Idea for porting & Initial releases.
[.] (codestation) 6.60 patches and nids.
[.] (vit9696) ICON0.PNG for installer.
[.] (Dark Alex) VLF library & psp-packer.
[.] (PSPSDK Dev`s) PSPSDK...