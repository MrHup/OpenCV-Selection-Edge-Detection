# OpenCV-Selection-Edge-Detection
A personal project built to play around with OpenCV and MySQL functionalities in C++

The program allows the user to define zones of interest on a live video stream. Then, these zones are used and analyzed such that the information obtained regarding movement and edge count in the selected areas are passed to a DataBase from which it can be displayed using a visualization tool or another piece of software.

We provide a simple interface for initial configurations and support:
- brightness and canny filter in-app tweaking
- configuration saving/loading
- dynamic interaction with the defined zones (add, delete, update)

## Prerequisites

Install and configure OpenCV. For simplicity I recommend using this repo as a shortcut: https://github.com/jayrambhia/Install-OpenCV ;
Install and configure MySQL (tested for version 14.14, Distrib 5.7.27);
In the main.cpp file, modify lines 26-29 accordingly;
Comment line 141 in main.cpp and replace it with the next one if you want to use a live video feed instead of a webcam.

## Installation

The application requires OpenCV 3.4.3 or higher and compile the code with C++11.
As for the OS, the program was tested on Ubuntu 16.04 and we found it incompatible with Ubuntu 18.04.

Run with:
```
g++ -std=c++11 -o output main.cpp InterestZone.cpp menu.cpp display.cpp -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql `pkg-config --cflags --libs opencv`
```

## Functionality

Once installed, execute the generated output and you will be prompted with a window. Either press 1 to load a new configuration, 2 for an existing one or 3 to view the different controls. From there, interact with the application and reduce the noise by playing with the settings.
