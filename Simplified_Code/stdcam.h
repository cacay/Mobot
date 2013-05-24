
/* Copyright (c) 2013 Cosku Acay, http://www.coskuacay.com */

#ifndef STD_CAM_H
#define STD_CAM_H

#include "stdports.h"
#include <CMUcam4.h>
#include <CMUcom4.h>

/* Function Definitions and Constants */

#define IMAGE_WIDTH 80
#define IMAGE_HEIGHT 60

CMUcam4 camSetup();

/* Camera Tracking Settings */

#define YUV_MODE 1

#if (!YUV_MODE)
  #define RED_MIN 220
  #define RED_MAX 255
  #define GREEN_MIN 220
  #define GREEN_MAX 255
  #define BLUE_MIN 220
  #define BLUE_MAX 255
#else
  #define RED_MIN 0     // V_MIN
  #define RED_MAX 255   // V_MAX
  #define GREEN_MIN 180 // Y_MIN
  #define GREEN_MAX 255 // Y_MAX
  #define BLUE_MIN 0    // U_MIN
  #define BLUE_MAX 255  // U_MAX
#endif

#define CAM_NOISE_FILTER 0
#define CAM_ADJUST_DELAY 4000 // 5 seconds


/* Implementation */

// Include the Core Arduino Library
#if defined(ARDUINO) && (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
  
CMUcam4 camSetup()
{
  CMUcam4 cam(CAM);
  cam.begin();

  // Wait for auto gain and auto white balance to run
  cam.LEDOn(5); // 5 Hzc
 
  // Turn auto gain and auto white balance off.
  cam.autoGainControl(false);
  cam.autoWhiteBalance(false);
  
  cam.noiseFilter(CAM_NOISE_FILTER);
  cam.LEDOn(CMUCAM4_LED_ON);

  return cam;
}

#endif // STD_CAM_H

