# FIRST Tech Challenge Scoring Software for Rover Ruckus

This project was created to serve as a final project for CS126 Software Design Studio.

This project allows one to run a fully functional FTC scoring system. FTC is a worldwide robotics competition where middle school and high school students build robots with which to compete in a yearly challenge. This system allows for:

* Creating tournaments
* Adding teams to tournaments
* Generating match schedules
* Entering scores for the Rover Ruckus season
* Viewing team rankings

# Installing

This code makes use of the [Nana](http://nanapro.org/en-us/) C++ library. This library can be installed with the [Vcpkg](https://github.com/Microsoft/vcpkg) package manager. To use, run the following from the Vcpkg installation directory.

'''
.\vcpkg install nana:x64-windows
.\vcpkg integrate install
'''

Also, you need to install the [MySQL](https://www.mysql.com/) server and configure your project to use the [Connector C++](https://dev.mysql.com/doc/connector-cpp/8.0/en/connector-cpp-apps-windows-visual-studio.html) library. Then, build the project in Visual Studio and try it out.