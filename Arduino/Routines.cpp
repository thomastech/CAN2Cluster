// File: Routines.cpp. Used in Can2Cluster.ino project.
// Purpose: Misc Sub-Routines.
// Author: T. Black
// Created: Jan-07-2019
// Last Change: Mar-21-2019. 
//              Revised EngineStartUp(), UpdatePowerState(). 
//              Added SetAmplifierPower(), SetClusterPower().
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

//
// ============================================================================================================================

#include "Can2Cluster_Ford.h"

const unsigned long RUN_TIME = 500UL;                   // Ignition Run Time, in mS. Used to heavily debounce the start switch.
const unsigned long START_TIME = 90000UL;               // Ignition Start Time, in mS. Start Sw idle lapse time.

// ============================================================================================================================
// EngineStartUp(): Start the engine when the Ignition switch is in the START position.
// There are two different gauge animation sequences to play. If there has been a long lapse of time since the last engine 
// start (>START_TIME) then use the Engine Rev sequence. If there has been a recent start then toggle between the Engine 
// Rev and Car Driving animation sequences.
void EngineStartUp(void)
{
    bool StartSw = StartSwOff;                          // Switch Status State.
    static bool OldStartSw = StartSwOff;                // Saves Previous Switch Status State.
    static unsigned int StartDebnc = 0;                 // Start Switch Debounce.
    static unsigned int StartState = 0;                 // State counter for which Engine Sequence to use.
    static unsigned long StartTimer = millis();
    static unsigned long RunTimer = millis();

    StartSw = digitalRead(StartSwPin);                  // Get Ignition Key Start State.
    
    if(RunTimer > millis()) {                           // Debouncing key-Start switch. Do nothing, exit.
        return;
    }
    else if(StartSw != OldStartSw) {                    // Ignition Start Key Switch changed state.
        if(StartDebnc < 5) {                            // Check multiple times for transient noise filtering.
            StartDebnc++;
            return;                                 
        }
        
        RunTimer = millis() + RUN_TIME;
        serial_manager.print(F("Ignition Key Start: "));
        
        if(StartSw == StartSwOn) {                      // New Ignition Start.
            OldStartSw = StartSw;
            serial_manager.println(F("On"));

            if(StartTimer < millis()) {                 // Been awhile since last Start.
                StartState = 0;                         // Reset State to use first Engine Sequence.
            }
            else if(StartSw == StartSwOn) {             // New Key Start, play next Engine Sequence.
                if(++StartState>1) {
                    StartState = 0;
                }
            }
//          serial_manager.print(F("Ignition Start State: "));
//          serial_manager.println(String(StartState));
            
            Run_Sequencer(SequencerStop,0);             // Kill any running Animations.
            
            if(StartState == 0) {
                AnimationMode = AnimateRev1;
                SeqRunOnce = true;                      // Tell Sequencer to Run Once.
                serial_manager.println(F("Play Engine Rev #1"));
                #ifdef MP3PLAYER
                 myDFPlayer.play(MP3_FILE_REV1);        // Play the Engine REV #1 sound, MP3.
                #endif
            }
            else {
                AnimationMode = AnimateDrive;
                SeqRunOnce = true;                      // Tell Sequencer to Run Once.
                serial_manager.println(F("Play Car Drive"));
                #ifdef MP3PLAYER
                 myDFPlayer.play(MP3_FILE_DRIVE);       // Play the CAR DRIVE sounds, MP3.
                #endif
            }

        }
        else {
            OldStartSw = StartSw;
            serial_manager.println(F("Off"));
        }

        StartTimer = millis() + START_TIME;
    } 
    else {
        StartDebnc = 0;                                 // Reset Start Switch Debounce counter.
    }
    
    return;
}

// ============================================================================================================================

// isNumeric(): Return true if the string is all numeric.
bool isNumeric(String str)
{
    for(byte i=0;i<str.length();i++) {
        if(!isDigit(str.charAt(i))) return false;
    }
    return true;
}

// ============================================================================================================================

