# pdcApp-ECU1

ECU1 application repository for the PDC (Parking Distance Control) project, developed as part of the **SEA:ME** program.

This repository is used as a submodule of the main PDC repository.

## Contents

| Folder | Description |
|--------|-------------|
| [VehicleControlECU/](VehicleControlECU/) | VehicleControl ECU application — runs on Raspberry Pi 4 |

## Overview

ECU1 runs on a **Raspberry Pi 4** and is responsible for:
- Physical vehicle control (steering, throttle) via PCA9685
- Battery monitoring via INA219
- Gamepad input handling
- CAN bus communication with Arduino (speed, distance)
- SOME/IP service provider to ECU2 (Jetson Orin Nano)
- Camera streaming to ECU2 via RTP/UDP

For full documentation see [VehicleControlECU/README.md](VehicleControlECU/README.md).
