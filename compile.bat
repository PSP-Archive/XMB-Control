PATH = c:/pspsdk/bin

cd xmbctrlpro
rm *.prx
bin2c translates\ja_prosettings.txt ja_translate.h ja_translate
bin2c translates\en_prosettings.txt en_translate.h en_translate
bin2c translates\fr_prosettings.txt fr_translate.h fr_translate
bin2c translates\es_prosettings.txt es_translate.h es_translate
bin2c translates\de_prosettings.txt de_translate.h de_translate
bin2c translates\it_prosettings.txt it_translate.h it_translate
bin2c translates\nl_prosettings.txt nl_translate.h nl_translate
bin2c translates\pt_prosettings.txt pt_translate.h pt_translate
bin2c translates\ru_prosettings.txt ru_translate.h ru_translate
bin2c translates\ko_prosettings.txt ko_translate.h ko_translate
bin2c translates\ch1_prosettings.txt ch1_translate.h ch1_translate
bin2c translates\ch2_prosettings.txt ch2_translate.h ch2_translate
make ITEM=1
bin2c_64 xmbctrl.prx ..\loader\ixmbctrlpro.h ixmbctrlpro
make clean
make ITEM=0
bin2c_64 xmbctrl.prx ..\loader\xmbctrlpro.h xmbctrlpro
rm *.o *.elf *.h *.prx

cd ..\xmbctrlme
rm *.prx
bin2c translates\ja_mesettings.txt ja_translate.h ja_translate
bin2c translates\en_mesettings.txt en_translate.h en_translate
bin2c translates\fr_mesettings.txt fr_translate.h fr_translate
bin2c translates\es_mesettings.txt es_translate.h es_translate
bin2c translates\de_mesettings.txt de_translate.h de_translate
bin2c translates\it_mesettings.txt it_translate.h it_translate
bin2c translates\nl_mesettings.txt nl_translate.h nl_translate
bin2c translates\pt_mesettings.txt pt_translate.h pt_translate
bin2c translates\ru_mesettings.txt ru_translate.h ru_translate
bin2c translates\ko_mesettings.txt ko_translate.h ko_translate
bin2c translates\ch1_mesettings.txt ch1_translate.h ch1_translate
bin2c translates\ch2_mesettings.txt ch2_translate.h ch2_translate
make ITEM=1
psp-packer xmbctrl.prx
bin2c_64 xmbctrl.prx ..\loader\ixmbctrlme.h ixmbctrlme
make clean
make ITEM=0
psp-packer xmbctrl.prx
bin2c_64 xmbctrl.prx ..\loader\xmbctrlme.h xmbctrlme
rm *.o *.elf *.h *.prx

cd ..\spoofer
rm *.prx
make
bin2c_64 spoofer.prx ..\loader\spoofer.h spoofer
make clean

cd ..\loader
make clean
make
bin2c loader.prx ..\installer\xmbcontrolprx.h xmbcontrolprx
rm *.o *.elf xmbctrlpro.h xmbctrlme.h ixmbctrlpro.h ixmbctrlme.h spoofer.h

cd ..\installer
rm *.PBP
cd vlf
bin2c vlf.prx vlf_prx.h vlf_prx
bin2c intra.prx intra.h intra
bin2c iop.prx iop.h iop
cd ..
bin2c ..\data\sys_mac_address.txt sys_mac_address.h sys_mac_std
bin2c ..\data\sys_version.txt sys_version.h sys_ver_std
bin2c ..\data\xmbctrl.ini xmbctrlini.h xmbctrlini
make
rm *.prx *.o *.elf *.sfo xmbcontrolprx.h sys_version.h sys_mac_address.h xmbctrlini.h
cd vlf
rm *.h

pause