// File: cli.cpp. Used in Can2Cluster.ino project.
// Purpose: Command Line Interface (CLI). Provides Serial Terminal Control.
// Author: T. Black
// Created: Jan-07-2019
// Last Change: Mar-21-2019. Revised ProcessCommands().
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


#include "Can2Cluster_Ford.h"

// ============================================================================================================================
// ProcessCommands(): Process the commands sent through the serial port.
// This is the Command Line Interface (CLI).
void ProcessCommands(void)
{
    unsigned int Value = 0;                             // General Purpose variable.
    String StrValue;                                    // General Purpose message string.
    
    if(serial_manager.onReceive()) {                    // Process any serial commands from user (CLI).
//     StrValue = "Raw CLI Data: ";                     // Debug message.
//     StrValue += serial_manager.getValue();
//     serial_manager.println(StrValue);

        UserCmd = serial_manager.getCmd();              // Get Command.
        UserCmd.toUpperCase();
        if(UserCmd == "?" || UserCmd == "H" || UserCmd == "HELP") {
            serial_manager.println("");
            serial_manager.println(F("COMMAND LINE INTERFACE HELP SUMMARY"));
            serial_manager.println(F(" Brake Fluid:   BF,[0-1]     -> off/on"));
            serial_manager.println(F(" Back Light:    BL,[0-100]   -> percent"));
            serial_manager.println(F(" CruiseControl: CC,[0-1]     -> off/on"));
            serial_manager.println(F(" Check Engine:  CE,[0-2]     -> off/on/blink"));            
            serial_manager.println(F(" Cluster Power: CP,[0-1]     -> off/on"));
            serial_manager.println(F(" Charge System: CS,[0-1]     -> off/on"));
            serial_manager.println(F(" Coolant Temp:  CT,[0-100]   -> percent"));
            serial_manager.println(F(" Door & Trunk:  DT,[0-3]     -> off/driver|pass|trunk"));
            serial_manager.println(F(" Fuel Tank:     FT,[0-100,R] -> percent/Reset"));
            serial_manager.println(F(" High Beam:     HB,[0-1]     -> off/on"));
            serial_manager.println(F(" Head Light:    HL,[0-1]     -> off/on"));
            serial_manager.println(F(" Maximum Temp:  MT,[0-2]     -> off/on1/on2"));
            serial_manager.println(F(" Overdrive:     OD,[0-2]     -> off/on/blink"));
            serial_manager.println(F(" Oil Pressure:  OP,[0-1]     -> off/on"));
            serial_manager.println(F(" Park Brake:    PB,[0-1]     -> off/on"));
            serial_manager.println(F(" PowerTrain:    PT,[0-1]     -> off/on [RESET PWR TO CLEAR]"));
            serial_manager.println(F(" Speedometer:   SP,[0-120]   -> mph"));
            serial_manager.println(F(" Security Sys:  SS,[0-2]     -> off/on/blink"));
            serial_manager.println(F(" Tire Pressure: TP,[0-4]     -> off/low/fault/low-fault/flash"));
            serial_manager.println(F(" Tach RPM:      TR,[0-7000]  -> revs"));
            serial_manager.println(F(" Turn Signal:   TS,[0-3]     -> off/right/left/emergency"));
            serial_manager.println(F(" Warning Beep:  WB,[0-1]     -> off/on"));
            serial_manager.println(F(" -----------------------------------------------------------------"));
            serial_manager.println(F("       ** DIAGNOSTIC COMMANDS **"));
            serial_manager.println(F(" CAN-Bus Info:  CBINF,[0-2] -> HS/MS/HS-MS"));
            serial_manager.println(F(" HS CAN Data:   HSCAN,[0-1] -> off/on"));
            serial_manager.println(F(" MS CAN Data:   MSCAN,[0-1] -> off/on"));
            serial_manager.println(F(" Run Animation: RUNAN,[0-6] -> STOP/FIXED/REV/DRV/TEST/USER1/USER2"));
            serial_manager.println(F(" Reset Data:    RDAT        -> Reset All Data to Defaults"));
            serial_manager.println(F(" Show Version:  VERS        -> Version & Project information"));
            serial_manager.println(F(" -----------------------------------------------------------------"));
            serial_manager.println("");       
        }
        else if(ClusterPwr == ClusterRlyOff && UserCmd != "CP" &&
          UserCmd != "CBINF" && UserCmd != "HSCAN" && UserCmd != "MSCAN" && 
          UserCmd != "RDAT" && UserCmd != "VERS" ) {
            serial_manager.println(F("COMMAND IGNORED! Please use Ignition Key or CP command and Turn On Power."));
        }
        else if(UserCmd == "BF") {                      // Brake Fluid Warning & Indicator, 0-1.
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(Pk_Brake.ctrl.BrakeFluid,1,true);
            }   
            else {
                if(StrValue == "0") {
                    Pk_Brake.ctrl.BrakeFluid = BRAKEFLUID_OFF;
                    StrValue = F(" (0=Off)");
                }
                else if(StrValue == "1") {
                    Pk_Brake.ctrl.BrakeFluid = BRAKEFLUID_ON;
                    StrValue = F(" (1=On)");
                }
                else if(StrValue == "OFF") {
                    Pk_Brake.ctrl.BrakeFluid = BRAKEFLUID_OFF;
                    StrValue = F(" (Off)");
                }
                else if(StrValue == "ON") {
                    Pk_Brake.ctrl.BrakeFluid = BRAKEFLUID_ON;
                    StrValue = F(" (On)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("BRAKE FLUID: "));
                PrintHex(Pk_Brake.ctrl.BrakeFluid,1,false);
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "BL") {                      // Back Lighting Intensity, 0-100%.
            String StrValue = serial_manager.getParam();
            if(StrValue == "?") {
                PrintHex(BackLightLvl,1,true);
            }
            else if(StrValue != "" && isNumeric(StrValue)) {
                serial_manager.println(StrValue);
                Value = StrValue.toInt();
                BackLightLvl = (unsigned int)((float)(Value) * 2.55);
                if(BackLightLvl > FULLBRIGHT) BackLightLvl = FULLBRIGHT;
                serial_manager.print(F("BACKLIGHT LEVEL: "));
                PrintHex(BackLightLvl,1,false);
                StrValue = " (" + StrValue + "%)";
                serial_manager.println(StrValue);
            }
            else {
                serial_manager.print(F("BACKLIGHT LEVEL: "));
                PrintHex(BackLightLvl,1,false);
                serial_manager.println(F(" (Error)"));
            }            
        }
        else if(UserCmd == "CC") {                      // Cruise Control Indicator, 0-1;
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(Cruise_SecSys.ctrl.Cruise,1,true);
            }
            else {
                if(StrValue=="0") {
                    Cruise_SecSys.ctrl.Cruise = CRUISE_CTRL_OFF;
                    StrValue = F(" (0=Off)");
                }
                else if(StrValue=="1") {
                    Cruise_SecSys.ctrl.Cruise = CRUISE_CTRL_ON;
                    StrValue = F(" (1=On)");
                }
                else if(StrValue=="OFF") {
                    Cruise_SecSys.ctrl.Cruise = CRUISE_CTRL_OFF;
                    StrValue = F(" (Off)");
                }
                else if(StrValue=="ON") {
                    Cruise_SecSys.ctrl.Cruise = CRUISE_CTRL_ON;
                    StrValue = F(" (On)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("Cruise Control: "));
                PrintHex(Cruise_SecSys.ctrl.Cruise,1,false);
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "CE") {                      // Check Engine Light (CEL), 0-2.
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(MaxT_OD_CEL.ctrl.CEL,1,true);
            }
            else {
                if(StrValue=="0") {
                    MaxT_OD_CEL.ctrl.CEL = CEL_OFF;
                    StrValue = " (" + StrValue + F("=Off)");
                }
                else if(StrValue=="1") {
                    MaxT_OD_CEL.ctrl.CEL = CEL_ON;
                    StrValue = " (" + StrValue + F("=On)");
                }
                else if(StrValue=="2" || StrValue=="B") {
                    MaxT_OD_CEL.ctrl.CEL = CEL_BLINK;
                    StrValue = " (" + StrValue + F("=Blink)");
                }
                else if(StrValue=="OFF") {
                    MaxT_OD_CEL.ctrl.CEL = CEL_OFF;
                    StrValue = F(" (Off)");
                }
                else if(StrValue=="ON") {
                    MaxT_OD_CEL.ctrl.CEL = CEL_ON;
                    StrValue = F(" (On)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("Chk Eng Light: "));
                PrintHex(MaxT_OD_CEL.ctrl.CEL,1,false);
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "CP") {                      // Cluster Power, 0-1.
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                StrValue = F("CLUSTER POWER: ");
                if(ClusterPwr == ClusterRlyOn){
                    StrValue += F("On ");
                }
                else {
                    StrValue += F("Off ");
                }
                if(RunSwitch == RunSwOn){               // Ignition Switch is in the Run or Start Position.
                    StrValue += F("(by Ignition Key)");
                }
                serial_manager.println(StrValue);
            }   
            else {
                if(StrValue == "0" || StrValue == "OFF") {
                    CLI_PwrFlag = false;
                    StrValue = F("Off");
                }
                else if(StrValue == "1" || StrValue == "ON") {
                    CLI_PwrFlag = true;
                    StrValue = F("On");
                }
                else {
                    StrValue = F(" Error, Invalid Parameter");
                }
                
                if(RunSwitch == RunSwOn) {              // Ignition Switch is in the Run or Start Position.
                    CLI_PwrFlag = false;                // Cancel CLI Power On command.
                    StrValue += F(" Refused (Ignition Key Override)");
                }
                serial_manager.print(F("CLUSTER POWER: "));
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "CS") {                      // Charge System Fault Indicator, 0-1;
            StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(Charge_PwrTrain.ctrl.Charge,1,true);
            }
            else {
                if(StrValue=="0") {
                    Charge_PwrTrain.ctrl.Charge = CHARGESYS_OFF;
                    StrValue = F(" (0=Off, RESET PWR TO CLEAR!)");
                }
                else if(StrValue=="1") {
                    Charge_PwrTrain.ctrl.Charge = CHARGESYS_ON;
                    StrValue = F(" (1=On)");
                }
                else if(StrValue=="OFF") {
                    Charge_PwrTrain.ctrl.Charge = CHARGESYS_OFF;
                    StrValue = F(" (Off, RESET PWR TO CLEAR!)");
                }
                else if(StrValue=="ON") {
                    Charge_PwrTrain.ctrl.Charge = CHARGESYS_ON;
                    StrValue = F(" (On)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("Charge System: "));
                PrintHex(Charge_PwrTrain.ctrl.Charge,1,false);
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "CT") {                      // Coolant Temperature, 0-100%.
            String StrValue = serial_manager.getParam();
            if(StrValue == "?") {
                PrintHex(CoolantTemperature,1,true);
            }   
            else if(StrValue != "" && isNumeric(StrValue)) {
                Value = StrValue.toInt();
                if(Value >= 56 && Value <= 100) {       // Gauge Scale Correction needed on upper range.
                    String strx = String(sizeof(CoolGaugeLUT)); // Get Lookup Value from correction map.
                    Value = pgm_read_byte(&(CoolGaugeLUT[Value-56])); // Use corrected (rescaled) temperature.
                }
                CoolantTemperature = COLDTEMP + ((float)(Value)*0.75);
                if(CoolantTemperature > OVERTEMP) CoolantTemperature = OVERTEMP; // Force Warning indicator.
                MaxT_OD_CEL.ctrl.MaxTemp = MAXTEMP_OFF;    // Turn Off Maximum Temperature Override.
                serial_manager.print(F("TEMPERATURE: "));
                PrintHex(CoolantTemperature,1,false);
                StrValue = " (" + StrValue + "%)";
                serial_manager.println(StrValue);
            }
            else {
                serial_manager.print(F("TEMPERATURE: "));
                PrintHex(CoolantTemperature,1,false);
                serial_manager.println(F(" (Error)"));
            }
        }
        else if(UserCmd == "DT") {                      // Doors & Trunk indicators, 0-3.
            String StrValue = serial_manager.getParam();
            if(StrValue == "?") {
                PrintHex(Doors,1,true);
            }   
            else if(StrValue != "" && isNumeric(StrValue)) {
                Value = StrValue.toInt();
                if(Value == 0) {
                    Doors = DOORCLOSE;
                    StrValue = " (" + StrValue + ")";
                }
                else if(Value == 1) {
                    Doors = Doors | DRVOPEN;
                    StrValue = " (" + StrValue + ")";
                }
                else if(Value == 2) {
                    Doors = Doors | PASOPEN;
                    StrValue = " (" + StrValue + ")";
                }
                else if(Value == 3) {
                    Doors = Doors | TRNKOPEN;
                    StrValue = " (" + StrValue + ")";
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("DOORS: "));
                PrintHex(Doors,1,false);
                serial_manager.println(StrValue);
            }
            else {
                serial_manager.println(F("DOORS: Error"));
            }
        }
        else if(UserCmd == "FT") {                      // Fuel Tank Gauge, 0-100%
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(Fuel,1,true);
            }
            else if(StrValue == "R") {
                FuelRstFlag = true;
                serial_manager.print(F("Fuel: "));
                PrintHex(Fuel,1,false);
                serial_manager.println(F(" (Reset Gauge)"));
            }
            else if(StrValue != "" && isNumeric(StrValue)) {
                Value = StrValue.toInt();
                if(Value <= 55) {
                    Fuel = FUEL0 + (unsigned int)((float)(Value) * FUELSCALE);
                    if(Fuel > FUEL55) Fuel = FUEL55;
                }
                else {
                    Fuel = FUEL56 + (unsigned int)((float)(Value-55) * FUELSCALE);
                    if(Fuel > FUEL100) Fuel = FUEL100;
                }
                serial_manager.print(F("Fuel: "));
                PrintHex(Fuel,1,false);
                StrValue = " (" + StrValue + "%)";
                serial_manager.println(StrValue);
            }
            else {
                serial_manager.print(F("Fuel: "));
                PrintHex(Fuel,1,false);
                serial_manager.println(F(" (Error)"));
            }
        }
        else if(UserCmd == "HB") {                      // High Beam (Headlights), 0-1.
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(HighBm.ctrl.HighBeam,1,true);
            }
            else {
                if(StrValue=="0") {
                    HighBm.ctrl.HighBeam = HIGHBEAM_OFF;
                    StrValue = F(" (0=Off)");
                }
                else if(StrValue=="1") {
                    HighBm.ctrl.HighBeam = HIGHBEAM_ON;
                    StrValue = F(" (1=On)");
                }
                else if(StrValue=="OFF") {
                    HighBm.ctrl.HighBeam = HIGHBEAM_OFF;
                    StrValue = F(" (Off)");
                }
                else if(StrValue=="ON") {
                    HighBm.ctrl.HighBeam = HIGHBEAM_ON;
                    StrValue = F(" (On)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("HIGHBEAM: "));
                PrintHex(HighBm.ctrl.HighBeam,1,false);
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "HL") {                      // Head Lights (Daytime / Nighttime), 0-1.
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(HdLight.ctrl.Headlights,1,true);
            }
            else {
                if(StrValue=="0") {
                    HdLight.ctrl.Headlights = HEADLIGHTS_OFF;
                    StrValue = F(" (0=Off)");
                }
                else if(StrValue=="1") {
                    HdLight.ctrl.Headlights = HEADLIGHTS_ON;
                    StrValue = F(" (1=On)");
                }
                else if(StrValue=="OFF") {
                    HdLight.ctrl.Headlights = HEADLIGHTS_OFF;
                    StrValue = F(" (Off)");
                }
                else if(StrValue=="ON") {
                    HdLight.ctrl.Headlights = HEADLIGHTS_ON;
                    StrValue = F(" (On)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("HEADLIGHTS: "));
                PrintHex(HdLight.ctrl.Headlights,1,false);
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "MT") {                      // Force Max Temperature (Gauge Override), 0-2;
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(MaxT_OD_CEL.ctrl.MaxTemp,1,true);
            }
            else {
                if(StrValue=="0") {
                    MaxT_OD_CEL.ctrl.MaxTemp = MAXTEMP_OFF;
                    StrValue = F(" (0=Off)");
                }
                else if(StrValue=="1") {
                    MaxT_OD_CEL.ctrl.MaxTemp = MAXTEMP_ON1;
                    StrValue = F(" (1=On)");
                }
                else if(StrValue=="2") {
                    MaxT_OD_CEL.ctrl.MaxTemp = MAXTEMP_ON2;
                    StrValue = F(" (1=On)");
                }
                else if(StrValue=="OFF") {
                    MaxT_OD_CEL.ctrl.MaxTemp = MAXTEMP_OFF;
                    StrValue = F(" (Off)");
                }
                else if(StrValue=="ON1") {
                    MaxT_OD_CEL.ctrl.MaxTemp = MAXTEMP_ON1;
                    StrValue = F(" (On-1)");
                }
                else if(StrValue=="ON2") {
                    MaxT_OD_CEL.ctrl.MaxTemp = MAXTEMP_ON2;
                    StrValue = F(" (On-2)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("MAX TEMPERATURE: "));
                PrintHex(MaxT_OD_CEL.ctrl.MaxTemp,1,false);
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "OD") {                      // Overdrive indicator, 0-2;
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(MaxT_OD_CEL.ctrl.OD,1,true);
            }
            else {
                if(StrValue=="0") {
                    MaxT_OD_CEL.ctrl.OD = OD_OFF;
                    StrValue = F(" (0=Off)");
                }
                else if(StrValue=="1") {
                    MaxT_OD_CEL.ctrl.OD = OD_ON;
                    StrValue = F(" (1=On)");
                }
                else if(StrValue=="2" || StrValue=="B") {
                    MaxT_OD_CEL.ctrl.OD = OD_BLINK;
                    StrValue = " (" + StrValue + F("=Blink)");
                }
                else if(StrValue=="OFF") {
                    MaxT_OD_CEL.ctrl.OD = OD_OFF;
                    StrValue = F(" (Off)");
                }
                else if(StrValue=="ON") {
                    MaxT_OD_CEL.ctrl.OD = OD_ON;
                    StrValue = F(" (On)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("OVERDRIVE: "));
                PrintHex(MaxT_OD_CEL.ctrl.OD,1,false);
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "OP") {                      // Oil Pressure Idiot Gauge, 0-1.
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(!OilP.ctrl.OilPress,1,true);   // Invert state (active low).
            }
            else {
                if(StrValue=="0") {
                    OilP.ctrl.OilPress = OILPRESS_OFF;
                    StrValue = F(" (0=Off)");
                }
                else if(StrValue=="1") {
                    OilP.ctrl.OilPress = OILPRESS_ON;
                    StrValue = F(" (1=On)");
                }
                else if(StrValue=="OFF") {
                    OilP.ctrl.OilPress = OILPRESS_OFF;
                    StrValue = F(" (Off)");
                }
                else if(StrValue=="ON") {
                    OilP.ctrl.OilPress = OILPRESS_ON;
                    StrValue = F(" (On)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("OIL PRESSURE: "));
                PrintHex(!OilP.ctrl.OilPress,1,false);  // Invert state (active low).
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "PB") {                      // Parking Brake Warning & Indicator, 0-1.
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(Pk_Brake.ctrl.ParkBrake,1,true);
            }   
            else {
                if(StrValue == "0") {
                    Pk_Brake.ctrl.ParkBrake = PARKBRAKE_OFF;
                    StrValue = F(" (0=Off)");
                }
                else if(StrValue == "1") {
                    Pk_Brake.ctrl.ParkBrake = PARKBRAKE_ON;
                    StrValue = F(" (1=On)");
                }
                else if(StrValue == "OFF") {
                    Pk_Brake.ctrl.ParkBrake = PARKBRAKE_OFF;
                    StrValue = F(" (Off)");
                }
                else if(StrValue == "ON") {
                    Pk_Brake.ctrl.ParkBrake = PARKBRAKE_ON;
                    StrValue = F(" (On)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("PARKBRAKE: "));
                PrintHex(Pk_Brake.ctrl.ParkBrake,1,false);
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "PT") {                      // Power Train Fault Indicator, 0-1;
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(Charge_PwrTrain.ctrl.PwrTrain,1,true);
            }
            else {
                if(StrValue=="0") {
                    Charge_PwrTrain.ctrl.PwrTrain = PWRTRAIN_OFF;
                    StrValue = F(" (0=Off, RESET PWR TO CLEAR!)");
                }
                else if(StrValue=="1") {
                    Charge_PwrTrain.ctrl.PwrTrain = PWRTRAIN_ON;
                    StrValue = F(" (1=On)");
                }
                else if(StrValue=="OFF") {
                    Charge_PwrTrain.ctrl.PwrTrain = PWRTRAIN_OFF;
                    StrValue = F(" (Off, RESET PWR TO CLEAR!)");
                }
                else if(StrValue=="ON") {
                    Charge_PwrTrain.ctrl.PwrTrain = PWRTRAIN_ON;
                    StrValue = F(" (On)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("POWERTRAIN: "));
                PrintHex(Charge_PwrTrain.ctrl.PwrTrain,1,false);
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "SP") {                      // Speed (mph), 0-120.
            String StrValue = serial_manager.getParam();
            if(StrValue == "?") {
                serial_manager.println(Speed);
            }   
            else if(StrValue != "" && isNumeric(StrValue)) {
                Speed = StrValue.toInt();
                if(Speed > MAXSPEED) Speed = MAXSPEED;
                serial_manager.print(F("SPEED: "));
                serial_manager.println(Speed);
            }
            else {
                serial_manager.print(F("SPEED: "));
                serial_manager.print(Speed);
                serial_manager.println(F(" (Error)"));
            }
        }
        else if(UserCmd == "SS") {                      // Security System Indicator, 0-2.
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(Cruise_SecSys.ctrl.Security,1,true);
            }
            else {
                if(StrValue=="0") {
                    Cruise_SecSys.ctrl.Security = SECURITY_OFF;
                    StrValue = F(" (0=Off)");
                }
                else if(StrValue=="1") {
                    Cruise_SecSys.ctrl.Security = SECURITY_ON;
                    StrValue = F(" (1=On)");
                }
                else if(StrValue=="2" || StrValue=="B") {
                    Cruise_SecSys.ctrl.Security = SECURITY_BLINK;
                    StrValue = " (" + StrValue + F("=Blink)");
                }
                else if(StrValue=="OFF") {
                    Cruise_SecSys.ctrl.Security = SECURITY_OFF;
                    StrValue = F(" (Off)");
                }
                else if(StrValue=="ON") {
                    Cruise_SecSys.ctrl.Security = SECURITY_ON;
                    StrValue = F(" (On)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("SECURITY SYSTEM: "));
                PrintHex(Cruise_SecSys.ctrl.Security,1,false);
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "TP") {                      // Tire Pressure Monitor (TPM), 0-4.
            String StrValue = serial_manager.getParam();
            if(StrValue == "?") {
                PrintHex(Tires,1,true);
            }   
            else if(StrValue != "" && isNumeric(StrValue)) {
                Value = StrValue.toInt();
                if(Value == 0) {
                    Tires = TIRE_OK;
                    StrValue = F(" (0=TireOK)");
                }
                else if(Value == 1) {
                    Tires = TIRE_LOW;
                    StrValue = F(" (1=TireLow)");
                }
                else if(Value == 2) {
                    Tires = TPM_FAULT;
                    StrValue = F(" (3=TpmFault)");
                }
                else if(Value == 3) {
                    Tires = TIRE_LOW | TPM_FAULT;
                    StrValue = F(" (2=TireLow_TpmFault)");
                }
                else if(Value == 4) {
                    Tires = TPM_FLASH;
                    StrValue = F(" (4=TpmFlash)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("TIRES: "));
                PrintHex(Tires,1,false);
                serial_manager.println(StrValue);
            }
            else {
                serial_manager.println(F("TIRES: Error"));
            }
        }
        else if(UserCmd == "TR") {                      // Tach RPM, 0-7000.
            String StrValue = serial_manager.getParam();
            if(StrValue == "?") {
                serial_manager.println(TachRPM);
            }   
            else if(StrValue != "" && isNumeric(StrValue)) {
                TachRPM = serial_manager.getParam().toInt();
                if(TachRPM > MAXRPM) TachRPM = MAXRPM; 
                serial_manager.print(F("RPM: "));
                serial_manager.println(TachRPM);
            }
            else {
                serial_manager.print(F("RPM: "));
                serial_manager.print(TachRPM);
                serial_manager.println(F(" (Error)"));
            }
        }
        else if(UserCmd == "TS") {                      // Turn Signals, Emergency Lights, 0-3.
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(TSignal.ctrl.TurnSig,1,true);
            }
            else {
                if(StrValue=="OFF") {
                    TSignal.ctrl.TurnSig = TURNSIG_OFF;
                    StrValue = F(" (Off)");
                }
                else if(StrValue=="O" || StrValue=="OFF" || StrValue=="0") {
                    TSignal.ctrl.TurnSig = TURNSIG_OFF;
                    StrValue = F(" (0=Off)");
                }
                else if(StrValue=="R" || StrValue=="1") {
                    TSignal.ctrl.TurnSig = TURNSIG_RIGHT;
                    StrValue = " (" + StrValue + F("=Right)");
                }
                else if(StrValue=="L" || StrValue=="2") {
                    TSignal.ctrl.TurnSig = TURNSIG_LEFT;
                    StrValue = " (" + StrValue + F("=Left)");
                }
                else if(StrValue=="E" || StrValue=="3") {
                    TSignal.ctrl.TurnSig = TURNSIG_EMERGENCY;
                    StrValue = " (" + StrValue + F("=Emergency)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("TURN SIGNAL: "));
                PrintHex(TSignal.ctrl.TurnSig,1,false);
                serial_manager.println(StrValue);
            }
        }
        else if(UserCmd == "WB") {                      // Warning Beeper.
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue == "?") {
                PrintHex(!WarnBzr.ctrl.WarnBeep,1,true); // Invert state (active low).
            }   
            else {
                if(StrValue == "0") {
                    WarnBzr.ctrl.WarnBeep = WARNBEEP_OFF;
                    StrValue = F(" (0=Off)");
                }
                else if(StrValue == "1") {
                    WarnBzr.ctrl.WarnBeep = WARNBEEP_ON;
                    StrValue = F(" (1=On)");
                }
                else if(StrValue == "OFF") {
                    WarnBzr.ctrl.WarnBeep = WARNBEEP_OFF;
                    StrValue = F(" (Off)");
                }
                else if(StrValue == "ON") {
                    WarnBzr.ctrl.WarnBeep = WARNBEEP_ON;
                    StrValue = F(" (On)");
                }
                else {
                    StrValue = F(" (Error)");
                }
                serial_manager.print(F("WARNING BEEP: "));
                PrintHex(!WarnBzr.ctrl.WarnBeep,1,false); // Invert state (active low).
                serial_manager.println(StrValue);
            }
        }        
        else if(UserCmd == "HSCAN") {                   // Display HS CAN-Bus Data.
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue=="0") {
                if(HS_Print_Flag == true) StrValue = F("Quit");
                else StrValue = F("Quit, But Wasn't Running");
                HS_Print_Flag = false;
            }
            else if(StrValue=="1") {
                HS_Print_Flag = true;
            }
            else if(StrValue=="OFF") {
                if(HS_Print_Flag == true) StrValue = F("Quit");
                else StrValue = F("Quit, But Wasn't Running");
                HS_Print_Flag = false;
            }
            else if(StrValue=="ON") {
                HS_Print_Flag = true;
            }
            else {
                if(StrValue == "") StrValue = "?";
                StrValue = "(" + StrValue + F(" is Unknown Parameter)");
            }
            serial_manager.print(F("HS CAN DATA: "));
            serial_manager.println(StrValue);
        }
        else if(UserCmd == "MSCAN") {                   // Display MS CAN-Bus Data.
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue=="0") {
                if(MS_Print_Flag == true) StrValue = F("Quit");
                else StrValue = F("Quit, But Wasn't Running");
                MS_Print_Flag = false;
            }
            else if(StrValue=="1") {
                MS_Print_Flag = true;
            }
            else if(StrValue=="OFF") {
                if(MS_Print_Flag == true) StrValue = F("Quit");
                else StrValue = F("Quit, But Wasn't Running");
                MS_Print_Flag = false;
            }
            else if(StrValue=="ON") {
                MS_Print_Flag = true;
            }
            else {
                if(StrValue == "") StrValue = "?";
                StrValue = "(" + StrValue + F(" is Unknown Parameter)");
            }
            serial_manager.print(F("MS CAN DATA: "));
            serial_manager.println(StrValue);
        }
        else if(UserCmd == "CBINF") {                   // CAN-Bus Information (Online Status)
            String StrValue = serial_manager.getParam();
            StrValue.toUpperCase();
            if(StrValue=="0") {
                StrValue = F("HS CAN-Bus ");
                if(HSCAN_Online() == true) {
                    StrValue += F("Online");
                }
                else {
                    StrValue += F("Offline");
                }
            }
            else if(StrValue=="1") {
                StrValue = F("MS CAN-Bus ");
                if(MSCAN_Online() == true) {
                    StrValue += F("Online");
                }
                else {
                    StrValue += F("Offline");
                }
            }
            else if(StrValue=="2") {
                StrValue = F("HS CAN-Bus ");
                if(HSCAN_Online() == true) {
                    StrValue += F("Online, ");
                }
                else {
                    StrValue = F("Offline, ");
                }
                StrValue += F("MS CAN-Bus ");
                if(MSCAN_Online() == true) {
                    StrValue += F("Online");
                }
                else {
                    StrValue += F("Offline");
                }
            }
            else {
                StrValue = F("(Error, Invalid Parameter)");
            }
            serial_manager.print(F("CAN-BUS INFO: "));
            serial_manager.println(StrValue);
        }
        else if(UserCmd == "RUNAN") {                   // Run (Play) Animated Sequence, 0-4.
            String StrValue = serial_manager.getParam();
            if(StrValue != "" && isNumeric(StrValue)) {
                Value = StrValue.toInt();
                if(Value >= 0 && Value <= 6) {          // Valid Numerical Entry.
                    Run_Sequencer(SequencerStop,0);     // Kill any running Animations.
                    SeqRunOnce = true;                  // Tell Sequencer to Run Once.
                    StrValue = F("Play ");
                }
                else {
                    StrValue = F("Error, ");
                }

                if(Value == 0) {
                    AnimationMode = AnimateStop;        // Use Stop Animation Sequence Map.
                    StrValue += F("Engine Stop");
                }
                else if(Value == 1) {
                    AnimationMode = AnimateFixed;       // Use Fixed Sequence Map (no animation).
                    SeqRunOnce = true;                  // Tell Sequencer to Run Once.
                    StrValue += F("Fixed Speed & RPM");
                }
                else if(Value == 2) {
                    AnimationMode = AnimateRev1;        // Use Engine Rev'ing Animation Sequence Map.
                    SeqRunOnce = true;                  // Tell Sequencer to Run Once.
                    StrValue += F("Engine Rev #1");
                }
                else if(Value == 3) {
                    AnimationMode = AnimateDrive;       // Use Car Driving Animation Sequence Map.
                    SeqRunOnce = true;                  // Tell Sequencer to Run Once.
                    StrValue += F("Car Drive");
                }
                else if(Value == 4) {
                    AnimationMode = AnimateTest;        // Use Gauge Test Animation Sequence Map.
                    SeqRunOnce = true;                  // Tell Sequencer to Run Once.
                    StrValue += F("Gauge Test");
                }
                else if(Value == 5) {
                    AnimationMode = AnimateUser1;       // User #1 Pre-Set Animation Sequence Map.
                    SeqRunOnce = true;                  // Tell Sequencer to Run Once.
                    StrValue += F("User Pre-Set #1");
                }
                else if(Value == 6) {
                    AnimationMode = AnimateUser2;       // User #2 Pre-Set Animation Sequence Map.
                    SeqRunOnce = true;                  // Tell Sequencer to Run Once.
                    StrValue += F("User Pre-Set #2");
                }
                else {
                    StrValue += F("Invalid Parameter");
                }
                serial_manager.print(F("RUN ANIMATION: "));
                serial_manager.println(StrValue);
            }
            else {
                serial_manager.println(F("RUN ANIMATION: Error, Invalid Parameter"));
            }
        }
        else if(UserCmd == "VERS") {                    // Show Project & Version information
            PrintVersion();
        }
        else if(UserCmd == "RDAT") {                    // Reset CAN-Buss Data to default values.
            ClusterDefault(true);                       // Load default data.
            FuelRstFlag = true;                         // Force Fuel Level update.
            serial_manager.println(F("RESET DATA: Default values loaded"));
        }
        else {
            if(serial_manager.getValue() != "") {
                StrValue = (F("UNKNOWN CMD: "));
                StrValue += serial_manager.getValue();
                serial_manager.println(StrValue);
            }
            else {
                serial_manager.println("-> CLI Ready. Enter ? for help.");
            }
        }

    }

    return;
}
