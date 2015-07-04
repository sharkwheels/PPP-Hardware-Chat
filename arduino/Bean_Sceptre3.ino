#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define SAMPLES 5
#define accelerationThreshold 150  


#define PIN 2 // NEOPIXELS PIN

 // button pin
 const int BUTTON = 5; // BUTTON PIN
 int buttonState;
 int lastButtonState = LOW;

 long lastDebounceTime = 0;
 long debounceDelay = 50;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);

class SmoothAccelerometer
{
  public:
    SmoothAccelerometer();

    void init();
    void process();

    int16_t xAxis;
    int16_t yAxis;
    int16_t zAxis;

    int16_t xRaw;
    int16_t yRaw;
    int16_t zRaw;

  private:
    int16_t _nX[SAMPLES];
    int16_t _nY[SAMPLES];
    int16_t _nZ[SAMPLES];
    int _nReadingIndex;

    int16_t xAvg();
    int16_t yAvg();
    int16_t zAvg();
};

SmoothAccelerometer accel;

void setup()
{
  // This is to extend start-up idle time in case needed for uploading sketches
  Bean.sleep(10);
  // Initialize Serial interface
  Serial.begin(57600);
  Serial.setTimeout(25);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  pinMode(BUTTON, INPUT); 

  accel.init();

  // still need to put in the button. :-/ grumble.

}

void loop() 
{

  char buffer[64];
  size_t length = 64;
  length = Serial.readBytes(buffer, length); 

  accel.process();

  // get your dirty inccorectly mapped y angles 
  float yAng = makeYAngles();

  // get the overall difference in magnitude
  int accelDifference = abs( accel.xRaw-accel.xAxis ) + abs( accel.yRaw-accel.yAxis) + abs( accel.zRaw-accel.zAxis);
  
  int reading = digitalRead(BUTTON);  

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    } 

  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        //
      }
    }
  }

    if (accelDifference > accelerationThreshold) 
    {

      

      if (accelDifference > 270 && (yAng > -0.5 && yAng < 0.5)) 
      {
        //Serial.println(accelDifference);
        //Serial.println(yAng);
        String stringToPrint = String();
        stringToPrint = stringToPrint + "p1slash";
        
        Serial.print(stringToPrint);
        
        rainbow(2);
        
        yAng = 0;
        
        

      } else if (accelDifference > 220 && yAng <= -0.8) 
      {
        //Serial.println(accelDifference);
        //Serial.println(yAng);
        String stringToPrint = String();
        stringToPrint = stringToPrint + "p1spin";
        Serial.print(stringToPrint);
        //Bean.setLed(0,0,0);
        theaterChase(strip.Color(127, 127, 127), 50); // White

        yAng = 0;
        
        
        // shake and light up
      } else if (accelDifference > 270 && yAng >= 0.8) {
          String stringToPrint = String();
          stringToPrint = stringToPrint + "p1stab";
          Serial.print(stringToPrint);
          //Bean.setLed(0,0,0);
          theaterChase(strip.Color(255, 0, 0), 50); // White
          yAng = 0;     
      }

    } else {
    // set everything to a rest mode
      //Bean.setLed(0,0,0);
      resetPixels();
  }
  


  delay(200); // could also be bean.sleep, have to wait for neopixels code. 
  
} // end of loop


void resetPixels() {

  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i,0);
    strip.show();
  }

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}




// Just making dirty Y angles
float makeYAngles()
{
  float x1 = accel.xAxis;
  float y1 = accel.yAxis;
  float z1 = accel.zAxis;


  float x2 = abs(x1 * x1);
  float y2 = abs(y1 * y2);
  float z2 = abs (z1 * z1);

  float result;
  float accel_angle_y;
  float yAtan2;

  // Y-Axis
  result = sqrt(x2+z2);
  result = y1/result;
  accel_angle_y = atan(result);

  
  
  return accel_angle_y;

}

// make some dirty x angles

float makeXAngles() {

  float x1 = accel.xAxis;
  float y1 = accel.yAxis;
  float z1 = accel.zAxis;


  float x2 = abs(x1 * x1);
  float y2 = abs(y1 * y2);
  float z2 = abs (z1 * z1);

  float resultX;
  float accel_angle_x;
  

  // Y-Axis
  resultX = sqrt(y2+z2);
  resultX = x1/resultX;
  accel_angle_x = atan(resultX);
  
  return accel_angle_x;

}

// Acell smoothing by bdwalker1

SmoothAccelerometer::SmoothAccelerometer()
{

}

void SmoothAccelerometer::init()
{
  // Initialize accelerometer reading arrays
  AccelerationReading accXYZ = Bean.getAcceleration();
  xRaw = accXYZ.xAxis;
  yRaw = accXYZ.yAxis;
  zRaw = accXYZ.zAxis;
  _nReadingIndex = 0;
  for(int i=0; i<SAMPLES; i++ ) { _nX[i] = xRaw; }
  for(int i=0; i<SAMPLES; i++ ) { _nY[i] = yRaw; }
  for(int i=0; i<SAMPLES; i++ ) { _nZ[i] = zRaw; }

  // Initialize axis values
  xAxis = xAvg();
  yAxis = yAvg();
  zAxis = zAvg();
}

void SmoothAccelerometer::process()
{
  // Get current accelerometer readings
  AccelerationReading accXYZ = Bean.getAcceleration();
  xRaw = accXYZ.xAxis;
  yRaw = accXYZ.yAxis;
  zRaw = accXYZ.zAxis;

  // Update reading arrays
  _nX[_nReadingIndex] = xRaw;
  _nY[_nReadingIndex] = yRaw;
  _nZ[_nReadingIndex] = zRaw;
  _nReadingIndex++;
  if( _nReadingIndex == SAMPLES ) _nReadingIndex = 0;

  // Update axis values
  xAxis = xAvg();
  yAxis = yAvg();
  zAxis = zAvg();
}

int16_t SmoothAccelerometer::xAvg()
{
  int32_t nX = 0;
  for( int i=0; i<SAMPLES; i++)
  {
    nX += _nX[i];
  }
  return (int16_t) (nX / SAMPLES);
}

int16_t SmoothAccelerometer::yAvg()
{
  int32_t nY = 0;
  for( int i=0; i<SAMPLES; i++)
  {
    nY += _nY[i];
  }
  return (int16_t) (nY / SAMPLES);
}

int16_t SmoothAccelerometer::zAvg()
{
  int32_t nZ = 0;
  for( int i=0; i<SAMPLES; i++)
  {
    nZ += _nZ[i];
  }
  return (int16_t) (nZ / SAMPLES);
}
