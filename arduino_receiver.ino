/* Firing up dependencies */
#include <LiquidCrystal.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <VirtualWire.h>

// Setting up the pins for use with the lcd
LiquidCrystal lcd(7, 6, 5, 4, 3, 0);

// Setting up the pins for the Antenna
RF24 radio(9,10);

// Defining the addresses for the two antennas (transmitter & receiver)
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

// The setup loop
void setup()
{
	// Setting up the lcd parameters (columns, rows)
	lcd.begin(16,2);
	
	// Choosing the 57600 BaudRate
	Serial.begin(57600);
	
	// Starting a radio connection
	radio.begin();
	radio.setRetries(15,15);
	
	// Openning a listening pipe on the specified address
	radio.openReadingPipe(1,pipes[1]);
	radio.startListening();
	
	// verbal mode
	radio.printDetails();
}

// main loop
void loop(void)
{
	// A connection has been established
	if ( radio.available() ) {
		
		// this will be our data container
		char data[10];
		
		// reading from radio, into data
		radio.read( &data, sizeof(data) );

		/*
		** Expecting data in the format temperature:heartrate
		** e.g.		26.02:62.09
		*/
			
		// the counter
		int i = 0;
		
		// the regex index
		int regex = 0;
		
		String Temp;
		String rate;

		/* Looping through the string, and splitting at the ":" regex */
		for(i=0; i<10; i++){

		  if(regex == 0){
		  
			// we didn't find the regex yet
			if(data[i] == ':'){
				// store the index
				regex = i;
			} else {
				// concatinate the new byte to the temperature variable
			  Temp.concat(data[i]);
			}
		  } else {
			if(!data[i]){
				// an empty byte
				break;
			}
			// concatinate the new byte to the heartrate variable
			rate.concat(data[i]);
			}
		}
	
	/* Flushing data to the serial port */
	Serial.print("Air Tempreature = ");
	Serial.print(Temp);
	Serial.print("'C");
	Serial.println();
	Serial.print("Beat Rate = ");
	Serial.print(rate);
	Serial.println();
	lcd.print(data);
	
	// sleep before the next input
	delay(1);
	}
}













