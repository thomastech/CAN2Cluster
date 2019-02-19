# CAN2Cluster Command Line Interface (CLI)

### Introduction
The Command Line Interface (CLI) provides serial terminal access to the Instrument Cluster. It allows manual control of the gauges 
and indicator lights. Serial communication is through the Arduino's built-in USB Com port.

The serial port can also be used to monitor the runtime log messages. During operation the log messages are sent in real-time to report actions and status.

### Communication Requirements
Many popular serial terminal programs can be used. A convenient choice is the Serial Monitor built into the Arduino IDE (Tools->Serial Monitor).  

The basic communication settings are 115200 baud, 8 bits, 1 stop, no parity, no flow. All commands must be terminated with CR (ASCII 13). Do **NOT** send Newline (ASCII 10).  

**Serial Terminal Setting Examples:**

**1. Arduino Serial Monitor:**  
  - [x] Autoscroll Enabled  
  - [x] Carriage return  
  - [x] 115200 baud  

**2. PuTTY:**  
  - [x] 115200 baud, 8 bits, 1 stop  
  - [x] Parity None, Flow Control None  
  - [x] Implicit LF Off  
  - [x] Implicit CR Off  

### Command Query
All commands support a status query function that returns its current parameter value. Example: BF,? returns the Brake Fluid parameter.

### Command Summary
- `?`= List all commands (Help Screen)  
- `H`= List all commands (alternate to '?')  
- `BF`= Brake Fluid (Off/On)  
   Usage: BF,[0-1]  
   Example: BF,1  
   Action: Turn Brake Fluid Warning Message On  
   Status Query: `BF,?` returns the state (hex value)  
- `BL`= Back Light (0-100%)  
   Usage: BL,[0-100]  
   Example: BL,75  
   Action: Set Night Time Backlight intensity to 75%  
   Status Query: `BL,?` returns intensity's hex value  
   Notes: Headlights must be on (command: HL,1).  
-  `CC`= Cruise Control Indicator (Off / On)  
   Usage: CC,[0-1]  
   Example: CC,1  
   Action: Turn On Cruise Control Indicator  
   Status Query: `CC,?` returns the state (hex value)  
- `CE`= Check Engine Light (Off / On / Blink)  
   Usage: CE,[0-2]  
   Example: CE,2  
   Action: Blink Check Engine Indicator  
   Status Query: `CE,?` returns the state (hex value)  
- `CP`=Cluster Power Relay Control (Off / On)  
   Usage: CP,[0-1]  
   Example: CP,1  
   Action: Turn On Cluster Power.  
   Status Query: `CP,?` returns "Off" / "On"  
   Notes:  Ignition key (Run position) overrides this setting.  
- `CS`= Charge System Warning (Off / On)  
   Usage: CS,[0-1]  
   Example: CS,1  
   Action: Turn On Charge Indicator  
   Status Query: `CS,?` returns off/on state (hex value)  
- `CT`= Coolant Temperature (0-100%)  
   Usage: CT,[0-100]  
   Example: CT,75  
   Action: Set Coolant Gauge to 75%  
   Status Query: `CT,?` returns temperature's hex value  
