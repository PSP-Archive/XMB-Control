/**
  This file is part of TN PPATCH.

  Copyright (C) 2011, Frostegater	

  TN PPATCH is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  TN PPATCH is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with TN PPATCH. If not, see <http://www.gnu.org/licenses/.
 */

#include <pspsdk.h>
#include <pspkernel.h>
#include "include/conf.h"

#include <string.h>
#include <systemctrl.h>


#define CONFIG_MAGIC2 0x43424D58

static inline u32 get_conf_magic2(void)
{
	u32 version;
	version = CONFIG_MAGIC2;

	return version;
}

int sctrlSE2SetConfigEx(Config2 *config2, int size)
{
	SceUID fd;
	int written;
	   
	fd = sceIoOpen(CONF_PATH, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);

	if (fd < 0)
		return -1;

	config2->magic = get_conf_magic2();

	written = sceIoWrite(fd, config2, size);

	if (written != size)
	{
		sceIoClose(fd);
		return -1;
	}

	sceIoClose(fd);

	return 0;
}

int sctrlSE2GetConfigEx(Config2 *config2, int size)
{
	int read;
	SceUID fd;

	read = -1;

	memset(config2, 0, size);
	fd = sceIoOpen(CONF_PATH, PSP_O_RDONLY, 0666);

	if (fd > 0)
	{
		read = sceIoRead(fd, config2, size);
		sceIoClose(fd);


		if(read != size)
			return -2;

		if(config2->magic != get_conf_magic2())
			return -3;
					
		return 0;
	}
	
	return -1;
}

int sctrlSE2SetConfig(Config2 *config2)
{
	return sctrlSE2SetConfigEx(config2, sizeof(*config2));
}

int sctrlSE2GetConfig(Config2 *config2)
{
	return sctrlSE2GetConfigEx(config2, sizeof(*config2));
}
