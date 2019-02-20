// File: IR_Codes.h, Used in Can2Cluster.ino project.
// Purpose: IR Remote Key Codes.
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

// Change these IR Remote keypad button constants to match the received data.
#ifdef IRCONTROL
const unsigned long RC_Button1 = 0xffa25d;
const unsigned long RC_Button2 = 0xff629d;
const unsigned long RC_Button3 = 0xffe21d;
const unsigned long RC_Button4 = 0xff22dd;
const unsigned long RC_Button5 = 0xff02fd;
const unsigned long RC_Button6 = 0xffc23d;
const unsigned long RC_Button7 = 0xffe01f;
const unsigned long RC_Button8 = 0xffa857;
const unsigned long RC_Button9 = 0xff906f;
const unsigned long RC_Button0 = 0xff9867;
const unsigned long RC_ButtonS = 0xff6897;
const unsigned long RC_ButtonP = 0xffb04f;
const unsigned long RC_UP      = 0xff18e7;
const unsigned long RC_DN      = 0xff4ab5;
const unsigned long RC_LT      = 0xff10ef;
const unsigned long RC_RT      = 0xff5aa5;
const unsigned long RC_OK      = 0xff38c7;
const unsigned long RC_REP     = 0Xffffffff;
#endif