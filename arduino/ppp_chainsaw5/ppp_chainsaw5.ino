//**************************//
//  PPP VERSION 2.0		
//	NL Sept, 2015			
//	Slash / Spin / Stab 
//	Idle Mode
//**************************//

/// SAMPLES + THRESHHOLDS + LIBRARIES ///////////////////////////////////////////////////////////////

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define PIN 2
#define NUM_PIXELS 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

/// to smooth
#define SAMPLES 5

// threshold for each movement, can play around with them

#define SLASH_THRESHOLD 50
#define STAB_THRESHOLD 50
#define SPIN_THRESHOLD 50
#define BASE_THRESHOLD 10

/// SMOOTHING OUT THE ACCELEROMTER AXIS ///////////////////////////////////////////////////////////

int smoothYArray[SAMPLES];
int rawYAxis, smoothYAxis;

int smoothXArray[SAMPLES];
int rawXAxis, smoothXAxis;

int smoothZArray[SAMPLES];
int rawZAxis, smoothZAxis;

/// ANGLE SET UP ///////////////////////////////////////////////////////////////////////////////////

/// Overall min max of accel ///

int minVal = 3;
int maxVal = 272;

/// calculated values ///
double x;
double y;
double z;

////// MAGNITUDE OF DIFFERENCE //////////////////////////////////////////////////////////////////////

/// empty readings ///
int previousX;
int previousY;
int previousZ;


/// WEAPON FIGHTING POSITION SAVE STATE /////////////////////////////////////////////////////////////

boolean wasStab = false; 
boolean wasSpin = false;
boolean wasSlash = false;
boolean wasIdle = false;

/// WEAPON IDLE STATE ////////////////////////////////////////////////////////////////////////////// 

unsigned long interval = 5000;  // the time we need to wait (5 seconds).
unsigned long previousMillis = 0; // millis() returns an unsigned long.

void setup() {

	/// SETUP /// 
	Serial.println("setup");

	/// SERIAL ///
  	Serial.begin(57600);

  	/// INITIAL READINGS ///
	AccelerationReading rawAccel = Bean.getAcceleration();
  	previousX = rawAccel.xAxis;
  	previousY = rawAccel.yAxis;
  	previousZ = rawAccel.zAxis;

  	/// neopixels stuff
  	strip.begin();
  	strip.show(); // Initialize all pixels to 'off'

  	/// blink once that you're done for 2 seconds  (purple)
  	Bean.setLed(200, 0, 200);
  	Bean.sleep(1000);
  	Bean.setLed(0, 0, 0); 
}


