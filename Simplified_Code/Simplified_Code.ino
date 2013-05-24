/*-
 * Copyright (c) 2013 Cosku Acay, http://www.coskuacay.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

/* Include these headers in all project files */
#include <CMUcam4.h>
#include <CMUcom4.h>
#include "stdMobot.h"

CMUcam4 cam;

void setup(){  
  pidSetup(); // Setup the PID controller
  cam = camSetup(); // Setup the CMUcam
}

// Movement Constants
#define TURN_SPEED 32
#define TURN_BIAS -1
#define NORMAL_SPEED 96

void loop() {
  CMUcam4_image_data_t f_data;
  CMUcam4_tracking_data_t t_data;
  
  cam.colorTracking(YUV_MODE);
  cam.lineMode(true);
  cam.trackColor(RED_MIN, RED_MAX, GREEN_MIN, GREEN_MAX, BLUE_MIN, BLUE_MAX);
 
  for(;;)
  {
    cam.getTypeTDataPacket(&t_data); // Get a tracking packet.
    cam.getTypeFDataPacket(&f_data); // Get an image packet.

    int area = computeCentroid((byte*)(&(f_data.pixels)));
    
    if (area == 255) area = 0; // Go straight if you see no line
    
    int motorTurn = 128 + TURN_BIAS + ((area * TURN_SPEED) / (IMAGE_WIDTH / 2));
    setMotorTurn(motorTurn);
    setMotorSpeed(128 + NORMAL_SPEED);
  }
}

#define AREA_SAMPLES 10
#define AREA_SAMPLE_THRESHOLD 4
#define PIXELS_PER_ROW_THRESHOLD 5

/** Computes the area between the projected path and the actual path
  * using the given amount of samples
  */
int computeCentroid(byte* image)
{
  const int rowStart = ((IMAGE_HEIGHT / 2) / AREA_SAMPLES) * (IMAGE_WIDTH / 8);
  const int rowStep = (IMAGE_HEIGHT / AREA_SAMPLES) * (IMAGE_WIDTH / 8);
  const int finalByte = rowStart + rowStep * AREA_SAMPLES;
     
  LineDetector detector(IMAGE_WIDTH / 8);
  
  int centroid = 0;
  int nSamples = AREA_SAMPLES;
  for (byte* curr = image + rowStart; curr < image + finalByte; curr += rowStep)
  {
    byte partial = detector.computeCentroid(curr, PIXELS_PER_ROW_THRESHOLD);
    if (partial == 255)
      nSamples--;
    else
      centroid += (int)(partial) - (IMAGE_WIDTH / 2);
  }
  
  // Return the average area
  if (nSamples == 0) return 255;
  if (nSamples < AREA_SAMPLE_THRESHOLD) return 255;
  return (centroid / nSamples); // May lose information
}

