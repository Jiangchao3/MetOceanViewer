# MetOceanViewer
Multipurpose tool for viewing hydrodynamic model data, such as ADCIRC, with a gui interface. The code is written in Qt C++ and is designed to run on Windows, Mac OSX, and Linux systems.

# Feature List
- [x] Access and view NOAA station data
- [x] Access and view USGS station data
- [x] Read ADCIRC fort.61 files (when accompanied by station file)
- [x] Read ADCIRC netCDF formatted files
- [x] Read/write IMEDS formatted data
- [x] Read HWM comparisons, plot to map, and calculate statistics

# Planned Upgrades
- [ ] Read Delft3D native formatted NEFIS files
- [ ] Allow download of longer periods of data from NOAA interface
- [ ] Remove restriction on files having same locations
- [ ] Add interface to NOAA Currents stations
- [ ] Add data to session files upon request of user

# Screenshots
![Screenshot 1](https://github.com/zcobell/MetOceanViewer/blob/master/screenshots/mov_ss1.JPG)
![Screenshot 2](https://github.com/zcobell/MetOceanViewer/blob/master/screenshots/mov_ss2.JPG)
![Screenshot 3](https://github.com/zcobell/MetOceanViewer/blob/master/screenshots/mov_ss3.JPG)

# Compiling
To compile this program, please use the Qt suite of tools. The MetOceanViewer.pro file has been tested using QtCreator and Qt version 5.4.0 on Windows, Mac OSX, and Linux.

# License
This program conforms to the GNU GPL, Version 3. 