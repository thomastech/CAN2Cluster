// File: CanBus.cpp. Used in Can2Cluster.ino project.
// Purpose: Transmit CAN-Bus Data.
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


#include "Can2Cluster_Ford.h"

// ============================================================================================================================
// A bit of interesting information:
// Refresh rate for MS & HS CAN-Bus boards is set to 50mS (CYCLE_TIME).
// HS data rate is 500KBPS, transmit update period is ~0.8mS (o-scope measurement).
// MS data rate is 125KBPS, transmit update period is ~6.9mS (o-scope measurement).

// ============================================================================================================================

// ClusterDefault(): Initialized Cluster to default gauge and indicators states.
// On entry, true = Set fuel to 50% level, false = don't change fuel level.
void ClusterDefault(bool FuelReset)
{    
    Charge_PwrTrain.ctrl.Charge = CHARGESYS_OFF;
    Charge_PwrTrain.ctrl.PwrTrain = PWRTRAIN_OFF;
    Cruise_SecSys.ctrl.Cruise = CRUISE_CTRL_OFF;
    Cruise_SecSys.ctrl.Security = SECURITY_OFF;
    HdLight.ctrl.Headlights = HEADLIGHTS_ON;
    HighBm.ctrl.HighBeam = HIGHBEAM_OFF;
    MaxT_OD_CEL.ctrl.OD = OD_OFF;
    MaxT_OD_CEL.ctrl.MaxTemp = MAXTEMP_OFF;
    MaxT_OD_CEL.ctrl.CEL = CEL_OFF;
    OilP.ctrl.OilPress = OILPRESS_OFF;
    Pk_Brake.ctrl.ParkBrake = PARKBRAKE_OFF;
    Pk_Brake.ctrl.BrakeFluid = BRAKEFLUID_OFF;
    TSignal.ctrl.TurnSig = TURNSIG_OFF;
    WarnBzr.ctrl.WarnBeep = WARNBEEP_ON;

    if(FuelReset) Fuel = FUEL50;
    BackLightLvl = (FULLBRIGHT*3)/4;                    // Cluster Backlight Intensity.
    CoolantTemperature = COLDTEMP;
    Doors = DOORCLOSE;
    Speed = 0;
    TachRPM = 0;
    Tires = TIRE_OK;

    return;
}

// ============================================================================================================================

// OdometerPhaseCycler(): Update the 3-Phase odometer cycle counter. Cycle time based on speedometer value.
void OdometerPhaseCycler(void)
{
    float floatVar = 0;                                 // General Purpose float variable.
    unsigned long PhaseTime = 0;                        // Odometer 3-Phase Cycle Cam Timer.

    // Service the odometer's 3-phase cycle timer. Used by Odometer and Average mph (in Message Center).
    floatVar = (float)(MAXSPEED)/(float)(Speed);        // Calculate phase delay scaling using current speed (MPH).
    PhaseTime = (unsigned long)(ODOMSCALE * floatVar);
    if(micros() >= OdoTime) {                           // ~300mS=120mph, ~400ms=90mph, ~500mS=72mph, ~1S=36mph, ~2S=18mph, ~4S=9mph (avg mph message center).
        OdoTime = micros()  + PhaseTime;                // Reset uS timer.
        if(Speed > 0) OdomPhase++;                      // We are moving.
        if(OdomPhase > 2) OdomPhase=0;                  // Three phases completed. Reset odometer phase cycle.
    }
    
    return;
}

// ============================================================================================================================

// Print_HSCAN_Data(): Display one HS CAN-Bus Data Record (if available). Tab formatted.
void Print_HSCAN_Data(void)
{
    byte len = 0;
    byte buf[8];
    unsigned long ArbID =0x0000;

// If CAN-Bus Data available, print the packet.
    while (CAN_MSGAVAIL == CAN_HS.checkReceive()) {
        CAN_HS.readMsgBufID(&ArbID, &len, buf);
        serial_manager.print("HS ArbID:");
        PrintHex(ArbID, 2, false);
        serial_manager.print(" = ");
        if(len > 8) len = 8;
        for(int i = 0; i<len; i++) {
            PrintHex(buf[i],1,false);
            serial_manager.print("\t");
        }
        serial_manager.println("");
    }

    return;
}

// ============================================================================================================================

// Print_MSCAN_Data(): Display one MS CAN-Bus Data Record (if available). Tab formatted.
void Print_MSCAN_Data(void)
{
    byte len = 0;
    byte buf[8];
    unsigned long ArbID =0x0000;

// If CAN-Bus Data available, print the packet.
    while (CAN_MSGAVAIL == CAN_MS.checkReceive()) {
        CAN_MS.readMsgBufID(&ArbID, &len, buf);
        serial_manager.print("MS ArbID:");
        PrintHex(ArbID, 2, false);
        serial_manager.print(" = ");
        if(len > 8) len = 8;
        for(int i = 0; i<len; i++) {
            PrintHex(buf[i],1,false);
            serial_manager.print("\t");
        }
        serial_manager.println("");
    }

    return;
}

