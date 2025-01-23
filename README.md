> [!IMPORTANT]
> This project is not finished yet and is available at a very early stage of testing.
>
> I only work on this project when i have time and feel like it. 

# ScrnshotMgr (Screenshot Manager)
ScrnshotMgr is an open-source gui app made in Qt6 for taking screenshots with ease.

Additionally features minimal video recording (WIP) for taking clips

This is a hobby project because i wanted to replace windows snipping tool and not use sharex, as well as learn Qt and C++.

![App Preview](./doc/preview.png)

## Features:

- Screenshot capture list
- Preset/Region capture
- Preview region of capture

## Building/Contributing

### Windows

- Install Qt 6.8.1 and windeployqt using maintenancetool.exe
- Add windeployqt to PATH
- `git clone https://github.com/Panniku/Scrnshotmgr`
- Run the application via QtCreator at least once
- `windeploy [PATH TO PROJECT]/build/[BUILD DIRECTORY WITH EXECUTABLE]`

### Linux

- Install `qt6`, `cmake` and `make` using your distribution's package manager
- `git clone https://github.com/Panniku/Scrnshotmgr`
- `cd [PATH TO PROJECT] && mkdir build && cd /build && cmake .. && make`
- run the executable (differs on dists? fix later)

### MacOS

run the project through qtcreator i guess
