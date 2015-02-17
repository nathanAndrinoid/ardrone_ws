This is a catkin_make directory for automated control of the tum_simulator ardrone. 

In order to use this starter code to build your drone controls you will need to do the following:

1.) First clone the repository:

   git clone https://github.com/nathanAndrinoid/ardrone_controller

2.) Open the source main.cpp file in the following directory to edit the code.

   cd ~/ardrone_ws/src/ardrone_controller/src

3.) If you added non local dependancies you may need to edit the
   CMakeList.txt and the package.xml. located in:
   
   cd ~/ardrone_ws/src/ardrone_controller
         
4.) After the desired changes have been made go back to the root folder

   cd ~/ardrone_ws

5.) To build and compile enter:

   catkin_make
   
6.) To run the program after the simulator is started exicute from root directory:

   ./devel/lib/ardrone_controller/ardrone_controller