// PrintHex(): Print number in hex format.
// The bytecnt is the byte size of the hex value. Set lf to true for linefeed.
// One and two bytecnt values are formatted with a leading 0x. All others omit it.
void PrintHex(unsigned long dec, byte bytecnt, bool lf)
{
    String Str;
    byte Value;

    if(bytecnt == 1 && dec <= 0xffff) {                 // One Hex Byte to print.
        dec = (unsigned int)(dec);
        Value = lowByte(dec);                           // Use Only Low Byte.
        if(Value == 0) {
            Str = "0x00";
        }
        else if(Value <= 0x0f){
            Str = "0x0" + String(Value,HEX);
        }
        else {
            Str = "0x" + String(Value,HEX);
        }
        serial_manager.print(Str);
    }
    else if(bytecnt == 2 && dec <= 0xffff) {            // Two Hex bytes to print.
        dec = (unsigned int)(dec);
        Value = highByte(dec);                          // High Byte.
        if(Value == 0) {
            Str = "0x00";
        }
        else if(Value <= 0x0f) {
            Str = "0x0" + String(Value,HEX);
        }
        else {
            Str = "0x" + String(Value,HEX);
        }
        serial_manager.print(Str);

        Value = lowByte(dec);                           // Low Byte.
        if(Value == 0) {
            Str = "00";
        }
        else if(Value <= 0x0f) {
            Str = "0" + String(Value,HEX);
        }
        else {
            Str = String(Value,HEX);
        }
        serial_manager.print(Str);
    }
    else {                                              // Print unformatted hex value.
        Str = String(dec,HEX);
        serial_manager.print(Str);
    }
    
    if(lf) serial_manager.println("");                  // New line.

    return;
}

// ============================================================================================================================

// PrintVersion(): Print the Project information and Version to the serial port, a.k.a. Splash Screen.
void PrintVersion(void)
{
    Serial.println("");
    Serial.println(F("*****************************************"));
    Serial.println(F("* INSTRUMENT CLUSTER CAN-BUS CONTROLLER *"));
    Serial.println(F("*****************************************"));
    Serial.println(F(" FOR 2007-2009 FORD MUSTANG, 6-GAUGES"));
    Serial.print(F(" by T. Black, "));
    Serial.println(VERSION_STR + DATE_STR);
    Serial.println(F(" GNU General Public License Version 3"));
    Serial.print(F(" Project Blog Site: "));
    Serial.println(PROJECT_STR);
    Serial.print(F(" GitHub Repository: "));
    Serial.println(GITHUB_STR + F("\r\n"));
    return;
}

// ============================================================================================================================

