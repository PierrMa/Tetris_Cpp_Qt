This project is my first project in C++ with Qt Creator.
The version of Qt I used for this project was Qt Creator 16.0.2 (Community) and I used CMake as builder.

If you try to reproduce this project, this are the steps I followed:
1-Draw the grid
2-Generate a tetromino
3-Move down tetrominos using timer
4-Manage collision
5-Allow movement of tetrominos with keyboard keys
6-Manage the end of the game
7-Manage row deletion when it is complete
8-Implement pause mode
9-Add the display of the score
10-Add the menu (Start, Settings, Quit)
11-Implementer Settings page
12-Generate a professionnel executable

I also complete some extra task (not necessary to already have a great game):
-Allow for command customization
-Add background music and allow to manage mute mode
-Add sound effect for row deletion, game over and button interaction 
-Allow for saving and loading of settings
-Accelerate tetrominos fall over time
-Add score page
-Allow for saving and loading of the 10 best scores
-Allow for game saving

This project helps me to leverage the following knowledges:
-Virtual method
-Method and class surchage
-Inheritance
-QtEvent
-Connection, signals, slots
-Lambda function
-How to use timer
-How to use ressources and add theme to your project (sound, image)
-How to create a project with different pages and navigate between them
-How to save data through sessions with QtSettings and Json files
-File system interaction
-Keyboard and mouse interaction
-How to create tables
-Scripting with CMake
-How to create a project realease
-How to create an installer with QtInstallerFramework
-How to add a custom icon to executable 

The project is organize in the following way:
-main.cpp contains the code to display the window
-MainWindow is a class to manage all the others classes
-Each page got a class:
	-GameBoard for the game
	-MenuWidget displays all the buttons towards the other pages
	-Scores displays the array with the 10 best scores
	-Settings allows to customize the command used for the game and to choose between Mute or Unmute mode
-HoverButton is a custom class to add sound effect when hovering the button created with this class.