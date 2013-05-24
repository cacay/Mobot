
/* Copyright (c) 2013 Cosku Acay, http://www.coskuacay.com */

#ifndef STD_PID_H
#define STD_PID_H

#include "stdports.h"

/**
 * Setup the PID controller
 * Call this function before using the others
 */
void pidSetup();

/**
 * Changes the speed of the robot
 * @param motorSpeed speed of the robot determines as:
 * 0-127: reverse; 128: stop; 129-255: forward
 */
void setMotorSpeed(int motorSpeed);

/**
 * Adjusts the speed of the motors to turn the robot
 * left <- 0 - 128 (straight) - 255 -> right
 */
void setMotorTurn(int motorTurn);

void resetEncoders();
long getEncoder1();
long getEncoder2();
long getEncoderDifference();
byte getBatteryVoltage();
void printBatteryVoltage();

/* You do not need to know the rest */

// Include the Core Arduino Library
#if defined(ARDUINO) && (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Command codes for PID interface
#define SYNC (byte)0x00
#define SET_MODE (byte)0x34
#define SET_SPEED (byte)0x31
#define SET_TURN (byte)0x32

#define RESET_ENCODERS (byte)0x35
#define GET_ENCODER_1 (byte)0x23
#define GET_ENCODER_2 (byte)0x24
#define GET_ENCODERS (byte)0x25
#define GET_VOLTS (byte)0x26

void pidSetup() {
  PID.begin(38400); // Start serial communication

  // Set PID mode to 2 (turn mode)
  PID.write(SYNC);
  PID.write(SET_MODE);
  PID.write((byte)0x02);

  // Initially not moving
  setMotorSpeed(128);
  setMotorTurn(128);
  resetEncoders();
}

inline void setMotorSpeed(int motorSpeed)
{
  // Handle overflow
  if (motorSpeed > 255) motorSpeed = 255;
  else if (motorSpeed < 0) motorSpeed = 0;
  
  PID.write(SYNC);
  PID.write(SET_SPEED);
  PID.write((byte)motorSpeed);
}

inline void setMotorTurn(int motorTurn)
{
  // Handle overflow
  if (motorTurn > 255) motorTurn = 255;
  else if (motorTurn < 0) motorTurn = 0;
  
  PID.write(SYNC);
  PID.write(SET_TURN);
  PID.write((byte)motorTurn);
}

inline void resetEncoders() {
  PID.write(SYNC);
  PID.write(RESET_ENCODERS);
}

inline long readLong()
{
  unsigned long result;
  result = ((unsigned long)((unsigned byte)PID.read())) << 24;
  result |= ((unsigned long)((unsigned byte)PID.read())) << 16;
  result |= ((unsigned long)((unsigned byte)PID.read())) << 8;
  result |= ((unsigned long)((unsigned byte)PID.read()));
  return (long)result;
}

inline long getEncoder1() {
  PID.write(SYNC);
  PID.write(GET_ENCODER_1);
  return readLong();
}

inline long getEncoder2() {
  PID.write(SYNC);
  PID.write(GET_ENCODER_2);
  return readLong();
}

inline long getEncoderDifference() {  
  PID.write(SYNC);
  PID.write(GET_ENCODERS);
  return (readLong() - readLong());
}

byte getBatteryVoltage()
{
  byte result;
  
  PID.write(SYNC);
  PID.write(GET_VOLTS);
  delay(200);
  result = (byte)PID.read();
  //delay(500);
    
  if (result >= 10) result -= 10;
  return result;  
}

#endif // STD_PID_H

