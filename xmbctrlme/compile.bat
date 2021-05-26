del *.prx
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
c:/pspsdk/bin/make ITEM=1
psp-packer xmbctrl.prx
del *.o *.elf *.h
pause