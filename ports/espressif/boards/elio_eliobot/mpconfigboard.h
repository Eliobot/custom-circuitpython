// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2019 Scott Shawcroft for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#pragma once

// Micropython setup

#define MICROPY_HW_BOARD_NAME       "Eliobot V1"
#define MICROPY_HW_MCU_NAME         "ESP32S2"
#define CIRCUITPY_DRIVE_LABEL       "ELIOBOT"

#define MICROPY_HW_NEOPIXEL (&pin_GPIO1)

#define DEFAULT_I2C_BUS_SCL (&pin_GPIO9)
#define DEFAULT_I2C_BUS_SDA (&pin_GPIO8)

#define DEFAULT_SPI_BUS_SCK (&pin_GPIO30)
#define DEFAULT_SPI_BUS_MOSI (&pin_GPIO32)
#define DEFAULT_SPI_BUS_MISO (&pin_GPIO31)

#define DEFAULT_UART_BUS_RX (&pin_GPIO44)
#define DEFAULT_UART_BUS_TX (&pin_GPIO43)
