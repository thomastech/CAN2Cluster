# CAN2Cluster Arduino Code

### Introduction
The CAN2Cluster project uses an Arduino MEGA 2560 Rev3 CPU board. 
All the software source files are provided so you can make changes for your application. 

Please do NOT create a GitHub issue ticket if you have technical questions or seek advice. The place for 
that is at the official project discussion area found at the rc-cam.com forum:   
https://www.rc-cam.com/forum/index.php?/topic/4171-can2cluster-technical-details-mustang-instrument-cluster/

### The Fine Print
This project is not intended for Arduino beginners. If your Arduino experience is limited then start with a 
simpler project. There are many Arduino forums and YouTube instructional videos that will help get you going. 
To put it bluntly, don't use the CAN2Cluster community to ask for general Arduino help.

### Arduino IDE Version
The project successfully compiles on Arduino IDE version 1.8.5. Other versions might experience compile build problems and 
if this occurs please install 1.8.5 as a workaround.

### Arduino Libraries
Special libraries must be installed to compile the code. You can use the IDE's `Sketch->Include_Library->Manage_Libraries` and 
`Sketch->Include_Library->Manage_Libraries->Add_.ZIP_Library` to add them.

Required Libraries:
* CAN-BUS V1.0.0, https://github.com/Seeed-Studio/CAN_BUS_Shield
* RBD_SerialManager V1.0.0-alpha.3, http://robotsbigdata.com/docs-arduino-serial-manager.html
* DFRobotDFPlayerMini V1.0.3, https://github.com/DFRobot/DFRobotDFPlayerMini
* IRremote V2.2.3, https://github.com/z3t0/Arduino-IRremote

### Directory Information
The Arduino sketch files must be copied to an Arduino project folder named 'Can2Cluster_Ford'. Do not use any other directory name!

