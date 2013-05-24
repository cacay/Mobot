
/* Copyright (c) 2013 Cosku Acay, http://www.coskuacay.com */

#ifndef LINE_DETECTOR_H
#define LINE_DETECTOR_H

// Include the Core Arduino Library
#if defined(ARDUINO) && (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


/* Class Definition */

class LineDetector
{
  public:
    LineDetector(byte imageWidth);

    byte computeCentroid(byte* imageRow, byte threshold);
    
  private:
    byte _imageWidth;
    static const byte _maskHigh[8];
    static const byte _maskLow[8];
    static const byte _popCntTable[256];
    static const byte _centroidTable[256];
};


#endif // LINE_DETECTOR_H