// Print_MP3_Detail(): Print the DFPlayer Mini MP3 status.
// This function will be omitted if MP3PLAYER is undefined in Config.h.
#ifdef MP3PLAYER
void Print_MP3_Detail(uint8_t type, int value)
{
  switch (type) {
    case TimeOut:
      serial_manager.println(F("Time Out!"));
      break;
    case WrongStack:
      serial_manager.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      serial_manager.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      serial_manager.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      serial_manager.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      serial_manager.print(F("Number:"));
      serial_manager.print(value);
      serial_manager.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      serial_manager.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          serial_manager.println(F("Card not found"));
          break;
        case Sleeping:
          serial_manager.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          serial_manager.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          serial_manager.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          serial_manager.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          serial_manager.println(F("Cannot Find File"));
          break;
        case Advertise:
          serial_manager.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
#endif


// ============================================================================================================================
// SetAmplifierPower(): Set Audio Amp Power Relay On/Off.
void SetAmplifierPower(bool state)
{
    AmplifierPwr = state;
    digitalWrite(AmpPwrPin,state);                      // Turn Off/On Amplifier Power Relay.

    if(digitalRead(AmpPwrPin) != state) {               // Hardware problem.
        serial_manager.print(F("Hardware Error: AmpPwrPin Stuck "));
        if(state == HIGH) {
            serial_manager.println(F("Low"));
        }
        else {
            serial_manager.println(F("High"));
        }
    }

    return;
}

// ============================================================================================================================
// SetClusterPower(): Set Cluster Power On/Off.
void SetClusterPower(bool state)
{
    ClusterPwr = state;
    digitalWrite(IcPwrPin,state);                       // Turn Off/On Instrument Cluster.

    if(digitalRead(IcPwrPin) != state) {                // Hardware problem.
        serial_manager.print(F("Hardware Error: IcPwrPin Stuck "));
        if(state == HIGH) {
            serial_manager.println(F("Low"));
        }
        else {
            serial_manager.println(F("High"));
        }
    }

    return;
}

// ============================================================================================================================

// UpdateAmpPwr(): Check to see if Amplifier Power is required (Controls Amplifier Power Relay On/Off states).
void UpdateAmpPwr(void)
{
    if(millis() < AmpPwrTimer) {                        // Automatic Power-on Timer is active.
        SetAmplifierPower(AmpRlyOn);                    // Turn On Amplifier Power.
    }
    else if(RunSwitch == RunSwOn) {
        AmpPwrTimer = millis() + AMPPWR_TIME;           // Refresh Audio Amplifier Power Shutoff Timer.
        SetAmplifierPower(AmpRlyOn);                    // Turn On Amplifier Power.
    }
    else if(CLI_PwrFlag == true){                       // CLI Power Mode active.
        AmpPwrTimer = millis() + AMPPWR_TIME;           // Refresh Audio Amplifier Power Shutoff Timer.
        SetAmplifierPower(AmpRlyOn);                    // Turn On Amplifier Power.
    }
    else {
        analogWrite(LightPwrPin,FULLDIM);               // Turn Off Lights in Message Center Sw's and Speakers.
        SetAmplifierPower(AmpRlyOff);                   // Turn Off Amplifier Power.
    }

    return;
}

// ============================================================================================================================

// UpdatePowerState(): Check the Ignition Switch, CLI Power Mode, and IR Automatic Power and use their present
// states to Control Cluster Power Relay (On/Off).
void UpdatePowerState(void)
{    
    static bool OldCLIPwr = CLI_PwrFlag;                // Save current CLI Power state.
    static bool IR_PwrFlag = false;                     // IR Remote's Automatic Power Mode.
    static bool OldIgnSwitch = RunSwOff;                // Save current Switch state.
    static byte OldLedPWM = LED_PWM_Level;              // Save current LED Brightness Level.
    static unsigned int OldBlLvl = BackLightLvl;        // Save current Cluster Backlight Level (night mode).
    static unsigned int RunDebnc = 0;                   // Run Switch Debouncer.
    static unsigned long SwTmr = millis();              // Routine Timer, in mS.
    
    if(millis() <  SwTmr + 50) {                        // 50mS Updates.
        return;
    }
    
    SwTmr = millis();                                   // New Update Timer value.

    if(digitalRead(RunSwPin) == RunSwOn) {
        if(RunDebnc < 5) {                              // Check multiple times for transient noise filtering.
            RunDebnc++;
        }
        else {
            RunSwitch = RunSwOn;
        }
    }
    else {
        if(RunDebnc > 0) {                              // Check multiple times for transient noise filtering.
            RunDebnc--;
        }
        else {
            RunSwitch = RunSwOff;
        }
    }
    
    if(RunSwitch == RunSwOn){                        // Ignition Switch is On.
        IR_PwrTimer = millis();                         // Reset Automatic Power-up Timer.
        IR_PwrFlag = false;                             // Disable Automatic Power-up.
        if(OldIgnSwitch != RunSwitch) {              // Switch changed states.
            OldIgnSwitch = RunSwitch;
            CLI_PwrFlag = false;                        // Override CLI Power Control.
            OldCLIPwr = false;
            LED_PWM_Level = OldLedPWM;                  // Restore LED Intensity
            BackLightLvl = OldBlLvl;                    // Restore Cluster Backlighting intensity.
            HdLight.ctrl.Headlights = HEADLIGHTS_OFF;   // Change cluster backlight to bright white.
            serial_manager.println(F("Power State: Run Switch On"));
        }
        else if (LED_PWM_Level < LedLvlStep){
            LED_PWM_Level = LedLvlStep;                 // Low intensity Message Center & Speaker LEDs.
        }
        analogWrite(LightPwrPin,LED_PWM_Level);         // Update LED Brightness in Message Center Sw's and Speakers.
        SetClusterPower(ClusterRlyOn);                    // Turn On Instrument Cluster Power Relay.
    }
    else if(CLI_PwrFlag == true){                       // CLI Power Mode active.
        IR_PwrTimer = millis();                         // Reset Automatic Power-up Timer.
        IR_PwrFlag = false;                             // Disable Automatic Power-up.
        if(OldCLIPwr != CLI_PwrFlag) {                  // CLI mode changed states.
            OldCLIPwr = true;
            LED_PWM_Level = OldLedPWM;                  // Restore LED Intensity
            analogWrite(LightPwrPin,LED_PWM_Level);     // Update LED Brightness in Message Center Sw's and Speakers.
            BackLightLvl = OldBlLvl;                    // Restore Cluster Backlighting intensity.
            serial_manager.println(F("Power State: CLI On"));
        }
        SetClusterPower(ClusterRlyOn);                    // Turn On Instrument Cluster Power Relay.
    }
    else if(millis() < IR_PwrTimer)  {
        if(IR_PwrFlag == false) {
            IR_PwrFlag = true;
            LED_PWM_Level = OldLedPWM;                  // Restore LED Intensity
            analogWrite(LightPwrPin,LED_PWM_Level);     // Update LED Brightness in Message Center Sw's and Speakers.
            BackLightLvl = OldBlLvl;                    // Restore Cluster Backlighting intensity.
            serial_manager.println(F("Power State: Remote On"));
        }
        SetClusterPower(ClusterRlyOn);                    // Turn On Instrument Cluster Power Relay.
    }
    else {
        IR_PwrTimer = millis();                         // Reset Automatic Power-up Timer.
        if(OldIgnSwitch != RunSwitch) {              // Switch changed states, Power Off.
            Run_Sequencer(SequencerStop,0);             // Kill any running Animations.
            ClusterDefault(false);                      // Reset Gauges and indicators, but leave fuel level as-is.
            OldIgnSwitch = RunSwitch;
            OldBlLvl = BackLightLvl;
            OldLedPWM = LED_PWM_Level;
            LED_PWM_Level = LedLvlStep/3;               // Reduce Message Center & Speaker LED.
            analogWrite(LightPwrPin,LED_PWM_Level);     // Update LED Brightness in Message Center Sw's and Speakers.
            HdLight.ctrl.Headlights = HEADLIGHTS_ON;    // Change cluster backlight to dimmable color.
            BackLightLvl = FULLBRIGHT/2;
            serial_manager.println(F("Power State: Run Switch Off"));
            SetClusterPower(ClusterRlyOff);               // Turn Off Instrument Cluster Power Relay.
            #ifdef MP3PLAYER
             myDFPlayer.stop();                         // Kill MP3 sounds.
            #endif
        }
        else if(OldCLIPwr != CLI_PwrFlag) {             // CLI Power changed to Off State.
            OldCLIPwr = CLI_PwrFlag;
            OldBlLvl = BackLightLvl;
            OldLedPWM = LED_PWM_Level;
            LED_PWM_Level = LedLvlStep/3;               // Reduce Message Center & Speaker LED.
            analogWrite(LightPwrPin,LED_PWM_Level);     // Update LED Brightness in Message Center Sw's and Speakers.
            HdLight.ctrl.Headlights = HEADLIGHTS_ON;    // Change cluster backlight to dimmable color.
            BackLightLvl = FULLBRIGHT/2;
            SetClusterPower(ClusterRlyOff);               // Turn Off Instrument Cluster Power Relay.
        }
        else if(IR_PwrFlag == true) {                   // The Remote's Automatic timer has ended.
            IR_PwrFlag = false;
            OldBlLvl = BackLightLvl;
            OldLedPWM = LED_PWM_Level;
            LED_PWM_Level = LedLvlStep/3;               // Reduce Message Center & Speaker LED.
            analogWrite(LightPwrPin,LED_PWM_Level);     // Update LED Brightness in Message Center Sw's and Speakers.
            HdLight.ctrl.Headlights = HEADLIGHTS_ON;    // Change cluster backlight to dimmable color.
            BackLightLvl = FULLBRIGHT/2;
            #ifdef MP3PLAYER
             myDFPlayer.play(MP3_FILE_SHUTDN);          // Announce shutdown, MP3.
            #endif
            serial_manager.println(F("Power State: Automatic Off"));
            SetClusterPower(ClusterRlyOff);               // Turn Off Instrument Cluster Power Relay.
        }
    }
    
    return;
}
// ============================================================================================================================

