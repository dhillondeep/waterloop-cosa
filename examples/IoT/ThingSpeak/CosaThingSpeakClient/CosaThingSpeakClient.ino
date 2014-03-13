/**
 * @file CosaThingSpeakClient.ino
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014, Mikael Patel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * @section Description
 * ThingSpeak talkback demonstration; shows how to create a command
 * handler class, execute commands from the TalkBack command queue,
 * and push additional commands to the queue. Uses an Alarm for
 * periodic servicing of commands. The sketch will also post
 * sensor reading to a ThingSpreak channel.
 *
 * @section Circuit
 * The sketch uses Board::D15 (A1) as output pin for the LED.
 * Commands LED_ON/LED_OFF will turn on and off the LED.
 * The sketch also uses Board::EXT0 (D2) for DHT11 data.
 *
 * This file is part of the Arduino Che Cosa project.
 */

#include "Cosa/RTC.hh"
#include "Cosa/Event.hh"
#include "Cosa/Watchdog.hh"
#include "Cosa/IoT/ThingSpeak.hh"
#include "Cosa/Socket/Driver/W5100.hh"
#include "Cosa/Trace.hh"
#include "Cosa/IOStream/Driver/UART.hh"

#include "CommandHandler.h"
#include "SensorHandler.h"
#include "LED.h"
#include "Ping.h"

// Ethernet controller
static const char HOSTNAME[] __PROGMEM = "CosaThingSpeakClient";
W5100 ethernet;

// Thingspeak Client, TalkBack and Channel
ThingSpeak::Client client;

static const char CHANNEL_KEY[] __PROGMEM = "I3U14KLWM1R1SDPR";
ThingSpeak::Channel channel(&client, CHANNEL_KEY);

static const char TALKBACK_KEY[] __PROGMEM = "72PKOHOK3DKJW2TQ";
uint16_t TALKBACK_ID = 206;
ThingSpeak::TalkBack talkback(&client, TALKBACK_KEY, TALKBACK_ID);

// TalkBack commands
const char REBOOT_COMMAND[] __PROGMEM = "REBOOT";
Command reboot(&talkback, REBOOT_COMMAND);

const char LED_ON_COMMAND[] __PROGMEM = "LED_ON";
LED led_on(&talkback, LED_ON_COMMAND, Board::D15, 1);

const char LED_OFF_COMMAND[] __PROGMEM = "LED_OFF";
LED led_off(&talkback, LED_OFF_COMMAND, Board::D15, 0);

const char PONG_COMMAND[] __PROGMEM = "PONG";
Command pong(&talkback, PONG_COMMAND);
Ping ping(&talkback, &pong);

// The period alarm handlers; sensor and command handling
SensorHandler sensor_handler(&channel, Board::EXT0, 20);
CommandHandler command_handler(&talkback, 15);

// Use the default alarm scheduler
Alarm::Scheduler scheduler;

void setup()
{
  // Setup trace iostream to serial port
  uart.begin(9600);
  trace.begin(&uart, PSTR("CosaThingSpeakClient: started"));

  // Start the watchdog, real-time clock and the alarm scheduler
  Watchdog::begin(16, SLEEP_MODE_IDLE, Watchdog::push_timeout_events);
  RTC::begin();
  scheduler.begin();

  // Setup Ethernet controller and ThingSpeak with given ethernet socket 
  TRACE(ethernet.begin_P(HOSTNAME));
  TRACE(client.begin(ethernet.socket(Socket::TCP)));

  // Add the commands
  talkback.add(&reboot);
  talkback.add(&led_on);
  talkback.add(&led_off);
  talkback.add(&ping);
  talkback.add(&pong);

  // Enable the command and sensor handler
  command_handler.enable();
  sensor_handler.enable();
}

void loop()
{
  // The standard event dispatcher
  Event event;
  Event::queue.await(&event);
  event.dispatch();
}
