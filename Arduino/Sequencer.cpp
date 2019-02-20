// File: Sequencer.cpp. Used in Can2Cluster.ino project.
// Purpose: Animation Sequencer functions.
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

//
// ============================================================================================================================

#include "Can2Cluster_Ford.h"

// ============================================================================================================================

// AnimateSequencer(): Use AnimationMode to pick which Sequence Map array to Run. 
// This is repeatedly called in main loop.
void AnimateSequencer(void)
{
    if (AnimationMode == AnimateRev1) {
        Run_Sequencer(SequencerRev1,SEQ_REV1_STEPS);
    }
    else if (AnimationMode == AnimateRev2) {
        Run_Sequencer(SequencerRev2,SEQ_REV2_STEPS);
    }
    else if(AnimationMode == AnimateDrive) {
        Run_Sequencer(SequencerDrive,SEQ_DRV_STEPS);
    }
    else if(AnimationMode == AnimateFixed) {
        Run_Sequencer(SequencerFixed,SEQ_FIXED_STEPS);
    }
    else if(AnimationMode == AnimateTest) {
        Run_Sequencer(SequencerTest,SEQ_TEST_STEPS);
    }
    else if(AnimationMode == AnimateUser1) {
        Run_Sequencer(SequencerUser1,SEQ_USER1_STEPS);
    }
    else if(AnimationMode == AnimateUser2) {
        Run_Sequencer(SequencerUser2,SEQ_USER2_STEPS);
    }
    else {
        Run_Sequencer(SequencerStop,SEQ_STOP_STEPS);
    }
    
    return;
}

// ****************************************************************************************************************************

// Run_Sequencer(): Cluster Animation Sequencer using map data array. 
// To send the map, global SeqRunOnce must be set true. For example, when the Ignition Switch is moved to the Start Position.
// On Entry, StepCnt is number of data rows in the map array. Set to 0 to stop a running Sequencer.
// On Exit, return parameter is false when map is done (not running).
// A local timer is used to  process the sequence map updates (10Hz).
// The sizeof() function does not work on map inside this function! That is why StepCnt must be passed into the function.
bool Run_Sequencer(const byte map[][SEQ_DATA_CNT], unsigned int StepCnt){
    bool ExitFlag = false;                              // Initialize Exit Flag.
    static unsigned int SequenceStep = 0;               // Animation Sequence Step Count.
    static unsigned long StepRateTmr = millis();
    
    if(StepCnt == 0) {                                  // Reset.
        SequenceStep = 0;
        SeqRunOnce = false;
        StepRateTmr = millis();                         // Refresh local timer.    
        return(ExitFlag);
    }
    else if(SequenceStep == StepCnt+1 && SeqRunOnce) {  // Animated Sequence Complete, starter Key still active.
        SequenceStep = 0;
        StepRateTmr = millis();                         // Refresh local timer.
        serial_manager.println(F("Release Starter Key!"));
        return(ExitFlag);
    }
    else if(SequenceStep >= StepCnt && SeqRunOnce) {    // Animated Sequence Complete, no more data.
        SeqRunOnce = false;                             // Clear Run-Once Flag, no more data to read.
        SequenceStep = StepCnt+1;                       // Trick to detect stuck starter key.
        StepRateTmr = millis();                         // Refresh local timer.
        serial_manager.println(F("Animation Sequence Ended"));
        return(ExitFlag);
    }
    else if(millis() < SeqHoldTimer) {                  // Sequencer hold period.
        StepRateTmr = millis();                         // Refresh local timer.
    }
    else if(millis() > StepRateTmr + (SEQUENCE_DLY*4)){ // We've been idle for awhile.
        StepRateTmr = millis();                         // Refresh local timer.
        SequenceStep = 0;                               // Reset Sequence Step Count, start map data at beginning.
    }
    else if(!SeqRunOnce) {                              // Nothing to do, exit.
        return(ExitFlag);
    }

    if(millis() > (StepRateTmr + SEQUENCE_DLY)) {       // Time for Another Sequence Step, update CAN-Bus data.  
        if(SequenceStep == 0) {
            serial_manager.println(F("Animation Sequence Started"));
        }
        StepRateTmr = millis();        
        Speed = pgm_read_byte(&(map[SequenceStep][0]));
        TachRPM = pgm_read_byte(&(map[SequenceStep][1])) * 100; // Stored RPM Value is RPM/100.
        CoolantTemperature = pgm_read_byte(&(map[SequenceStep][2]));
        OilP.ctrl.OilPress = (bool)(pgm_read_byte(&(map[SequenceStep][3])));
        Charge_PwrTrain.ctrl.Charge = (bool)(pgm_read_byte(&(map[SequenceStep][4])));
        Pk_Brake.ctrl.ParkBrake = (bool)(pgm_read_byte(&(map[SequenceStep][5])));
        if(millis() > SeqHoldTimer) {                     // Not a Sequencer hold period.
            SequenceStep++;
        }
        ExitFlag = true;
    }
    
    return(ExitFlag);
}

// ****************************************************************************************************************************