void loop() {

	
	/// GET ACCEL READINGS ///

	AccelerationReading accel = Bean.getAcceleration();


	/// ACCEL READING SMOOTHING FOR ANGLES ///////////////////////////////////////////////////////////
  	
  	// read Y Axis, send to smooth function
  	rawYAxis = accel.yAxis;
  	smoothYAxis = digitalSmooth(rawYAxis, smoothYArray);

  	// read X Axis and send to smooth function
  	rawXAxis = accel.xAxis;
  	smoothXAxis = digitalSmooth(rawXAxis, smoothXArray);

  	// read Z Axis and send to smooth function
  	rawZAxis = accel.zAxis;
  	smoothZAxis = digitalSmooth(rawZAxis, smoothZArray);
  	
  	int accelDifference = getAccelDifference(previousX, previousY, previousZ, smoothXAxis, smoothYAxis, smoothZAxis); 
  	previousX = smoothXAxis;
  	previousY = smoothYAxis;
  	previousZ = smoothZAxis;

  	/// ANGLES (BLDR.ORG) /////////////////////////////////////////////////////////////////////////////

  	int xRead = smoothXAxis;
  	int yRead = smoothYAxis;
  	int zRead = smoothZAxis;

  	//convert read values to degrees -90 to 90 - Needed for atan2
  	int xAng = map(xRead, minVal, maxVal, -90, 90);
  	int yAng = map(yRead, minVal, maxVal, -90, 90);
  	int zAng = map(zRead, minVal, maxVal, -90, 90);

	//Caculate 360deg values like so: atan2(-yAng, -zAng)
	//atan2 outputs the value of -π to π (radians)
	//We are then converting the radians to degrees

	x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
	y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
	z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

	/// OUTPUT ANGLES (traces for testing and finding positions) ///
	
	  /*Serial.print(x);
  	Serial.print("\t");
  	Serial.print(y);
  	Serial.print("\t");
  	Serial.print(z);
  	Serial.print("\n");*/
	/*
	/// trace to see smoothed values from accel
  	String stringToPrint = String();
  	stringToPrint = stringToPrint + "X: " + smoothXAxis + "\tY: " + smoothYAxis + "\tZ: " + smoothZAxis;
  	Serial.println(stringToPrint);
  	Serial.print("\n");*/


  	/// if the bean is moving above the base threshold, start tripping the weapon logic. 
  	
  	if (accelDifference >= BASE_THRESHOLD) {

  		// current state as of this loop

  	boolean isStab; 
		boolean isSpin;
		boolean isSlash;
		boolean isIdle;

		/// WEAPON LOGIC /////////////////////////////////////////////////////////////////////////////////////////////////

      

      //( ((x >= 230 && x <= 260) && (y >= 240 && y <= 270) && (z >= 230 && z <= 250)) && (accelDifference > STAB_THRESHOLD))
      
	  	if ( ((x >= 230 && x <= 260) && (y >= 240 && y <= 270) && (z >= 210 && z <= 250)) && (accelDifference > STAB_THRESHOLD)) {  

	  		// STAB (downward motion)
        // pretty good so far.

	  		isStab = true;
	  		isSpin = false;
	  		isSlash = false;
	  		//Serial.println("p1stab");
        String stringToPrint = String();
        //stringToPrint = stringToPrint + "p2stab";
        stringToPrint = stringToPrint + "p2stab";
        Serial.print(stringToPrint);
      
      //((x >= 155 && x <= 170) && (y >= 170 && y <= 185) && (z >= 80 && z<= 110)) && (accelDifference > SPIN_THRESHOLD) 

	  	}else if ( ((x >= 155 && x <= 245) && (y >= 150 && y <= 205) && (z >= 3 && z<= 300)) && (accelDifference > SPIN_THRESHOLD) ) {

	  		// Upward motion / held over the head

	  		isSpin = true;
	  		isStab = false;
	  		isSlash = false;
	  		//Serial.println("p1spin");
        String stringToPrint = String();
        //stringToPrint = stringToPrint + "p2spin";
        stringToPrint = stringToPrint + "p2spin";
        Serial.print(stringToPrint);

      //((x >= 270 && x <= 315) && (y >= 280 && x <= 350) && (z >= 240 && z <= 285)) && (accelDifference > SLASH_THRESHOLD)

	  	}else if ( ((x >= 290 && x <= 345) && (y >= 284 && x <= 325) && (z >= 186 && z <= 247)) && (accelDifference > SLASH_THRESHOLD) ) {

	  		/// SLASH - side to side motion

	  		isSlash = true;
	  		isSpin = false;
	  		isStab = false;
	  		//Serial.println("p1slash");
        String stringToPrint = String();
        //stringToPrint = stringToPrint + "p2slash";
        stringToPrint = stringToPrint + "p2slash";
        Serial.print(stringToPrint);
      
	  	} else {
	  		isStab = false;
  			isSpin = false;
	  		isSlash = false;
	  		isIdle = true;
	  	}
	  
	  	/// STATE DETECTION AND SAVING /////////////////////////////////////////////////////////////

	  	if (wasStab!= isStab) {
	    	// state has changed, so do something
	    	if (isStab) {
	    		Bean.setLed(200,0,0);
	    		colorWipe(strip.Color(200, 0, 0), 20);
	    	} 
	    	wasStab = isStab; 
	  	} 

	  	if (wasSpin!= isSpin) {
	    	// state has changed, so do something
	    	if (isSpin) {
	      		//set led to blue
	      		Bean.setLed(0,0,200);
	      		theaterChase(strip.Color(0, 0, 200), 20);
	      		//colorWipe(strip.Color(0, 0, 200), 20);
	    	} 
	    	wasSpin = isSpin; 
	  	}

	  	if (wasSlash!= isSlash) {
	    	// state has changed, so do something
	    	if (isSlash) {
	      		//set led to green
	      		Bean.setLed(0,200,0);
	      		colorWipe(strip.Color(0, 200, 0), 20);
	    	} 
	    	wasSlash = isSlash; 
	  	}

	  	if (wasIdle!= isIdle) {
	    	// state has changed, so do something
	    	if (isIdle) {
	      		//set led to off
	      		Bean.setLed(0,0,0);
	      		resetPixels();
	    	} 
	    	wasIdle = isIdle; 
	  	}

	} else {

		/// Just keep it Idle if it isn't moving ///////////////////////////
  		
  		if ((unsigned long)(millis() - previousMillis) >= interval) {
  			Bean.setLed(0,0,0);
  			resetPixels();
  			previousMillis = millis(); 
  		}
	}

	/// needed so the bean doesn't lock up. Can't use sleep because millis.
  Serial.flush();
	delay(20);
	  	
} /// end of loop


