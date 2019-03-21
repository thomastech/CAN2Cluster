// File: Can2Cluster.h. Used in Can2Cluster.ino project.
// Purpose: Master project repository.
// Author: T. Black
// Created: Jan-07-2019
// Last Change: Mar-21-2019. Added RunSwitch, AmplifierPwr, ClusterPwr, SetAmplifierPower(), SetClusterPower().
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
// Global Required Include Files.
#include <arduino.h>                                    // Main Arduino Include File for Type Defs.
#include <SPI.h>                                        // Default SPI library, V1.0.0, https://www.arduino.cc/en/Reference/SPI
#include <mcp_can.h>                                    // Seeedstudio CAN-BUS V1.0.0, https://github.com/Seeed-Studio/CAN_BUS_Shield
#include <RBD_SerialManager.h>                          // Serial CLI, V1.0.0-alpha.3, http://robotsbigdata.com/docs-arduino-serial-manager.html
#include "CanBus.h"                                     // Project CAN-Bus data packet Structs.
#include "Config.h"                                     // Project Master Configuration parameters.
#include "GaugeLUT.h"                                   // Project Gauge Scaling Correction Maps.
#include "ClusterDefs.h"
#include "Sequencer.h"                                  // Project Event Sequence Animator.

// Global Optional Include Files.
#ifdef MP3PLAYER
 #include <DFRobotDFPlayerMini.h>                       // DFPlayer Mini MP3 V1.0.3, https://github.com/DFRobot/DFRobotDFPlayerMini
 #include "mp3.h"
#endif

#ifdef IRCONTROL
 #include <IRremote.h>                                  // IR Remote Control V2.2.3, https://github.com/z3t0/Arduino-IRremote
 #define IR_USE_TIMER5                                  // Pin 46 assigned to IR Transmit (but not used).
#endif
// ****************************************************************************************************************************

// VERSION CONTROL MESSAGE STRINGS 
extern const String VERSION_STR;                        // Version Number.
extern const String DATE_STR;                           // Public Release Date.
extern const String GITHUB_STR;                         // Latest Released GitHub Code.
extern const String PROJECT_STR;                        // Project Introduction, rc-cam.com forum Blog

// ****************************************************************************************************************************
// PROTOTYPES
bool isNumeric(String);
bool HSCAN_Online(void);
bool MSCAN_Online(void);
bool Run_Sequencer(const byte map[][SEQ_DATA_CNT],unsigned int StepCnt);
void AnimateSequencer(void);
void ClusterDefault(bool FuelReset);
void EngineStartUp(void);
void OdometerPhaseCycler(void);
void PrintHex(unsigned long val, byte bytecnt, bool lf);
void Print_HSCAN_Data(void);
void Print_MSCAN_Data(void);
void Print_MP3_Detail(uint8_t type, int value);
void PrintVersion(void);
void ProcessCommands(void);
void RemoteControl(void);
void SendCanBus(void);
void SetAmplifierPower(bool state);
void SetClusterPower(bool state);
void UpdateAmpPwr(void);
void UpdatePowerState(void);

// ****************************************************************************************************************************

// Global Variables
extern bool AmplifierPwr;                               // Master Audio Amp Power Relay On/Off state.
extern bool BlinkFlag;                                  // Blinker On/Off Toggle State Flag.
extern bool CLI_PwrFlag;                                // CLI Cluster Power State Flag.
extern bool ClusterPwr;                                 // Master Instrument Cluster Power Relay On/Off state.
extern bool FuelRstFlag;                                // Fuel Gauge Reset timer.
extern bool HS_Print_Flag;                              // HS CAN-Bus Data Display Mode.
extern bool MS_Print_Flag;                              // MS CAN-Bus Data Display Mode.
extern bool RunSwitch;                                  // Run Switch Status.
extern bool SeqRunOnce;                                 // Sequencer Run Once Flag.
extern byte CAN_data[];                                 // CAN-Bus Data Packet.
extern byte CoolantTemperature;                         // Temperature Gauge.
extern unsigned int AnimationMode;                      // Animation Mode.
extern unsigned int BackLightLvl;                       // Night Time Back Light.
extern unsigned int Doors;                              // Driver Door, Passenger Door, & Trunk.
extern unsigned int Fuel;                               // Fuel Gauge.
extern unsigned int LED_PWM_Level;                      // Message Center & Speaker LED Brightness Level.
extern unsigned int OdomPhase;                          // Odometer 3-Phase Cycle Cam.
#ifdef MP3PLAYER
extern unsigned int MP3Volume;                          // MP3 Player Volume.
#endif
extern unsigned int Speed;                              // Speedometer.
extern unsigned int TachRPM;                            // Tachometer.
extern unsigned int Tires;                              // TPM.
extern unsigned long CycleTime;                         // CAN-Bus Refresh Timer.
extern unsigned long AmpPwrTimer;                       // Timer for Amplifier Power-On Time.
extern unsigned long IR_PwrTimer;                       // Timer for IR Remote activated Cluster Power-On Time.
extern unsigned long OdoTime;                           // Odometer Phase Timer.
extern unsigned long SeqHoldTimer;                      // Timer for Pausing Animation Sequencer.
extern String UserCmd;                                  // Serial Manager (CLI) User command parameter.

