c:/pspsdk/bin/make clean
cd vlf
bin2c vlf.prx vlf_prx.h vlf_prx
bin2c intra.prx intra.h intra
bin2c iop.prx iop.h iop
cd ..
bin2c ..\data\sys_mac_address.txt sys_mac_address.h sys_mac_std
bin2c ..\data\sys_version.txt sys_version.h sys_ver_std
bin2c ..\data\xmbctrl.ini xmbctrlini.h xmbctrlini
c:/pspsdk/bin/make TEST=1
rm *.prx *.o *.elf *.sfo sys_version.h sys_mac_address.h xmbctrlini.h
cd vlf
rm *.h
cd ..
copy EBOOT.PBP L:\PSP\GAME\XMBControl\EBOOT.PBP
pause