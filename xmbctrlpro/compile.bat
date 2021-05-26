PATH = c:/pspsdk/bin
del *.prx
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
del *.o *.elf *.h
pause