// ****************************************************************************************************************************

// ** HARDWARE CONSTANTS SECTION **
extern const bool AmpRlyOn;                             // Turn On Amp Power Relay.
extern const bool AmpRlyOff;                            // Turn Off Amp Power Relay.
extern const bool ClusterRlyOn;                         // Turn On Cluster Run Power Relay.
extern const bool ClusterRlyOff;                        // Turn Off Cluster Run Power Relay.
extern const bool MP3BusyOn;                            // MP3 Player Busy, active Low.
extern const bool RunSwOff;                             // Engine Run Switch Off.
extern const bool RunSwOn;                              // Engine Run Switch, active Low.
extern const bool StartSwOff;                           // Starter Switch Off.
extern const bool StartSwOn;                            // Starter Switch, active Low.
extern const unsigned int AmpPwrPin;                    // MP3 Audio Power Amp Relay Pin.
extern const unsigned int IcPwrPin;                     // Instrument Cluster "Run" Power Relay Pin.
extern const unsigned int IRRcvPin;                     // IR Remote Receive Pin.
extern const unsigned int LED_Pin;                      // Arduino's Board Mounted LED (D13).
extern const unsigned int LightPwrPin;                  // LED Lights Pin, MOSFET (PWM).
extern const unsigned int MP3BusyPin;                   // MP3 Player Busy, active Low.
extern const unsigned int RunSwPin;                     // Engine Run Switch, active Low.
extern const unsigned int StartSwPin;                   // Starter Switch, active Low.
extern const unsigned int SwitchOff;                    // Switch State for Ignition Switch Off.
extern const unsigned int SwitchRun;                    // Switch State for Run Switch On.
extern const unsigned int SwitchStart;                  // Switch State for Start Switch On.

// ****************************************************************************************************************************

// ** LIBRARY MODULE SECTION **

// CAN-Bus Communication
extern MCP_CAN CAN_MS;                                  // Set MS CAN-BUS Shield's CS pin.
extern MCP_CAN CAN_HS;                                  // Set HS CAN-BUS Shield's CS pin.

// Serial Control Line Interface (CLI)
extern RBD::SerialManager serial_manager;

// MP3 Audio Player
#ifdef MP3PLAYER                                        // Optional Audio Player for Engine Sounds.
 extern DFRobotDFPlayerMini myDFPlayer;
#endif

// IR Remote Control
#ifdef IRCONTROL
 extern IRrecv irrecv;
 extern decode_results IRdataRx;
#endif

// ****************************************************************************************************************************

// ** SYSTEM CONSTANTS **
extern const byte PWM_Max;                              // Maximum PWM Value (On). For MOSFET controlled lighting.
extern const byte PWM_Min;                              // Minimum PWM Value (Off). For MOSFET controlled lighting.
extern const unsigned int LedLvlStep;                   // Ambient LED Intensity Level Change, per step. For MOSFET PWM lighting.
extern const unsigned long AMPPWR_TIME;                 // Audio Amplifier Power Time, in mS.
extern const unsigned long CYCLE_TIME;                  // Main Loop Cycle Time, in uSecs.
extern const unsigned long BLINKER_TIME;                // Turn Signal Blinker Delay Time, in uSecs.
extern const unsigned long IRPWR_TIME;                  // IR Remote Auto Power Time, in mS.

// ****************************************************************************************************************************

