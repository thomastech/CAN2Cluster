// File: IR_Control.cpp. Used in Can2Cluster.ino project.
// Purpose: IR Remote Control (Optional)
// Author: T. Black
// Created: Jan-07-2019
// Last Change: Mar-21-2019. Revised RemoteControl().
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

#include "Can2Cluster_Ford.h"
#include "IR_Control.h"

// ============================================================================================================================
#ifdef IRCONTROL 
// RemoteControl(): Process the IR Remote commands. All hand-held controller buttons are used.
void RemoteControl(void) 
{
    unsigned long KeyCode;
    
    if (irrecv.decode(&IRdataRx)) {
        KeyCode = IRdataRx.value;                       // Get IR Key Code.

// Un-comment the following code block to log the data packet details on each IR Command.
/*        
        serial_manager.print(F("Remote IR Code: "));
        PrintHex(KeyCode,0,true);
        serial_manager.print(F(" Data length: "));
        serial_manager.println(String(IRdataRx.rawlen));
        serial_manager.print(F(" Type: "));
        serial_manager.println(String(IRdataRx.decode_type));
*/
        irrecv.resume();                                // Resume IR Remote decoding.
    
        if(KeyCode == RC_REP) {                         // Ignore the REPEAT key code.
            return;                                     // Nothing to do, exit.
        }
        
        if(ClusterPwr == ClusterRlyOff){     // Power is off.
            IR_PwrTimer = millis() + IRPWR_TIME;
            AmpPwrTimer = millis() + AMPPWR_TIME;
            serial_manager.println(F("IR Remote: Automatic Power-Up"));
        }
        else if(RunSwitch == RunSwOff && CLI_PwrFlag == false) {
            IR_PwrTimer = millis() + IRPWR_TIME;
            AmpPwrTimer = millis() + AMPPWR_TIME;
            serial_manager.println(F("IR Remote: Refresh Timeout"));
        }
            
        serial_manager.print(F("REMOTE CMD: "));
        
        if(KeyCode == RC_Button1) {                     // Play Rev #1 Animation Sequence.
            Run_Sequencer(SequencerStop,0);             // Kill any running Animations.
            AnimationMode = AnimateRev1;
            SeqRunOnce = true;                          // Tell Sequencer to Run Once.
            serial_manager.print(F("Play Engine Rev #1"));
            #ifdef MP3PLAYER
             myDFPlayer.play(MP3_FILE_REV1);            // Play the Engine REV #1 sound, MP3.
            #endif
        }
        else if(KeyCode == RC_Button2) {                // Play Car Driving Animation Sequence.
            Run_Sequencer(SequencerStop,0);             // Kill any running Animations.
            AnimationMode = AnimateDrive;
            SeqRunOnce = true;                          // Tell Sequencer to Run Once.
            serial_manager.print(F("Play Car Drive"));
            #ifdef MP3PLAYER
             myDFPlayer.play(MP3_FILE_DRIVE);           // Play the CAR DRIVE sounds, MP3.
            #endif
        }
        else if(KeyCode == RC_Button3) {                // Play Rev #2 Animation Sequence.
            Run_Sequencer(SequencerStop,0);             // Kill any running Animations.
            AnimationMode = AnimateRev2;
            SeqRunOnce = true;                          // Tell Sequencer to Run Once.
            serial_manager.print(F("Play Engine Rev #2"));
            #ifdef MP3PLAYER
             myDFPlayer.play(MP3_FILE_REV2);            // Play the Engine REV #2 sound, MP3.
            #endif
        }
        else if(KeyCode == RC_Button4) {                // Play Fixed RPM/Speed Gauge Sequence.
            Run_Sequencer(SequencerStop,0);             // Kill any running Animations.
            AnimationMode = AnimateFixed;
            SeqRunOnce = true;                          // Tell Sequencer to Run Once.
            serial_manager.print(F("Play Fixed Speed & RPM"));
            #ifdef MP3PLAYER
             myDFPlayer.play(MP3_FILE_FIX);             // Play the announcement, MP3.
            #endif
        }
        else if(KeyCode == RC_Button5) {                // Play Preset #1 Sequence.
            Run_Sequencer(SequencerStop,0);             // Kill any running Animations.
            AnimationMode = AnimateUser1;
            SeqRunOnce = true;                          // Tell Sequencer to Run Once.
            serial_manager.print(F("Play User Pre-Set #1"));
            #ifdef MP3PLAYER
             myDFPlayer.play(MP3_FILE_PRESET1);         // Play the PRESET-1 Announcement, MP3.
            #endif
        }
        else if(KeyCode == RC_Button6) {                // Play Preset #2 Sequence.
            Run_Sequencer(SequencerStop,0);             // Kill any running Animations.
            AnimationMode = AnimateUser2;
            SeqRunOnce = true;                          // Tell Sequencer to Run Once.
            serial_manager.print(F("Play User Pre-Set #2"));
            #ifdef MP3PLAYER
             myDFPlayer.play(MP3_FILE_PRESET2);         // Play the PRESET-2 Announcement MP3.
            #endif
        }
        else if(KeyCode == RC_Button7) {                // Play Gauge Sweep Test Sequence.
            Run_Sequencer(SequencerStop,0);             // Kill any running Animations.
            AnimationMode = AnimateTest;
            SeqRunOnce = true;                          // Tell Sequencer to Run Once.
            serial_manager.print(F("Play Gauge Sweep Test"));
            #ifdef MP3PLAYER
             myDFPlayer.play(MP3_FILE_GTEST);           // Play the ANIMATE Announcement, MP3.
            #endif
        }
        else if(KeyCode == RC_Button8) {                // Play Welcome Message.
            if(digitalRead(MP3BusyPin) == MP3BusyOn) {
                #ifdef MP3PLAYER
                 serial_manager.print(F("Busy, Cannot Play Welcome Message"));
                 myDFPlayer.advertise(MP3_FILE_AD_HIBEEP);// Barge-in with Beep Sound, MP3.
                 SeqHoldTimer= millis()+MP3_PAUSE_HIBEEP; // Pause the Sequencer while Beep plays.
                #else
                 serial_manager.print(F("Invalid Request, MP3 Player not installed"));
                #endif
            }
            else {
                Run_Sequencer(SequencerStop,0);         // Kill any running Animations.
                ClusterDefault(true);                   // Load default CAN-Bus Data.
                FuelRstFlag = true;                     // Force Fuel Level update.
                #ifdef MP3PLAYER
                 serial_manager.print(F("Play Welcome Message & Load Defaults"));
                 myDFPlayer.play(MP3_FILE_HELLO);       // Play the ANIMATE Announcement, MP3.
                #else
                  serial_manager.print(F("Load Defaults"));
                #endif
            }
        }
        else if(KeyCode == RC_Button9) {                // Play Engine Stop Sequence.
            Run_Sequencer(SequencerStop,0);             // Kill any running Animations.
            AnimationMode = AnimateStop;
            SeqRunOnce = true;                          // Tell Sequencer to Run Once.
            serial_manager.print(F("Play Engine Stop"));
            #ifdef MP3PLAYER
             myDFPlayer.play(MP3_FILE_STOP);            // Play the STOP Announcement, MP3.
            #endif
        }
        else if(KeyCode == RC_Button0) {                // Pound Key, Toggle Cluster's Backlight (Night-time / Day-time).
            serial_manager.print(F("Backlight "));
            if(HdLight.ctrl.Headlights == HEADLIGHTS_OFF) {
                HdLight.ctrl.Headlights = HEADLIGHTS_ON;
                #ifdef MP3PLAYER
                 if(digitalRead(MP3BusyPin)==MP3BusyOn) {
                    myDFPlayer.advertise(MP3_FILE_AD_BLOOP);// Barge in with Bloop Sound, MP3.
                    SeqHoldTimer = millis()+MP3_PAUSE_BLOOP;// Pause the Sequencer while Bloop plays.
                 }
                 else {
                    myDFPlayer.play(MP3_FILE_NIGHTON);  // Play Announcement, MP3.
                 }
                #endif
                serial_manager.print(F("Nighttime (Dimmable Color)"));
            }
            else {
                HdLight.ctrl.Headlights = HEADLIGHTS_OFF;
                #ifdef MP3PLAYER
                 if(digitalRead(MP3BusyPin)==MP3BusyOn) {
                    myDFPlayer.advertise(MP3_FILE_AD_BLOOP);// Barge in with Bloop Sound, MP3.
                    SeqHoldTimer = millis()+MP3_PAUSE_BLOOP;// Pause the Sequencer while Bloop plays.
                 }
                 else {
                    myDFPlayer.play(MP3_FILE_DAYON);    // Play Announcement, MP3.
                 }
                #endif
                serial_manager.print(F("Daytime (Bright White)"));
            }
        }
        else if(KeyCode == RC_UP) {                     // Vol+.
            #ifdef MP3PLAYER
             if(MP3Volume < MP3_MAX_VOL) MP3Volume++;
             myDFPlayer.volume(MP3Volume);              // Command MP3 Player Volume.
             if(digitalRead(MP3BusyPin)==MP3BusyOn) {
                myDFPlayer.advertise(MP3_FILE_AD_HIBEEP);// Barge in with Chirp Sound, MP3.
                SeqHoldTimer= millis()+MP3_PAUSE_HIBEEP; // Pause the Sequencer while Beep plays.
             }
             else {
                myDFPlayer.play(MP3_FILE_HIBEEP);       // Play Beep Sound, MP3.
             }
             serial_manager.print(F("Vol+ ["));
             serial_manager.print(String(MP3Volume) + F("]"));
            #else
             serial_manager.print(F("Invalid Vol+ Command, MP3 Player Not installed"));
            #endif
        }
        else if(KeyCode == RC_DN) {                     // Vol-.
            #ifdef MP3PLAYER
             if(MP3Volume > MP3_MIN_VOL) MP3Volume--;
             myDFPlayer.volume(MP3Volume);              // Command MP3 Player Volume.
             if(digitalRead(MP3BusyPin)==MP3BusyOn) {
                myDFPlayer.advertise(MP3_FILE_AD_LOBEEP); // Barge in with Beep Sound, MP3.
                SeqHoldTimer = millis()+MP3_PAUSE_LOBEEP; // Pause the Sequencer while Beep plays.
             }
             else {
                myDFPlayer.play(MP3_FILE_LOBEEP);       // Play Beep Sound, MP3.
             }
             serial_manager.print(F("Vol- ["));
             serial_manager.print(String(MP3Volume) + F("]"));
            #else
             serial_manager.print(F("Invalid Vol- Command, MP3 Player Not installed"));
            #endif
        }
        else if(KeyCode == RC_OK) {                     // OK Key, Honk Horn.
            #ifdef MP3PLAYER 
                serial_manager.print(F("Honk Horn"));
             if(digitalRead(MP3BusyPin)==MP3BusyOn) {
                myDFPlayer.advertise(MP3_FILE_AD_HORN); // Barge in with Horn Sound, MP3.
                SeqHoldTimer = millis() + MP3_PAUSE_HORN; // Pause the Sequencer while Horn plays.
             }
             else {
                myDFPlayer.play(MP3_FILE_HORN);         // Play Horn Sound, MP3.
             }
            #else
             serial_manager.print(F("Invalid Horn Command, MP3 Player Not installed"));
            #endif
        }
        else if(KeyCode == RC_RT) {
            serial_manager.print(F("Turn Signal "));
            if(TSignal.ctrl.TurnSig != TURNSIG_RIGHT) {
                TSignal.ctrl.TurnSig = TURNSIG_RIGHT;
                serial_manager.print(F("Right"));
            }
            else {
                TSignal.ctrl.TurnSig = TURNSIG_OFF;
                serial_manager.print(F("Off"));
            }
        }
        else if(KeyCode == RC_LT) {
            serial_manager.print(F("Turn Signal "));
            if(TSignal.ctrl.TurnSig != TURNSIG_LEFT) {
                TSignal.ctrl.TurnSig = TURNSIG_LEFT;
                serial_manager.print(F("Left"));
            }
            else {
                TSignal.ctrl.TurnSig = TURNSIG_OFF;
                serial_manager.print(F("Off"));
            }
        }
        else if(KeyCode == RC_ButtonS) {                // Change Accessory Lights Intensity (Message Center & Speaker LEDs).
            #ifdef MP3PLAYER 
             if(digitalRead(MP3BusyPin)==MP3BusyOn) {
                myDFPlayer.advertise(MP3_FILE_AD_BLOOP); // Barge in with Bloop Sound, MP3.
                SeqHoldTimer = millis()+MP3_PAUSE_BLOOP; // Pause the Sequencer while Bloop plays.
             }
             else {
                myDFPlayer.play(MP3_FILE_BLOOP);        // Play Bloop Beep Sound, MP3.
             }
            #endif
        
            LED_PWM_Level += LedLvlStep;                // Next Intensity Level.
            if(LED_PWM_Level > PWM_Max) LED_PWM_Level = LedLvlStep;
            analogWrite(LightPwrPin,LED_PWM_Level);     // Set LED Intensity on Message Center Sw's and Speakers.
            serial_manager.print(F("LED Intensity "));
            serial_manager.print(String((LED_PWM_Level/LedLvlStep)*(100/(PWM_Max/LedLvlStep))));
            serial_manager.print(F("%"));
        }
        else if(KeyCode == RC_ButtonP) {                // Change Cluster Backlight Intensity.
            #ifdef MP3PLAYER
             if(digitalRead(MP3BusyPin)==MP3BusyOn) {
                myDFPlayer.advertise(MP3_FILE_AD_BLOOP);// Barge in with Bloop Sound, MP3.
                SeqHoldTimer = millis()+MP3_PAUSE_BLOOP;// Pause the Sequencer while Bloop plays.
             }
             else if(HdLight.ctrl.Headlights == HEADLIGHTS_OFF) {
                myDFPlayer.play(MP3_FILE_TURNON2);      // Play "Turn On Night Lights" Announcement, MP3.
             }
             else {
                myDFPlayer.play(MP3_FILE_BLOOP);        // Play Bloop Beep Sound, MP3.
             }
            #endif
            serial_manager.print(F("Backlight Intensity ")); 
            if(HdLight.ctrl.Headlights == HEADLIGHTS_ON) {
                BackLightLvl += BackLightLvlStep;
                if(BackLightLvl > FULLBRIGHT) BackLightLvl = BackLightLvlStep;
                serial_manager.print(String((BackLightLvl/BackLightLvlStep)*(100/(FULLBRIGHT/BackLightLvlStep))));
                serial_manager.print(F("%"));
            }
            else {
                serial_manager.print(F("Command Ignored, Turn On Night Time Lights!")); 
            }
        }
        else if (KeyCode != RC_REP) {
            serial_manager.print(F("Un-Assigned Key, Code: "));
            PrintHex(KeyCode,0,false);
        }

        serial_manager.println(F(""));
    }
    
    return;
}

#endif
// ============================================================================================================================
