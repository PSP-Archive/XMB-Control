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

#ifndef __CONF_H__
#define __CONF_H__

#define CONF_PATH "flash1:/config.xmb"

typedef struct _Config2
{
	int magic;
	s16 propasswordcontrol;
	s16 prohideumdupdate;
	s16 prohidefwversion;
	s16 prousevshmenu;
	s16 proosklimitincrease;
	s16 proextendedosk;
	int passwordcontrol;
	int hideumdupdate;
	int spoofversion;
	int osklimitincrease;
	int extendedosk;
} Config2;

/**
 * Gets the SE configuration.
 * Avoid using this function, it may corrupt your program.
 * Use sctrlSE2GetCongiEx function instead.
 *
 * @param config - pointer to a Config2 structure that receives the SE configuration
 * @returns 0 on success
*/
int sctrlSE2GetConfig(Config2 *config2);

/**
 * Gets the SE configuration
 *
 * @param config - pointer to a Config2 structure that receives the SE configuration
 * @param size - The size of the structure
 * @returns 0 on success
*/
int sctrlSE2GetConfigEx(Config2 *config2, int size);

/**
 * Sets the SE configuration
 * This function can corrupt the configuration in flash, use
 * sctrlSE2SetConfigEx instead.
 *
 * @param config - pointer to a Config2 structure that has the SE configuration to set
 * @returns 0 on success
*/
int sctrlSE2SetConfig(Config2 *config2);

/**
 * Sets the SE configuration
 *
 * @param config - pointer to a Config2 structure that has the SE configuration to set
 * @param size - the size of the structure
 * @returns 0 on success
*/
int sctrlSE2SetConfigEx(Config2 *config2, int size);


int SetConfig2(Config2 *config2);
int GetConfig2(Config2 *config2);

#endif