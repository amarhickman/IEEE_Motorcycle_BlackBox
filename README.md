# IEEE_Motorcycle_BlackBox
2026 IEEE Motorcycle Black Box Porject

This repo will serve as documentation of code base for the ESP32s used during the development phase.

The prupose of this project is to create a crash box for motorcycles. This box should be able to detect when a crash has happened and send a corresponding emergency alert. 

Main Module:
This module will be the main entity of the project. This will have a custom 3d printed enclosure for all components. Components will be soldered onto a vector board. This module will have the ability to detect g forces and device angles and log date in live time.
- ESP32
- Gyroscope
- Accelerometor
- SD card module  

Crash Demo:
This module will be demonstrate the purpose of the acceleromter. This will be done by attaching module onto an RC Car that will bump into a wall at a designated speed. This data will be written and displayed on visual graphs to showcase the accleration and g force spike under impact.
- ESP32
- Accelerometor 
- SD card module 

Angle Demo:
This module will serve as a demonstration of how the gyroscope will track the devices angle. A custom 3d printed carnival like game will be created. Users will have to balance a round spindle with a gyroscope attached. There will be a live telemtary data screen showcasing the live angle of the device. The goal of the user will be to balance the device until a certain angle is achieved.
- ESP32
- Gyroscope
- 3d printed housing
