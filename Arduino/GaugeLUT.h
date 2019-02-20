// File: GaugeLUT.h. Used in Can2Cluster.ino project.
// Purpose: Gauge Scale Correction Maps.
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

// Coolant Temperature Correction MAP. Improved linear readings for its 56-100% scale range.
const uint8_t PROGMEM  CoolGaugeLUT [] = {              // Offset starts at 56%. (Only 45 values.)
66, 72, 78, 86,                                         // 56-59%.
88, 88, 88, 88, 88, 88, 89, 89, 89, 89,                 // 60-69%.
90, 90, 91, 91, 92, 92, 92, 93, 93, 93,                 // 70-79%.
94, 94, 94, 94, 95, 95, 95, 95, 95, 96,                 // 80-89%.
96, 96, 97, 97, 97, 98, 98, 99, 99, 100,                // 90-99%.
102                                                     // 100%.
};
