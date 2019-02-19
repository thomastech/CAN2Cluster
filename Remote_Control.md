# CAN2Cluster Remote Control Function
### Introduction
The IR Remote Control can be used to activate a variety of instrument cluster functions. The hand-held controller 
has a range of about eight feet (~three meters). It is important to aim it directly at the receiver module's lens.

![ir_remote2_175](https://user-images.githubusercontent.com/10354989/53042862-0ea6b800-343c-11e9-92e6-1884eb9415dc.jpg)

### Useful Information:  
Each button on the hand-held controller is modulated with a unique digital code. An IR sensitive LED receives the 
invisible infrared light beam and the pressed button's value is decoded in a software routine. The button code 
values are defined in the IR_Control.h file and may be changed to match your hand-held controller.

The remote function is optional. The IR receiver module can be omitted which also requires that its software functions 
be disabled. Disabling requires an edit to the Arduino Config.h file (see the file's IRCONTROL section).

### HandHeld Controller's Button Summary:  
- `1`Button = Play Extended Engine Rev Animation.  
- `2`Button = Play Car Driving Animation.  
- `3`Button = Play Short Engine Rev Animation (Repeatable Revs).  
- `4`Button = 3500 rpm at 60 mph.  
- `5`Button = User Preset #1. Default is 7000 rpm, 120 mph, hot coolant warning.  
- `6`Button = User Preset #2. Default is 1000 rpm, 20 mph.  
- `7`Button = Gauge Sweep Test.  
- `8`Button = Reset cluster to default gauge & indicator values. Say Welcome Message.  
- `9`Button = Engine Off.  
- `0`Button = Toggle Headlights (Daytime/Nighttime lighting).  
- `*`Button = Change Cluster Nighttime Backlight Intensity (10% each press).  
- `#`Button = Change Accessory LED Intensity (10% each press).  
- `OK`Button = Honk Horn.  
- ▲ Button = Increase Volume (about 3% each press).  
- ▼ Button = Decrease Volume (about 3% each press).  
- ► Button = Right Turn Signal (press again to turn off).  
- ◄ Button = Left Turn Signal (press again to turn off).  