- `DT`= Door & Trunk Ajar Warning (off / driver|pass|trunk)  
   Usage: DT,[0-3]  
   Example: DT,1  
   Action: Turn On Driver Door Ajar Warning Message  
   Status Query: `DT,?` returns the state (hex value)  
   Notes: 0=Clear Door Warning. Driver=1, Passenger=2, Trunk=3 (Or'd together).  
- `FT`= Fuel Tank Level (0-100%, Reset)  
   Usage: FT,[0-100,R]  
   Example: FT,75  
   Action: Set Fuel Tank Level to 75%  
   Status Query: `FT,?` returns the fuel level (hex value)  
   Notes: New fuel level value updates will be delayed several minutes by Slosh 
   Protection. For immediate update use the reset parameter (FT,R) after
   changing the level.  
- `HB`= High Beam Indicator (Off / On)  
   Usage: HB,[0-1]  
   Example: HB,1  
   Action: Turn On High Beam Indicator  
   Status Query: `HB,?` returns the state (hex value)  
- `HL`= Head Lights (Off / On)  
   Usage: HL,[0-1]  
   Example: HL,1  
   Action: Turn On Head Lights  
   Status Query: `HL,?` returns the state (hex value)  
   Notes: This command controls the daytime and nighttime backlighting.
- `MT`= Maximum Temperature Gauge (Off / On / On)  
   Usage: MT,[0-2]  
   Example: MT,2  
   Action: Unconditionally Force Coolant Temperature Gauge to Maximum  
   Status Query: `MT,?` returns the state (hex value)  
   Notes: The MT On command overrides the CT value.  
- `OD`= Over-Drive Indicator (Off / On / Blink)  
   Usage: OD,[0-2]  
   Example: OD,2  
   Action: Blink the Over-Drive Indicator  
   Status Query: `OD,?` returns the state (hex value)  
- `OP`= Oil Pressure (Off / On)  
   Usage: OP,[0-1]  
   Example: OP,1  
   Action: Turn On Oil Pressure Gauge  
   Status Query: `OP,?` returns the state (hex value)  
   Notes: The Oil Pressure Gauge has only one active position.  
- `PB`= Parking Brake Indicator (Off / On)  
   Usage: PB,[0-1]  
   Example: PB,1  
   Action: Turn On Parking (hand) Brake indicator. If Speed is >1 then chime is heard.  
   Status Query: `PB,?` returns the state (hex value)  
- `PT`= Power-Train Fault (Off / On)  
   Usage: PT,[0-1]  
   Example: PT,1  
   Action: Turn On Powertrain Fault indicator (wrench icon)  
   Status Query: `PT,?` returns the state (hex value)  
   Notes: Power reset is required to clear the fault.  
- `SP`= Speed (0-120mph)  
   Usage: SP,[0-120]  
   Example: SP,75  
   Action: Set Speedometer Gauge to 75mph  
   Status Query: `SP,?` returns MPH (decimal value)  
- `SS`= Security System Indicator (Off / On / Blink)  
   Usage: SS,[0-2]  
   Example: SS,2  
   Action: Blink the Security Indicator  
   Status Query: `SS,?` returns the state (hex value)  
- `TP`= Tire Pressure Monitor (Off / Low / Fault / Low-fault / Flash)  
   Usage: TP,[0-4]  
   Example: TP,4  
   Action: Blink the TPM indicator  
   Status Query: `TP,?` returns the state (hex value)  
   Notes: TPM warnings also reported in Message Center.
- `TR`= Tachometer RPM (0-7000)  
   Usage: TR,[0-7000]  
   Example: TR,5000  
   Action: Set Tachometer Gauge to 5000 RPM.  
   Status Query: `TR,?` returns RPM (decimal value)  
- `TS`= Turn Signal Indicators (Off / Right / Left / Emergency)  
   Usage: TS,[0-3]  
   Example: TS,3  
   Action: Blink both right and left turn indicators  
   Status Query: `TS,?` returns the state (hex value)  
- `WB`= Warning Buzzer (Off / On)  
   Usage: WB,[0-1]  
   Example: WB,0  
   Action: Disable the warning buzzer noise  
   Status Query: `WB,?` returns the state (hex value)  
   
### Diagnostic Commands
There are several diagnostic commands. As follows:  
- `CBINF`= CAN-Bus Information (HS / MS / HS-MS)  
  Usage CBINF,[0-2]  
  Example: CBINF,0  
  Action: Show HS CAN-Bus Online / Offline State  
- `HSCAN`= Hight Speed CAN-Bus Data Monitor (Off / On)  
  Usage HSCAN,[0-1]  
  Example: HSCAN,1  
  Action: Turn on HS CAN-Bus Data Monitor  
- `MSCAN`= Medium Speed CAN-Bus Data Monitor (Off / On)  
  Usage MSCAN,[0-1]  
  Example: MSCAN,1  
  Action: Turn on MS CAN-Bus Data Monitor  
- `RUNAN`= Silently Run Animation (STOP / FIXED / REV / DRV / TEST / USER1 / USER2)  
  Usage: RUNAN[0-6]  
  Example: RUNAN,2  
  Action: Run the Engine Rev Animation  
  Notes: No MP3 audio, runs silently.
- `RDAT`= Reset CAN-Bus Data to Defaults  
  Usage: RDAT  
  Action: All Gauges and Indicator reset to default state.  
  Notes: Fuel=50%, everything else off. 
- `VERS`= Show Version  
  Example: VERS  
  Action: Display Version splash message  
