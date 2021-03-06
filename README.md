# MetOceanViewer
[![Build Status](https://travis-ci.com/zcobell/MetOceanViewer.svg?branch=master)](https://travis-ci.com/zcobell/MetOceanViewer)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/2d817318594440a0834476c9b06de219)](https://www.codacy.com/app/zachary.cobell/MetOceanViewer?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=zcobell/MetOceanViewer&amp;utm_campaign=Badge_Grade)

Multipurpose tool for viewing hydrodynamic model data, such as ADCIRC, with a gui interface. The code is written in Qt C++ and is designed to run on Windows, Mac, and Linux systems.

<img src="https://github.com/zcobell/MetOceanViewer/blob/master/MetOceanViewer/img/workswith.png" alt="Works with Windows, Mac, and Linux" width="200">

# Feature List
- [x] Access and view NOAA station data
- [x] Access and view USGS station data
- [x] Access and view NDBC archive station data
- [x] Create tide predictions using XTide
- [x] Access the CRMS database
- [x] Read ADCIRC fort.61 files (when accompanied by station file)
- [x] Read ADCIRC netCDF formatted files
- [x] Read/write IMEDS formatted data
- [x] Read/write generic netcdf formatted data
- [x] Read HWM comparisons, plot to map, and calculate statistics

# Screenshots
![Screenshot 1](https://github.com/zcobell/MetOceanViewer/blob/master/MetOceanViewer/screenshots/mov_ss1.JPG)
![Screenshot 2](https://github.com/zcobell/MetOceanViewer/blob/master/MetOceanViewer/screenshots/mov_ss2.JPG)
![Screenshot 3](https://github.com/zcobell/MetOceanViewer/blob/master/MetOceanViewer/screenshots/mov_ss3.JPG)

# Compiling
To compile this program, please use the Qt suite of tools. The MetOcean.pro file has been tested using QtCreator and Qt version 5.14.1 on Windows 10, Mac OSX 10.15.3 and Ubuntu Linux 18.04. It is expected that other versions and flavors of Linux should not pose an issue. Mac OSX is expected to work, however, some tweaks may be required. I don't currently have a development environment for Mac, so assistance with this would be appreciated. 

Other versions of Qt that include the QtCharts library should work, but have not been tested.

## netCDF
You will need to have netCDF version 4+ installed on your system with acccess to the headers and libraries. You may need to edit the netCDF path in MetOceanViewer.pro to correctly locate these files. When compiling for Windows, these libraries are already included in the package. However, Unix/Mac users will need to ensure they are in their LD_LIBRARY_PATH variable or configure the .pro file accordingly.

# MapBox
An interface to using MapBox based maps is provided. Use the Options menu to enter your MapBox API key, which can be obtained for free from their website.

# CRMS
Louisiana's Coastwide Reference Monitoring System provides an excellent resource for water level and water quality data. This data is processed weekly from a provided text file into netCDF and posted [here](https://metoceanviewer.s3.amazonaws.com/crms.nc). The processed database can be re-downloaded each week when it is updated.

# Credits
The following external libraries/APIs are used in this software

|Package|Source|
|-------|------|
| Qt | http://www.qt.io |
| NOAA CO-OPS API | http://tidesandcurrents.noaa.gov|
| USGS Waterdata API | http://waterdata.usgs.gov|
| XTide |http://www.flaterco.com/xtide/|
| CRMS | https://lacoast.gov/crms/ |
| NDBC | https://www.ndbc.noaa.gov/ |
| MapBox | https://www.mapbox.com/ |
| netCDF v4.3.3 | http://www.unidata.ucar.edu/software/netcdf)
| zlib v1.2.8 |http://www.zlib.net|
| OpenSSL v1.1.1d |https://www.openssl.org/|
| EzProj | http://github.com/zcobell/EzProj| 

# License
This program conforms to the GNU GPL, Version 3.
