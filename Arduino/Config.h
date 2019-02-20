// File: Config.h. Used in Can2Cluster.ino project.
// Purpose: User/Project Configuration.
// Author: T. Black
// Created: Jan-07-2019
// Last Change: Feb-20-2019
/*
   GNU GENERAL PUBLIC LICENSE VERSION 3
   Copyright (C) 2019  T. Black
   This file is part of Can2Cluster_Ford.ino (hereby referred to as Can2Cluster).
   
   Can2Cluster is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Can2Cluster is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Can2Cluster.  If not, see <https://www.gnu.org/licenses/>.
*/

// ****************************************************************************************************************************

// INSTRUCTIONS: USER DEFINES MAY BE CHANGED TO SUIT THE PROJECT REQUIREMENTS.

// ****************************************************************************************************************************
// MP3 AUDIO PLAYER SECTION
// Comment-out (disable) MP3PLAYER if DFPlayer module is not installed.
#define MP3PLAYER
#define MP3_SER_BAUD    9600                            // MP3 Serial Baud.
#define MP3_SER_CONFIG  SERIAL_8N1                      // MP3 Serial Configuration.
// ****************************************************************************************************************************
// IR REMOTE CONTROL SECTION
// Comment-out (disable) IRCONTROL if IR module is not installed.
#define IRCONTROL
#define IR_USE_TIMER5                                   // Force Pin 46 for TxD (unused).
// ****************************************************************************************************************************
// CAN-BUS SECTION
// Change these defines to match your CAN-Bus Boards.
// The CAN-Bus module used in this project is the NiRen MCP2515 + TJA1050 Chinese clone board.
#define HS_CAN_CS       8                               // Chip Select Digital Pin for HS CAN-Bus Module.
#define MS_CAN_CS       9                               // Chip Select Digital Pin for MS CAN-Bus Module.
#define CAN_FREQ        MCP_8MHz                        // CAN-Bus Module Xtal Frequency. 
// ****************************************************************************************************************************
// USB SERIAL/CLI PORT SECTION
#define SERIAL_BAUD        115200                       // Serial Baud Rate.
#define SERIAL_CONFIG      SERIAL_8N1                   // Serial Configuration.
// ****************************************************************************************************************************
// COMMAND LINE INTERFACE SECTION (USES SERIAL USB PORT)
// Change CMD_EOL_TERM if your serial console cannot send a Carriage Return <CR> when [ENTER] is pressed.
// For example, change to  '\n' if your terminal sends a NEWLINE <LF> when [ENTER] is pressed.
#define CMD_EOL_TERM    '\r'                            // Serial Manager EOL termination character.
// ****************************************************************************************************************************

// EOF