// ============================================================================================================================ 

// HSCAN_online() : Check to see if HS CAN-Bus is receiving data from cluster.
bool HSCAN_Online(void)
{
    byte len = 0;
    byte buf[8];
    unsigned long ArbID =0x0000;
    bool status = false;
    
    while (CAN_MSGAVAIL == CAN_HS.checkReceive()) {     // Data Available.
        CAN_HS.readMsgBufID(&ArbID, &len, buf);         // Flush.
        if(ArbID >= 0x0000) status = true;
    }
    
    return status;
}

// ============================================================================================================================ 

// MSCAN_online() : Check to see if MS CAN-Bus is receiving data from cluster.
bool MSCAN_Online(void)
{
    byte len = 0;
    byte buf[8];
    unsigned long ArbID =0x0000;
    bool status = false;
    
    while (CAN_MSGAVAIL == CAN_MS.checkReceive()) {     // Data Available.
        CAN_MS.readMsgBufID(&ArbID, &len, buf);         // Flush.
        if(ArbID >= 0x0000) status = true;
    }
    
    return status;
}

// ============================================================================================================================

// SendCanBus(): Periodically transmit all MS & HS CAN-Bus data to the cluster. Both are sent at the same refresh rate.
void SendCanBus(void)
{
    unsigned int SpeedX;                                // Temp var, holds Speedometer Value.
    static unsigned long FuelResetTime = 0;             // Initialize Fuel Reset Timer.
    static bool LocalFuelRstFlag = false;               // Fuel Reset Flag for local use.
    
    if(micros() < CycleTime + CYCLE_TIME) {             // Is it CAN-Bus Refresh Cycle Time?
        return;                                         // No, exit.
    }
    
    CycleTime = micros();                               // Reset uS timer.


// **************************************
// *       DEBUG CAN-BUS SECTION        *
// *    UNCOMMENT TO TEST NEW ARBIDs    *
// **************************************
/* 
    for(unsigned int PID = 0x???; PID <= 0x???; PID++) {
        CAN_data[0] = 0x7f;
        CAN_data[1] = 0xff;
        CAN_data[2] = 0xff;
        CAN_data[3] = 0xff;
        CAN_data[4] = 0xff;
        CAN_data[5] = 0xff;
        CAN_data[6] = 0xff;
        CAN_data[7] = 0xff;
        
        if(PID!=ARB_SPD_RPM && PID!=ARB_TEMP_OD_CEL) {
            CAN_HS.sendMsgBuf(PID,0,8,CAN_data); 
        }
        
        if(PID!=ARB_BEEP && PID!=ARB_LIGHTS && PID!=ARB_TURNSIG && PID!=ARB_TPM && PID!=ARB_DOORS && 
          PID!=ARB_BEEP && PID!=ARB_HBEAM && PID!=ARB_Pk_Brk && PID!=ARB_FUEL && PID!=ARB_OIL) {
            CAN_MS.sendMsgBuf(PID,0,8,CAN_data); 
        }
 //       String PIDStr = "PID ";
 //       PIDStr = PIDStr +  PID;
 //       Serial.println(PIDStr);
 //       serial_manager.println(PIDStr);
    }    
*/

// ******************************
// *     HS CAN-Bus Section     *
// ******************************
 
// Update Speedometer and Tach
    if(TachRPM > MAXRPM) TachRPM = MAXRPM;              // Limit RPM.
    CAN_data[0] = highByte(TachRPM*4);                  // RPM High Byte Value, with Ford data scaling.
    CAN_data[1] = lowByte(TachRPM*4);                   // RPM Low Byte Value, with Ford data scaling.
    CAN_data[2] = 0xFF;                                 // Unknown.
    CAN_data[3] = 0xFF;                                 // Unknown.

    if(Speed > MAXSPEED) Speed = MAXSPEED;              // Limit Speed.
    SpeedX = (unsigned int)((Speed + 67.0f) / 0.0065f); // Rescale per Ford speed data format.
    CAN_data[4] = highByte(SpeedX);                     // Speed High Byte Value.
    CAN_data[5] = lowByte(SpeedX);                      // Speed Low Byte Value.    
    CAN_data[6] = 0x00;                                 // Unknown.
    CAN_data[7] = 0x00;                                 // Unknown.
    CAN_HS.sendMsgBuf(ARB_SPD_RPM,0,8,CAN_data);    

// Update the coolant temperature gauge, odometer cycle counter, and O/D, Charge, Cruise indicators.
    CAN_data[0] = CoolantTemperature;                   // Coolant Temperature Gauge.    
    if(OdomPhase == 0) {
        CAN_data[1] = 0x10;                             // Trip meter phase-1 cycle (rolling 0x10, 0x11, 0x17).
    }
    else if(OdomPhase == 1) {
        CAN_data[1] = 0x11;                             // Trip meter phase-2 cycle.
    }
    else if(OdomPhase >= 2) {
        CAN_data[1] = 0x17;                             // Trip meter phase-3 cycle.
    }
    CAN_data[2] = 0x00;                                 // unknown.
    CAN_data[3] = 0x00;                                 // unknown.
    CAN_data[4] = MaxT_OD_CEL.all;                      // Indicators: O/D & check engine light. Also, Force max coolant temp.
    CAN_data[5] = Charge_PwrTrain.all;                  // Indicators: Charge System & Power Train faults.
    CAN_data[6] = Cruise_SecSys.all;                    // Indicators: Cruise control & Security.
    CAN_data[7] = 0x00;                                 // unused
    CAN_HS.sendMsgBuf(ARB_TEMP_OD_CEL,0,8,CAN_data); 

// ******************************
// *     MS CAN-Bus Section     *
// ******************************

// Headlights (Night Time Mode)
    CAN_data[0] = (byte)(BackLightLvl);                 // Backlight Intensity (Night time).
    CAN_data[1] = HdLight.all;                          // Headlights On/Off (Night time).
    CAN_data[2] = 0x00;
    CAN_data[3] = 0x00;
    CAN_MS.sendMsgBuf(ARB_LIGHTS,0,4,CAN_data);    
   
// Service Turn Signal Indicators
    if(BlinkFlag) {
        CAN_data[0] = TSignal.all;                      // Turn On Time for Turn indicators.
    }
    else {
        CAN_data[0] = TURNSIG_OFF;                      // Turn Off Right & Left Turn indicators.
    }
    CAN_data[1] = 0x00;
    CAN_data[2] = 0x00;
    CAN_data[3] = 0x00;
    CAN_MS.sendMsgBuf(ARB_TURNSIG,0,4,CAN_data); 

// Brake Warnings (Indicators, Message Center, Handbrake-On Chime)
    CAN_data[0] = Pk_Brake.all;                         // Warning Data.
    CAN_data[1] = 0x00;
    CAN_data[2] = 0x00;
    CAN_data[3] = 0x00;
    CAN_MS.sendMsgBuf(ARB_Pk_Brk,0,4,CAN_data); 

// Tire Pressure Monitor (TPM)
    CAN_data[0] = (byte)(Tires);                        // TPM Data.
    CAN_data[1] = 0x00;
    CAN_data[2] = 0x00;
    CAN_data[3] = 0x00;
    CAN_data[4] = 0x00;
    CAN_MS.sendMsgBuf(ARB_TPM,0,5,CAN_data); 
    
// Doors and Trunk (Message center)
    CAN_data[0] = (byte)(Doors);                        // Door Data.
    CAN_data[1] = 0x00;
    CAN_data[2] = 0x00;
    CAN_data[3] = 0x00;
    CAN_MS.sendMsgBuf(ARB_DOORS,0,4,CAN_data); 

// Fault Warning Beeper
    CAN_data[0] = (byte)(WarnBzr.all);                  // Beeper Data.
    CAN_data[1] = 0x00;
    CAN_data[2] = 0x00;
    CAN_data[3] = 0x00;
    CAN_MS.sendMsgBuf(ARB_BEEP,0,4,CAN_data); 
        
// High Beam Indicator
    CAN_data[0] = HighBm.all;                           // High Beam Lights On/Off.
    CAN_data[1] = 0x00;
    CAN_data[2] = 0x00;
    CAN_data[3] = 0x00;
    CAN_MS.sendMsgBuf(ARB_HBEAM,0,4,CAN_data);     

 // Fuel Gauge
 // Data changes to the Fuel Gauge are slosh protected (filtered) and needle movement requires several minutes.
 // Setting the FuelRstFlag after data changes will provide instant refresh (~one second delay).
    CAN_data[0] = highByte(Fuel);                       // Fuel High Byte Value.
    CAN_data[1] = lowByte(Fuel);                        // Fuel Low Byte Value.
    CAN_data[2] = FUELFLAG;
    CAN_data[3] = 0x00;
    CAN_data[4] = 0x00;
    CAN_data[5] = 0x00;
    CAN_data[6] = 0x00;
    CAN_data[7] = 0x00;
    
    if(FuelRstFlag) {                                   // User requests fuel gauge reset (instant refresh).
        FuelRstFlag = false;
        LocalFuelRstFlag = true;
        FuelResetTime = millis();
    }
    else if(LocalFuelRstFlag) {                         // Fuel Reset active.
        if(millis() > FuelResetTime + FUEL_RST_TIME) {  // End of Reset.
            LocalFuelRstFlag = false;                   // End Reset.
        }
    }
    else if(digitalRead(RunSwPin) == RunSwOn || 
      CLI_PwrFlag == true || 
      digitalRead(IcPwrPin) == ClusterRlyOn) {          // Instrument Cluster active.
        CAN_MS.sendMsgBuf(ARB_FUEL,0,8,CAN_data);       // Send Fuel Data.
    }
   
// Oil Gauge
    CAN_data[0] = OilP.all;                             // Oil Pressure Data.
    CAN_data[1] = 0x00;
    CAN_data[2] = 0x00;
    CAN_data[3] = 0x00;
    CAN_MS.sendMsgBuf(ARB_OIL,0,4,CAN_data);

    return;
}  
