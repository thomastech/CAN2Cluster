// File: CanBus.h. Used in Can2Cluster.ino project.
// Purpose: CAN-Bus Data Field Bit Mapping, Structures & Unions.
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

// CAN-BUS STRUCTS & UNIONS

union arb10AX1 {                                        // ArbID 0x10A, byte 1. Headlight
  struct
  {
    byte Spare1: 7;                                     // Bits D0-D6 unused.
    bool Headlights:1;                                  // Headlights (Night time).
  } ctrl;
  byte all;                                             // Bits D0-D7.
};

union arb3B3X0 {                                        // ArbID 0x3B3, byte 0. Warning Beep.
  struct
  {
    byte Spare1: 7;                                     // Bits D0-D6 unused.
    bool WarnBeep: 1;                                   // Warning Beeper.
  } ctrl;
  byte all;                                             // Bits D0-D7.
};

union arb3B8X0 {                                        // ArbID 0x3B0, byte 0. High Beam Lights.
  struct
  {
    byte Spare1: 7;                                     // Bits D0-D6 unused.
    bool HighBeam:1;                                    // Headlight High Beam Indicator.
  } ctrl;
  byte all;                                             // Bits D0-D7.
};


union arb3C1X0 {                                        // ArbID 0x3C1, byte 0. Brakes.
  struct
  {
    byte Spare1: 5;                                     // Bits D0-D5 unused.
    bool ParkBrake:1;                                   // Parking Brake Indicator & Message Center, chime if moving.
    bool BrakeFluid: 1;                                 // Brake Fluid Warning.
    byte Spare2: 1;                                     // Bit 7 unused.
  } ctrl;
  byte all;                                             // Bits D0-D7.
};

union arb383X0 {                                        // ArbID 0x383, byte 0. Turn Indicators.
  struct
  {
    byte Spare1: 5;                                     // Bits D0-D5 unused.
    byte TurnSig:2;                                     // Turn Signal Indicators.
    byte Spare2: 1;                                     // Bit 7 unused.
  } ctrl;
  byte all;                                             // Bits D0-D7.
};

union arb420X4 {                                        // ArbID 0x420, byte 4. OD indicator, Temp Gauge, CEL.
  struct
  {
    byte Spare1: 2;                                     // Bits D0, D1 unused.
    byte OD: 2;                                         // Overdrive Indicator.
    byte MaxTemp:2;                                     // Force Coolant Gauge to Maximum Temperature (Override).
    byte CEL: 2;                                        // Check Engine Light.
  } ctrl;
  byte all;                                             // Bits D0-D7.
};

union arb420X5 {                                        // ArbID 0x420, byte 5. 
  struct
  {
    byte Spare1:  3;                                    // Bits D0-D2 unused.
    bool Charge:  1;                                    // Charge System Fault Indicator.
    byte Spare2:  3;                                    // Bits D4-D6 unused.
    bool PwrTrain:1;                                    // PowerTrain Fault.
  } ctrl;
  byte all;                                             // Bits D0-D7.
};

union arb420X6 {                                        // ArbID 0x420, byte 6. Cruise Control and Security indicators.
  struct
  {
    byte Spare1:  3;                                    // Bits D0-D1 unused.
    bool Cruise:  1;                                    // Cruise Control Indicator.
    byte Security:2;                                    // Force Coolant Gauge to Maximum Temperature.
    byte Spare2:  2;                                    // Bits D6-D7 unused.
  } ctrl;
  byte all;                                             // Bits D0-D7.
};

union arb445X0 {                                        // ArbID 0x445, byte 0. Oil Pressure Idiot Gauge
  struct
  {
    byte Spare1: 7;                                     // Bits D0-D6 unused.
    bool OilPress:1;                                    // Oil Pressure Gauge (on/off).
  } ctrl;
  byte all;                                             // Bits D0-D7.
};
