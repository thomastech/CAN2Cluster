// File: ClusterDefs.h. Used in Can2Cluster.ino project.
// Purpose: Cluster Indicator & Gauge Defines.
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


// ============================================================================================================================

// INDICATOR DEFINES
#define BEEP_OFF        1                               // Warning Beeper Off.
#define BEEP_ON         0                               // Warning Beeper On.
#define BRAKE_OFF       0                               // Parking Brake Off.
#define BRAKE_ON        1                               // Parking Brake On.
#define CHARGE_OFF      0                               // Charge System Indicator Off.
#define CHARGE_ON       1                               // Charge System Indicator On.
#define CRUISE_OFF      0                               // Cruise Control Indicator Off.
#define CRUISE_ON       1                               // Cruise Control Indicator On.
#define HLIGHTS_OFF     0                               // Headlights (night mode) Off.
#define HLIGHTS_ON      1                               // Headlights (night mode) On.
#define HBEAM_OFF       0                               // High Beam Indicator Off.
#define HBEAM_ON        1                               // High Beam Indicator On.
#define OIL_OFF         1                               // Oil Pressure Off.
#define OIL_ON          0                               // Oil Pressure On.
#define PTRAIN_OFF      0                               // Powertrain Indicator Off.
#define PTRAIN_ON       1                               // Powertrain Indicator On.

// COOLANT TEMPERATURE DEFINES
#define COLD_TEMP       0x55                            // Gauge below zero, Cold Gauge Temperature.
#define COOL_TEMP       0x6b                            // Gauge zero, Minimum Temperature.
#define LOW_TEMP        0x6f                            // Gauge at about 25%.
#define MED_TEMP        0x7f                            // Centered (50%) Temperature Gauge.
#define WARM_TEMP       0x9a                            // Gauge at about 75%.
#define HIGH_TEMP       0x9f                            // High (100%) Temperature without Red Indicator.
#define OVER_TEMP       0xa1                            // High (101%) Temperature with Red Indicator.