// ** ANIMATION CONSTANTS **
extern const byte AnimateDrive;                         // Car Drive Animation.
extern const byte AnimateFixed;                         // Fixed Speed & RPM, no Animation.
extern const byte AnimateRev1;                          // Engine Rev Animation.
extern const byte AnimateRev2;                          // Engine Rev Animation.
extern const byte AnimateStop;                          // No Animation.
extern const byte AnimateTest;                          // Gauge Test Animation.
extern const byte AnimateUser1;                         // User Pre-set Animation #1, may be animated or not.
extern const byte AnimateUser2;                         // User Pre-set Animation #2, may be animated or not.

// ****************************************************************************************************************************

// ** MP3 PLAYER CONSTANTS **
#ifdef MP3PLAYER
extern const byte MP3_DEF_VOL;                          // Default MP3 Player Volume. 
extern const byte MP3_MAX_VOL;                          // Maximum MP3 Player Volume Setting.
extern const byte MP3_MIN_VOL;                          // Minimum MP3 Player Volume Setting.
extern const byte MP3_OFF_VOL;                          // No Sound, MP3 Player Volume Setting.
#endif

// ****************************************************************************************************************************

// ** CAN-BUS STRUCT & UNION SECTION **
extern union arb10AX1 HdLight;                          // Headlight Control & Back-light Intensity.
extern union arb3B3X0 WarnBzr;                          // Warning Beeper.
extern union arb3B8X0 HighBm;                           // High Beams.
extern union arb383X0 TSignal;                          // Turn Signals.
extern union arb3C1X0 Pk_Brake;                         // Parking Brakes, Brake Fluid.
extern union arb420X4 MaxT_OD_CEL;                      // Overdrive, Max Temp, Check Engine.
extern union arb420X5 Charge_PwrTrain;                  // Charge System, PowerTrain.
extern union arb420X6 Cruise_SecSys;                    // Cruise Control, Security System.
extern union arb445X0 OilP;                             // Oil Pressure Gauge.

// ****************************************************************************************************************************

// ** CAN-BUS HARDWARE CONSTANTS **
extern const unsigned int SPI_CS_PIN_MS;                // MS CAN-Bus CS pin. 
extern const unsigned int SPI_CS_PIN_HS;                // HS CAN-Bus CS pin.
//extern const unsigned int HS_INTR_PIN;                // HS CAN-Bus Interrupt Pin. (NOT USED)
//extern const unsigned int MS_INTR_PIN;                // MS CAN-Bus Interrupt Pin. (NOT USED)

// ****************************************************************************************************************************

// ** HS CAN-BUS ARBIDs **
extern const unsigned int ARB_SPD_RPM;                  // HS CAN-Bus, Speedometer & RPM.
extern const unsigned int ARB_TEMP_OD_CEL;              // HS CAN-Bus, Coolant Temperature, Odometer phase cycle, O/D & Check Engine Indicators.

// ** MS CAN-BUS ARBIDs **
extern const unsigned int ARB_LIGHTS;                   // MS CAN-Bus, Headlight Control & Back-light Intensity.
extern const unsigned int ARB_TURNSIG;                  // MS CAN-Bus, Turn Signal Indicators.
extern const unsigned int ARB_TPM;                      // MS CAN-Bus, Tire Pressure Monitor (TPM).
extern const unsigned int ARB_DOORS;                    // MS CAN-Bus, Doors (Message Center).
extern const unsigned int ARB_BEEP;                     // MS CAN-Bus, Fault Warning Beep.
extern const unsigned int ARB_HBEAM;                    // MS CAN-Bus, High Beam Indicator.
extern const unsigned int ARB_Pk_Brk;                   // MS CAN-Bus, Parking Brake Indicator.
extern const unsigned int ARB_FUEL;                     // MS CAN-Bus, Fuel Gauge.
extern const unsigned int ARB_OIL;                      // MS CAN-Bus, Oil Gauge.

// ****************************************************************************************************************************
// ** CAN-BUS CONTROL DATA CONSTANTS SECTION **

// BACKLIGHT CONSTANTS (Headlights must be On)
extern const byte FULLBRIGHT;
extern const byte FULLDIM;
extern const unsigned long BackLightCtrlTime;           // Backlight Intensity Control Timeout for IR Remote, in mS.
extern const unsigned int BackLightLvlStep;             // Backlight Intensity Level Change, per step. For IR Remote.

// BRAKE SYSTEM CONSTANTS
extern const bool PARKBRAKE_OFF;                        // HandBrake Off.
extern const bool PARKBRAKE_ON;                         // HandBrake On, Chime if moving.
extern const bool BRAKEFLUID_OFF;                       // Brake Fluid Normal, OK.
extern const bool BRAKEFLUID_ON;                        // Brake Fluid Indicator On, Message Center warning.