/// NEOPIXELS ////////////////////////////////////////////////////////////

/// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

/// Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 5 cycles of chasing
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

/// rainbow
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

/// turn everything off
void resetPixels() {

  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i,0);
    strip.show();
  }

}

/// ACELL DIFF (LIGHTBLUEBEAN.COM) //////////////////////////////////////////////////////////////

int getAccelDifference(int oldX, int oldY, int oldZ, int newX, int newY, int newZ){
  int deltaX = abs(newX-oldX);
  int deltaY = abs(newY-oldY);
  int deltaZ = abs(newZ-oldZ);
  // Return the magnitude
  return deltaX + deltaY + deltaZ;   
}


/// DIGITAL SMOOTH (ARDUINO.CC) //////////////////////////////////////////////////////////////

int digitalSmooth(int rawIn, int *sensSmoothArray){
	int j, k, temp, top, bottom; 
	long total;
	static int i;

	static int sorted[SAMPLES];
  	boolean done;
  	i = (i + 1) % SAMPLES;    // increment counter and roll over if necc. -  % (modulo operator) rolls over variable
  	sensSmoothArray[i] = rawIn;     // input new data into the oldest slot 

  	// Serial.print("raw = ");

  	// transfer data array into another array for sorting and averaging
  	for (j=0; j<SAMPLES; j++){     
    	sorted[j] = sensSmoothArray[j];
  	}
  	done = 0; // flag to know when we're done sorting 
  	while(done != 1){ // simple swap sort, sorts numbers from lowest to highest
  		done = 1;
  		for (j = 0; j < (SAMPLES - 1); j++){
  			if (sorted[j] > sorted[j + 1]){
  				temp = sorted[j + 1];
  				sorted [j+1] =  sorted[j];
  				sorted [j] = temp;
  				done = 0;
  			}
  		}
  	} 
  	// throw out top and bottom 15% of samples - limit to throw out at least one from top and bottom
  	bottom = max(((SAMPLES * 15)  / 100), 1);
  	// the + 1 is to make up for asymmetry caused by integer rounding
  	 top = min((((SAMPLES * 85) / 100) + 1  ), (SAMPLES - 1));   
  	k = 0;
  	total = 0;

  	for ( j = bottom; j< top; j++){
    	total += sorted[j];  // total remaining indices
    	k++; 
    	// Serial.print(sorted[j]); 
    	// Serial.print("   "); 
    }

    return total / k;    // divide by number of samples
	// end of digitalSmooth
}
