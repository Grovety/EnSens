# EnSens

[![License](https://img.shields.io/badge/License-GPL%203.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

## Table of content

- [EnSens](#ensens)
  - [Table of content](#table-of-content)
  - [About](#about)
  - [Environment, Compiling and running the code](#environment-compiling-and-running-the-code)

## About
Precise air condition monitoring module featuring Bosch BME688 for measurement accuracy.

- AQI (Air Quality Index)
- VOC (Volatile Organic Compounds)
- CO2 (Carbon Dioxide)
- Temperature
- Pressure
- Humidity
- 
Open-source firmware and ready-to-use demo app for Android.

## Environment, Compiling and running the code
### 1. Development Environment
#### Git
- Download and install [Git](https://git-scm.com/downloads)

#### Visual Studio Code
- Download and install [Visual Studio Code](https://code.visualstudio.com/)
- Install Extension [nRF Connect for Visual Studio Code](https://marketplace.visualstudio.com/items?itemName=nordic-semiconductor.nrf-connect)
- Set up nRF Connect SDK v2.9.1 and nRF Connect SDK Toolchain v2.9.1. Follow the [instructions on how to set up SDK and toolchain](https://docs.nordicsemi.com/bundle/nrf-connect-vscode/page/get_started/quick_setup.html)

#### Flashing tool
- Download and install [AuTerm](https://github.com/thedjnK/AuTerm/releases/tag/v0.35a-pre). You can also use other [tools](https://docs.zephyrproject.org/latest/services/device_mgmt/mcumgr.html#tools-libraries) for flashing

### 2. Get source code
To get the source code, you need to clone the repository from GitHub. Open your terminal and run the following command:

```console
git clone --recursive https://github.com/Grovety/EnSens.git
```

**_NOTE:_**
It’s important to use the --recursive flag when cloning the repository, which will automatically clone the submodules. If you forget to use this flag, you can manually clone the submodules by running
```console
git submodule update --init --recursive
```
in the root directory of the repository.

### 3. Compiling
- Open Visual Studio Code
- Open the nRF Connect extension in Visual Studio Code by clicking its icon in the [Activity Bar](https://docs.nordicsemi.com/bundle/nrf-connect-vscode/page/reference/ui_overview.html)
- In the [Welcome View](https://docs.nordicsemi.com/bundle/nrf-connect-vscode/page/reference/ui_sidebar_welcome.html), click on **Open an existing application**
- Select the EnSens folder
- In the [Applications View](https://docs.nordicsemi.com/bundle/nrf-connect-vscode/page/reference/ui_sidebar_applications.html), click on **Add build configuration**
- Select the board target **ensens_nrf52833/nrf52833**
- Make sure the **Build after generating configuration** checkbox is selected
- Click the **Build Configuration** button

This generates the configuration file and triggers the build process, which can take some time. You can monitor its progress in the notification that appears.

### 4. Flashing
- Press the button on the board
- Connect the board via USB Micro. After the connection is established (the LED on the board should be on continuously), release the button
- Open AuTerm
- Select the Config tab (1)
- Change the Config Port Settings to the port the board is connected to (2). You can view the port in the Device Manager (for Windows) under the Ports tab, there should be a USB Serial Device. At the bottom (3) it should say MCUBOOT (ZEPHYR) [board hardware id]. Note that the port number will vary from machine to machine and board to board
- Go to the MCUmgr tab (4)

![AuTerm Config tab](docs/images/AuTerm_Config_tab.png?raw=true)

- Select UART (5)
- Press Connect (6)
- Select Upload tab (7)
- Choose the firmware file EnSens/build/ensensefw/zephyr/zephyr.signed.bin (8)
- Select No action (9)
- Press Go (10). After that the firmware download should start. Wait until the progress bar is 100% full
- Go to the OS tab (11)

![AuTerm MCUmgr, Img tab](docs/images/AuTerm_MCUmgr_Img_tab.png?raw=true)

- Select the Reset tab (12)
- Press Go (13)

![AuTerm MCUmgr, OS tab](docs/images/AuTerm_MCUmgr_OS_tab.png?raw=true)

After that, an error message will appear due to the board resetting. Close it.

![AuTerm error](docs/images/AuTerm_error.png?raw=true)

- Go to the Terminal tab (14)
- Press Clear (15)
- Press Open Port (16)

![AuTerm Terminal tab](docs/images/AuTerm_Terminal_tab.png?raw=true)

Verify that the LED blinks once every 3 seconds and the terminal has messages of the following type:

```console
[00:03:50.818,786] <inf> app: temp: 22.899051; press: 99131.242187; humidity: 22.842298; iaq: 50; CO2: 500.000000; VOC: 0.499999; battery_percent: 99.90
[00:03:50.819,580] <inf> BATTERY: raw 13650 ~ 2999 mV
```

![AuTerm Terminal tab with log](docs/images/AuTerm_Terminal_tab_with_log.png?raw=true)