// CHARGE SYSTEM INDICATOR CONSTANTS
extern const bool CHARGESYS_OFF;
extern const bool CHARGESYS_ON;

// CHECK ENGINE LIGHT (CEL) INDICATOR
extern const byte CEL_OFF;
extern const byte CEL_ON;
extern const byte CEL_BLINK;

// COOLANT TEMPERATURE CONSTANTS
extern const byte COLDTEMP;                             // Below Zero Temperature Gauge.
extern const byte COOLTEMP;                             // Zero (0%) Temperature Gauge.
extern const byte LOWTEMP;                              // One-Quarter (25%) Temperature Gauge.
extern const byte MEDTEMP;                              // Centered (50%) Temperature Gauge.
extern const byte WARMTEMP;                             // Three-Quarters (75%) Temperature Gauge.
extern const byte HIGHTEMP;                             // High Gauge (100%) Temperature without Red Indicator.
extern const byte OVERTEMP;                             // High Gauge (101%) Temperature with Red Indicator.
extern const byte MAXTEMP_OFF;
extern const byte MAXTEMP_ON1;
extern const byte MAXTEMP_ON2;

// CRUISE CONTROL INDICATOR CONSTANTS
extern const bool CRUISE_CTRL_OFF;
extern const bool CRUISE_CTRL_ON;

// DOOR-TRUNK CONSTANTS
extern const byte DRVOPEN;                              // Driver Door Open.
extern const byte PASOPEN;                              // Passenger Door Open.
extern const byte TRNKOPEN;                             // Trunk Open.
extern const byte DOORCLOSE;                            // All Closed. 

// FAULT WARNING BEEP CONSTANTS
extern const bool WARNBEEP_OFF;                         // Warning Beeper not silenced.
extern const bool WARNBEEP_ON;                          // Warning Beeper Silent.

// FUEL GAUGE CONSTANTS
extern const byte FUELFLAG;                             // Fuel State Flag.
extern const unsigned int FUEL0;                        // Fuel 0%;
extern const unsigned int FUEL50;                       // Fuel 50%;
extern const unsigned int FUEL55;                       // Fuel 55%;
extern const unsigned int FUEL56;                       // Fuel 56%;
extern const unsigned int FUEL100;                      // Fuel 100%;
extern const float FUELSCALE;                           // Fuel Gauge Scale Factor.
extern const unsigned long FUEL_RST_TIME;               // Fuel Gauge Reset Timer, in uSecs.

// HEADLIGHT CONSTANTS
extern const bool HEADLIGHTS_OFF;
extern const bool HEADLIGHTS_ON;
extern const bool HIGHBEAM_OFF;
extern const bool HIGHBEAM_ON;

// ODOMETER CONSTANTS
extern const float ODOMSCALE;                           // Scaled uS Delay for 120mph Speedo, Odometer 3-Phase Cycle Timer.

// OIL PRESSURE IDIOT GAUGE CONSTANTS
extern const bool OILPRESS_ON;                          // Oil Pressure On.
extern const bool OILPRESS_OFF;                         // Oil Pressure Off.

// OVERDRIVE INDICATOR CONSTANTS
extern const byte OD_OFF;
extern const byte OD_ON;
extern const byte OD_BLINK;

// POWERTRAIN INDICATOR CONSTANTS
extern const bool PWRTRAIN_OFF;
extern const bool PWRTRAIN_ON;

// SECURITY INDICATOR CONSTANTS
extern const byte SECURITY_OFF;
extern const byte SECURITY_ON;
extern const byte SECURITY_BLINK;

// SPEEDOMETER GAUGE CONSTANTS
extern const unsigned int MAXSPEED;                     // Maximum Speed printed on Speedometer.

// TACHOMETER GAUGE CONSTANTS
extern const unsigned int MAXRPM;                       // Maximum RPM on Tachometer.

// TIRE PRESSURE MONITOR CONSTANTS
extern const byte TIRE_OK;
extern const byte TIRE_LOW;
extern const byte TIRE_FAULT;
extern const byte TPM_FLASH;
extern const byte TPM_FAULT;

// TURN SIGNAL INDICATOR CONSTANTS
extern const byte TURNSIG_OFF;
extern const byte TURNSIG_RIGHT;
extern const byte TURNSIG_LEFT;
extern const byte TURNSIG_EMERGENCY;

