// File: Can2Cluster.ino
// Project: CAN-Bus Controller for 2009 Ford Mustang 6-Gauge Instrument Cluster.
// Compiler: Arduino 1.8.5
// Author: T. Black
// Created: Jan-07-2019
// Last Change: Mar-21-2019. 
//              Added AmplifierPwr, ClusterPwr.
//              Revised Can2Cluster.cpp, Can2Cluster.h, CanBus.cpp, cli.cpp, IR_Control.cpp, Routines.cpp
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

// INCLUDE SECTION
#include "Can2Cluster_Ford.h"                           // Master project repository of constants and defines.

// ****************************************************************************************************************************

// VERSION CONTROL MESSAGE STRINGS (Update these after any code changes).
const String VERSION_STR = "V1.1";                      // Version Number.
const String DATE_STR    = " MAR-21-2019";              // Release Date.
const String GITHUB_STR  = "https://goo.gl/mNsPEY";     // Latest Released Code on GitHub Repository.
const String PROJECT_STR = "https://goo.gl/FrNDj7";     // Project Introduction, rc-cam.com forum Blog

// ****************************************************************************************************************************

// GLOBAL SYSTEM VARIABLES
byte CAN_data[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte CoolantTemperature = COLD_TEMP;                    // Coolant Temperature.
bool AmplifierPwr = AmpRlyOff;                          // Master Audio Amp Power Relay On/Off state.
bool BlinkFlag = false;                                 // Turn Signal Blinker On/Off Toggle Flag.
bool ClusterPwr  = ClusterRlyOff;                       // Master Instrument Cluster Power Relay On/Off state.
bool CLI_PwrFlag = ClusterRlyOff;                       // CLI Cluster Power State Flag.
bool HS_Print_Flag = false;                             // HS CAN-Bus Data Display Mode.
bool MS_Print_Flag = false;                             // MS CAN-Bus Data Display Mode.
bool FuelRstFlag = false;                               // Fuel Gauge Reset timer.
bool RunSwitch = RunSwOff;                              // Run Switch Status.
bool SeqRunOnce = true;                                 // Sequencer Run Once Flag.
unsigned int AnimationMode = AnimateStop;               // Animation Mode.
unsigned int BackLightLvl = (FULLBRIGHT*3)/4;           // Backlight Intensity.
unsigned int Doors = DOORCLOSE;                         // Doors & Trunk.
unsigned int Fuel = FUEL0;                              // Fuel Level.
unsigned int LED_PWM_Level = PWM_Min;                   // Message Center & Speaker LED Brightness Level, 0-255.
unsigned int OdomPhase = 0;                             // Odometer 3-Phase Cycle Cam.
#ifdef MP3PLAYER
unsigned int MP3Volume = MP3_DEF_VOL;                   // MP3 Player Volume.
#endif
unsigned int Speed = 0;                                 // Speedometer.
unsigned int TachRPM = 0;                               // Tachometer.
unsigned int Tires = TIRE_OK;                           // TPM (Tire Pressure Monitor).
unsigned long AmpPwrTimer = 0;                          // Timer for IR Remote activated Cluster Power-On Time.
unsigned long BlinkerTime = 0;                          // Timer for Turn Signal Blinker.
unsigned long CycleTime = 0;                            // Timer for CAN-Bus refresh.
unsigned long IR_PwrTimer= 0;                           // Timer for IR Remote's Automatic Power-On Time.
unsigned long OdoTime = 0;                              // Timer for Odometer.
unsigned long SeqHoldTimer = 0;                         // Timer for Pausing Animation Sequencer.
String UserCmd;                                         // Serial Port Commands from user (CLI).

// ****************************************************************************************************************************

// ** HARDWARE CONSTANTS SECTION **

const bool AmpRlyOn  = HIGH;                            // Turn On Amp Power Relay.
const bool AmpRlyOff = LOW;                             // Turn Off Amp Power Relay.
const bool ClusterRlyOn  = HIGH;                        // Turn On Cluster Run Power Relay.
const bool ClusterRlyOff = LOW;                         // Turn Off Cluster Run Power Relay.
const bool MP3BusyOn = LOW;                             // MP3 Player Busy, active Low.
const bool RunSwOff  = HIGH;                            // Engine Run Switch Off.
const bool RunSwOn   = LOW;                             // Engine Run Switch On.
const bool StartSwOff= HIGH;                            // Starter Switch Off.
const bool StartSwOn = LOW;                             // Starter Switch On.
const unsigned int AmpPwrPin   = 4;                     // MP3 Audio Power Amp Relay Pin.
const unsigned int CAN_XTAL    = CAN_FREQ;              // CAN-Bus Shield XTAL Frequency.
const unsigned int IcPwrPin    = 3;                     // Instrument Cluster "Run" Power Relay Pin.
const unsigned int LED_Pin     = LED_BUILTIN;           // Arduino's Board Mounted LED (D13).
const unsigned int IRRcvPin    = 12;                    // IR Remote Receive Pin.
const unsigned int LightPwrPin = 2;                     // LED Lights Pin, MOSFET (PWM).
const unsigned int MP3BusyPin  = 10;                    // MP3 Player Busy, active Low.
const unsigned int RunSwPin    = 5;                     // Engine Run Switch, active Low.
const unsigned int StartSwPin  = 6;                     // Starter Switch, active Low.

// ****************************************************************************************************************************

// ** LIBRARY MODULE SECTION **

// CAN-Bus Communication
MCP_CAN CAN_MS(SPI_CS_PIN_MS);                          // Set MS CAN-BUS Shield's CS pin.
MCP_CAN CAN_HS(SPI_CS_PIN_HS);                          // Set HS CAN-BUS Shield's CS pin.

// Serial Control Line Interface (CLI)
RBD::SerialManager serial_manager;                      // Command Line Serial Interface.

// MP3 Audio Player
#ifdef MP3PLAYER                                        // Optional Audio Player for Engine Sounds.
 DFRobotDFPlayerMini myDFPlayer;
#endif

// IR Remote Control
#ifdef IRCONTROL                                        // Optional IR Remote Control
 IRrecv irrecv(IRRcvPin);
 decode_results IRdataRx;
#endif


// ****************************************************************************************************************************

// ** SYSTEM CONSTANTS SECTION **
const byte PWM_Max = 0xff;                              // Maximum PWM Value for MOSFET controlled Lighting (Full-On).
const byte PWM_Min = 0x00;                              // Minimum PWM Value for MOSFET controlled Lighting (Off).
const unsigned int LedLvlStep = 25;                     // Ambient LED Intensity Level Change, per step. For MOSFET PWM lighting.
const unsigned long AMPPWR_TIME  = IRPWR_TIME +15000UL; // Audio Amplifier Power-On Time, in mS.
const unsigned long BLINKER_TIME = 500;                 // Blinker Timer Delay, in mS.
const unsigned long CYCLE_TIME   = 50000UL;             // Main Loop Cycle Time, in uS.
const unsigned long IRPWR_TIME   = 120000UL;            // IR Remote Auto Power-On Time, in mS.
const unsigned long FUEL_RST_TIME= 1100;                // Fuel Gauge Reset Timer, in mS.

// ****************************************************************************************************************************

// ** ANIMATION CONSTANTS SECTION **
const byte AnimateStop  = 0;                            // Engine Stopped, No Animation.
const byte AnimateFixed = 1;                            // Fixed Speed & RPM, no Animation.
const byte AnimateRev1  = 2;                            // Engine Rev Animation #1.
const byte AnimateRev2  = 3;                            // Engine Rev Animation #2.
const byte AnimateDrive = 4;                            // Car Drive Animation.
const byte AnimateTest  = 5;                            // Gauge Test Animation.
const byte AnimateUser1 = 6;                            // User Preset #1 Animation.
const byte AnimateUser2 = 7;                            // User Preset #2 Animation.

// ****************************************************************************************************************************

// ** MP3 Player CONSTANTS SECTION **
#ifdef MP3PLAYER
const byte MP3_DEF_VOL   = 12;                          // Default MP3 Player Volume.
const byte MP3_MAX_VOL   = 30;                          // Maximum MP3 Player Volume Setting.
const byte MP3_MIN_VOL   = 1;                           // Minimum MP3 Player Volume Setting.
const byte MP3_OFF_VOL   = 0;                           // No Sound, MP3 Player Volume Setting.
#endif


// ****************************************************************************************************************************

// ** CAN-BUS STRUCT & UNION SECTION **
union arb10AX1 HdLight;                                 // Headlights.
union arb3B3X0 WarnBzr;                                 // Warning Beeper.
union arb3B8X0 HighBm;                                  // High Beams.
union arb383X0 TSignal;                                 // Turn Signals.
union arb3C1X0 Pk_Brake;                                // Parking Brakes, Brake Fluid.
union arb420X4 MaxT_OD_CEL;                             // Overdrive, Max Temp, Check Engine.
union arb420X5 Charge_PwrTrain;                         // Charge System, PowerTrain.
union arb420X6 Cruise_SecSys;                           // Cruise Control, Security System.
union arb445X0 OilP;                                    // Oil Pressure Gauge.

// ****************************************************************************************************************************

// ** CAN-BUS HARDWARE CONSTANTS **
const unsigned int SPI_CS_PIN_MS = MS_CAN_CS;           // MS CAN-Bus CS pin. 
const unsigned int SPI_CS_PIN_HS = HS_CAN_CS;           // HS CAN-Bus CS pin.
//const unsigned int HS_INTR_PIN = ?;                   // HS CAN-Bus Interrupt Pin. (NOT USED)
//const unsigned int MS_INTR_PIN = ?;                   // MS CAN-Bus Interrupt Pin. (NOT USED)

// ****************************************************************************************************************************

// ** MS CAN-BUS ARBID CONSTANTS **
const unsigned int ARB_LIGHTS  = 0x10A;                 // MS CAN-Bus, Headlight Control & Back-light Intensity.
const unsigned int ARB_TURNSIG = 0x383;                 // MS CAN-Bus, Turn Signal Indicators.
const unsigned int ARB_TPM   = 0x3A5;                   // MS CAN-Bus, Tire Pressure Monitor.
const unsigned int ARB_DOORS = 0x3B1;                   // MS CAN-Bus, Doors (Message Center).
const unsigned int ARB_BEEP  = 0x3B3;                   // MS CAN-Bus, Fault Warning Beep.
const unsigned int ARB_HBEAM = 0x3B8;                   // MS CAN-Bus, High Beam Indicator.
const unsigned int ARB_Pk_Brk= 0x3C1;                   // MS CAN-Bus, Parking Brake Indicator.
const unsigned int ARB_FUEL  = 0x400;                   // MS CAN-Bus, Fuel Gauge.
const unsigned int ARB_OIL   = 0x445;                   // MS CAN-Bus, Oil Gauge.

// HS CAN-BUS ARBID CONSTANTS
const unsigned int ARB_SPD_RPM = 0x201;                 // HS CAN-Bus, Speedometer & RPM.
const unsigned int ARB_TEMP_OD_CEL = 0x420;             // HS CAN-Bus, Coolant Temp Gauge, Odometer cycle, O/D & CEL.

// ****************************************************************************************************************************

// ** CAN-BUS DATA CONSTANTS SECTION **

// BACKLIGHT CONSTANTS (Headlights must be On)
const byte FULLBRIGHT    = 0xff;
const byte FULLDIM       = 0x00;
const unsigned long BackLightCtrlTime = 5000;           // Backlight Intensity Control Timeout for IR Remote, in mS.
const unsigned int BackLightLvlStep = 25;               // Backlight Intensity Level Change, per step. For IR Remote.

// BRAKE SYSTEM CONSTANTS
const bool PARKBRAKE_OFF = 0;                           // HandBrake Off.
const bool PARKBRAKE_ON  = 1;                           // HandBrake On, Chime if moving.
const bool BRAKEFLUID_OFF= 0;                           // Brake Fluid Normal, OK.
const bool BRAKEFLUID_ON = 1;                           // Brake Fluid Indicator On, Message Center warning.

// CHARGE SYSTEM INDICATOR CONSTANTS
const bool CHARGESYS_OFF = 0;
const bool CHARGESYS_ON  = 1;

// CHECK ENGINE LIGHT (CEL) INDICATOR
const byte CEL_OFF  = 0x00;
const byte CEL_ON   = 0x01;
const byte CEL_BLINK= 0x02;

// COOLANT TEMPERATURE CONSTANTS
const byte COLDTEMP = COLD_TEMP;                        // Below Zero Temperature Gauge.
const byte COOLTEMP = COOL_TEMP;                        // Zero (0%) Temperature Gauge.
const byte LOWTEMP  = LOW_TEMP;                         // One-Quarter (25%) Temperature Gauge.
const byte MEDTEMP  = MED_TEMP;                         // Centered (50%) Temperature Gauge.
const byte WARMTEMP = WARM_TEMP;                        // Three-Quarters (75%) Temperature Gauge.
const byte HIGHTEMP = HIGH_TEMP;                        // High Temperature (100%) without Red Indicator.
const byte OVERTEMP = OVER_TEMP;                        // High Temperature (101%) with Red Indicator.
const byte MAXTEMP_OFF = 0x00;                          // Turn off Temperature Over-ride.
const byte MAXTEMP_ON1 = 0x01;                          // Temperature Override #1, Force Maximum (faux 100%).
const byte MAXTEMP_ON2 = 0x02;                          // Temperature Override #2, Force Maximum (faux 100%).

// CRUISE CONTROL INDICATOR CONSTANTS
const bool CRUISE_CTRL_OFF = CRUISE_OFF;                // Cruise Control Indicator On.
const bool CRUISE_CTRL_ON  = CRUISE_ON;                 // Cruise Control Indicator Off.

// DOOR-TRUNK CONSTANTS
const byte DRVOPEN  = 0x80;                             // Driver Door Open.
const byte PASOPEN  = 0x60;                             // Passenger Door Open.
const byte TRNKOPEN = 0x02;                             // Trunk Open.
const byte DOORCLOSE= 0x00;                             // All Closed. 

// FUEL GAUGE CONSTANTS
const byte FUELFLAG = 0xFE;                             // Fuel State Flag.
const unsigned int FUEL0  = 0x011F;                     // Fuel 0%;
const unsigned int FUEL50 = 0x01cf;                     // Fuel 50%;
const unsigned int FUEL55 = 0x01df;                     // Fuel 55%;
const unsigned int FUEL56 = 0x0225;                     // Fuel 56%;
const unsigned int FUEL75 = 0x026b;                     // Fuel 75%
const unsigned int FUEL100= 0x02cf;                     // Fuel 100%;
const float FUELSCALE = 3.52f;                          // Fuel Gauge Scale Factor.

// HEADLIGHT CONSTANTS
const bool HEADLIGHTS_OFF= HLIGHTS_OFF;
const bool HEADLIGHTS_ON = HLIGHTS_ON;
const bool HIGHBEAM_OFF  = HBEAM_OFF;
const bool HIGHBEAM_ON   = HBEAM_ON;

// ODOMETER CONSTANTS
const float ODOMSCALE = 321000.0f;                      // Scaled uS Delay for 120mph Speedo, Odometer 3-Phase Cycle Timer.

// OIL PRESSURE IDIOT GAUGE CONSTANTS
const bool OILPRESS_ON = OIL_ON;                        // Oil Pressure On.
const bool OILPRESS_OFF= OIL_OFF;                       // Oil Pressure Off.

// OVERDRIVE INDICATOR CONSTANTS
const byte OD_OFF  = 0x00;
const byte OD_ON   = 0x01;
const byte OD_BLINK= 0x02;

// POWERTRAIN INDICATOR CONSTANTS
const bool PWRTRAIN_OFF = PTRAIN_OFF;
const bool PWRTRAIN_ON  = PTRAIN_ON;

// SECURITY INDICATOR CONSTANTS
const byte SECURITY_OFF  = 0x00;
const byte SECURITY_ON   = 0x01;
const byte SECURITY_BLINK= 0x02;

// SPEEDOMETER GAUGE CONSTANTS
const unsigned int MAXSPEED = 120;                      // Maximum Speed printed on Speedometer.

// TACHOMETER GAUGE CONSTANTS
const unsigned int MAXRPM = 7000;                       // Maximum RPM on Tachometer.

// TIRE PRESSURE MONITOR CONSTANTS
const byte TIRE_OK   = 0x00;
const byte TIRE_LOW  = 0x50;
const byte TIRE_FAULT= 0x10;
const byte TPM_FLASH = 0x08;
const byte TPM_FAULT = 0x04;

// TURN SIGNAL INDICATOR CONSTANTS
const byte TURNSIG_OFF   = 0x00;
const byte TURNSIG_RIGHT = 0x01;
const byte TURNSIG_LEFT  = 0x02;
const byte TURNSIG_EMERGENCY= 0x03;

// WARNING BEEPER CONSTANTS
const bool WARNBEEP_OFF = BEEP_OFF;                     // Warning Beeper Off (Silent warnings).
const bool WARNBEEP_ON  = BEEP_ON;                      // Warning Beeper On.

// ****************************************************************************************************************************

// ** HARDWARE SETUP AND SYSTEM INITIALIZATION SECTION

void setup()
{
    static unsigned int RetryCnt = 0;
    
    UserCmd.reserve(32);                                // Reserve string space, minimize memory re-allocations.
    AnimationMode = AnimateFixed;                       // Set Cluster to Pre-Set Fixed Speed and RPM, without animation.
    FuelRstFlag = false;
    CLI_PwrFlag = false;                                // CLI Cluster Power State Off.
    LED_PWM_Level = PWM_Max/4;                          // Message Center & Speaker LED, Low Brightness Level.
    ClusterDefault(true);                               // Init Cluster Gauges and indicators with default idle state CAN-Bus data.

    #ifdef MP3PLAYER
     unsigned int MP3Volume = MP3_DEF_VOL;              // MP3 Player Volume. 
    #endif

    pinMode(StartSwPin,INPUT_PULLUP);
    pinMode(RunSwPin,INPUT_PULLUP);
    pinMode(MP3BusyPin,INPUT);
    pinMode(LED_Pin, OUTPUT);
    pinMode(AmpPwrPin,OUTPUT);
    pinMode(IcPwrPin,OUTPUT);
    analogWrite(LightPwrPin,LED_PWM_Level);             // Set LED Brightness in Message Center Sw's and Speakers.
    digitalWrite(LED_Pin, LOW);
    
    SetAmplifierPower(AmpRlyOff);                       // Turn Off Amplifier Power (avoid reset Pop).
    SetClusterPower(ClusterRlyOff);                     // Turn Off Instrument Cluster Power Relay.

    Serial.begin(SERIAL_BAUD,SERIAL_CONFIG);            // USB Serial Baud & Config for serial port.
    PrintVersion();                                     // Print the Project info to the serial port.
    serial_manager.println(F("Start of System Initialization")); 

    Serial.println(F("Initializing MS CAN-Bus ..."));   
    while (CAN_OK != CAN_MS.begin(CAN_125KBPS,CAN_XTAL)) { // Initialize MS CAN-Bus.
        digitalWrite(LED_Pin,!(digitalRead(LED_Pin)));  // Toggle LED.
        Serial.println(F("  MS CAN-Bus Shield init has failed"));   
        delay(250);
        String InitStr = F("->Init MS CAN-Bus Shield, retry #");
        InitStr = InitStr + RetryCnt++;
        Serial.println(InitStr);
    }

    Serial.println(F("Initializing HS CAN-Bus ..."));
    while (CAN_OK != CAN_HS.begin(CAN_500KBPS,CAN_XTAL)) { // Initialize HS CAN-Bus.
        digitalWrite(LED_Pin,!(digitalRead(LED_Pin)));  // Toggle LED.
        Serial.println(F("  HS CAN-Bus Shield init has failed"));  
        delay(250);
        String InitStr = F("->Init HS CAN-Bus Shield, retry #");
        InitStr = InitStr + RetryCnt++;
        Serial.println(InitStr);
    }
    
    Serial.println(F("MS & HS CAN-Bus Shields Have Been Initialized"));
    Serial.flush();
    Serial.end();
    
    serial_manager.start();                             // Start Command Line Processor.
    serial_manager.setFlag(CMD_EOL_TERM);               // Set EOL Termination character.
    serial_manager.println(F("Serial Manager CLI is Enabled")); // We're alive!

    #ifdef IRCONTROL 
     irrecv.enableIRIn();                               // Start the IR Remote Receiver.
     serial_manager.println(F("IR Remote Receiver is Enabled"));
    #endif
    
    #ifdef MP3PLAYER
     Serial3.begin(MP3_SER_BAUD,MP3_SER_CONFIG);        // Init Serial3 for MP3 Player.
     serial_manager.print(F("MP3 Player Initialization: "));     
     if (!myDFPlayer.begin(Serial3,true,true)) {        // MP3: Serial3, Ack, Reset.
        serial_manager.println(F("Fail!"));
        serial_manager.println(F(" ->Please recheck the MP3 connections"));
        serial_manager.println(F(" ->Please insert SD card with MP3 files"));
        serial_manager.println(F(" ->Use file format unsigned 16-bit 32KHz"));
     }
     else {
//      serial_manager.println(F("Success!"));
        Print_MP3_Detail(myDFPlayer.readType(), myDFPlayer.read());
     }
     myDFPlayer.setTimeOut(250);                        // Set MP3's serial time-out, in mS.
     myDFPlayer.EQ(DFPLAYER_EQ_BASS);                   // We want deep bass.
     myDFPlayer.volume(MP3Volume);                      // Set MP3 Volume.
     myDFPlayer.play(MP3_FILE_HELLO);                   // Play the Welcome announcement, MP3.
     SetAmplifierPower(AmpRlyOn);                       // Turn On Amplifier Power (avoid reset Pop).
    #endif
 
    digitalWrite(LED_Pin, HIGH);                        // LED On.
    delay(1500);                                        // Allow Cluster to Power-Up & Initialize.

    serial_manager.println(F("Instrument Cluster Ready"));
    
    AmpPwrTimer = millis() + AMPPWR_TIME;               // Initialize Audio Amplifier Power Shutoff Timer.
    BlinkerTime = millis();                             // Initialize Turn Signal Blinker Timer.
    CycleTime = micros();                               // Initialize CAN-Bus Cycle (refresh) Timer.
    IR_PwrTimer = millis();                             // Initialize Cluster Power Shutoff Timer.
    OdoTime = micros();                                 // Initialize Odometer Phase Timer.
    SeqHoldTimer= millis();                             // Initialize Animation Sequencer Hold Timer.
    
    serial_manager.println(F("End of System Initialization\r\n"));    
    
    return;
}


// ****************************************************************************************************************************
// ** THE MAIN LOOP SECTION **

void loop()
{
    
    if(millis() > BlinkerTime + BLINKER_TIME) {         // Turn Signal Blinker Timer.
        BlinkerTime = millis();
        BlinkFlag = ! BlinkFlag;                        // Toggle Turn Signal Blinker state.
    }

    #ifdef IRCONTROL     
    RemoteControl();                                    // Process IR Remote Commands.
    #endif
    
    if(HS_Print_Flag) Print_HSCAN_Data();               // Display HS CAN-Bus Data (CLI diagnostic feature).
    if(MS_Print_Flag) Print_MSCAN_Data();               // Display MS CAN-Bus Data (CLI diagnostic feature).
        
    UpdateAmpPwr();                                     // Update Audio Amplifier Power Relay (On/Off).
    EngineStartUp();                                    // Check for Ignition Key-Start.
    UpdatePowerState();                                 // Update Cluster Power Relay (Off/Run/Start).
    ProcessCommands();                                  // Process User Commands (via CLI).   
    OdometerPhaseCycler();                              // Service Odometer 3-Phase state processor.
    SendCanBus();                                       // Periodically Transmit CAN-Bus Data.
    AnimateSequencer();                                 // Perform Cluster Animations.
}


// ============================================================================================================================
// EOF
