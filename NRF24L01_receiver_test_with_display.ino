#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_GFX.h>   // Core graphics library
#include <P3RGB64x32MatrixPanel.h>

#define CE_PIN  27
#define CSN_PIN 26  
#define CLK_SPI 14 // 18
#define MISO 12 // 19
#define MOSI 13 //23
#define SS 15 // HSPI 15 // Yes same pin as CSN, but that's what it is for

#define R1 23
#define G1 22
#define BL1 1
#define R2 3
#define G2 21
#define BL2 19
#define pin_A 18
#define pin_B 5
#define pin_C 17
#define pin_D 16
#define CLK_RGB 25
#define LAT_RGB 32
#define OE_RGB 33

bool doOnceRadio = true;
bool rpdPrint;
RF24 radio(CE_PIN,CSN_PIN);

unsigned long timeDifference;

const uint64_t pipe1 = 0xE8E8EEF0A1LL;;
const uint64_t pipe2 = 0xA2LL;  
const uint64_t pipe3 = 0xA3LL;
const uint64_t pipe4 = 0xA4LL;
const uint64_t pipe5 = 0xA5LL;
const uint64_t pipe6 = 0xA6LL;


typedef struct {
  char filterChar[1];
  bool testBitLED;
  bool startBit;
  bool reset1;
  uint8_t setDiscipline;
  unsigned long setDuration;
  unsigned long masterMillis;
  bool returnResult;
  uint16_t result;
  bool switchChannel;
  bool reset2;
  } TXstruct;

  TXstruct message;

  // Matrix pin constructor
  
P3RGB64x32MatrixPanel matrix(R1,G1,BL1,R2,G2,BL2,CLK_RGB,LAT_RGB,OE_RGB,pin_A,pin_B,pin_C,pin_D);
  
  
void setup() {  
  Serial.begin(250000);
   SPI.begin(CLK_SPI,MISO,MOSI,SS);
  
  // Matrix initialization
  matrix.begin();
  matrix.setTextSize(1);     // size 1 == 8 pixels high
  //matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves
     
     // Start up Screen text:
  matrix.setTextColor(matrix.color555(1,15,31));
  matrix.setCursor(11, 2); 
  matrix.println("Display");
  matrix.setTextColor(matrix.color555(31,15,1));
  matrix.setCursor(20, 12); 
  matrix.println("Test");
  matrix.setTextColor(matrix.color555(31,1,1));
  matrix.setCursor(20, 22); 
  matrix.println("Radio");
 
  
  radio.begin();
  radio.openReadingPipe(1,pipe1); // (1, const uint8_t *address) Pipe number (usually 1), pipe address (which is usually 5 bytes in an array structure).
  radio.setPALevel(RF24_PA_MIN); // RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH and RF24_PA_MAX
  radio.setDataRate(RF24_2MBPS); // 500kBPS 1MBPS   2MBPS
  radio.enableAckPayload();
  radio.setRetries(15,15);
  radio.setChannel(108); // 0-124

  radio.startListening();


 
}
void loop() {

  if(doOnceRadio){
    radio.printDetails();
    doOnceRadio = false;
    }
    
  if (radio.available()) {
    
    radio.read(&message, sizeof(message));
    
  timeDifference = message.masterMillis-millis();
  
  
    //if(message.filterChar[0]=='H'){ // Check character to ensure no unvalid data enters
    Serial.print("Filter char ");
    Serial.print(message.filterChar[0]);
    Serial.print(" Set LED");
    Serial.print(message.testBitLED);
    Serial.print(" START? ");
    Serial.print(message.startBit);
    Serial.print(" reset? ");
    Serial.print(message.reset1);
    Serial.print(" ");
    Serial.print(message.reset1);
    Serial.print(" Disipline ");
    Serial.print(message.setDiscipline);
    Serial.print(" Duration ");
    Serial.print(message.setDuration);
    Serial.print(" Master Millis ");
    Serial.print(message.masterMillis);
    Serial.print(" Return Result ");
    Serial.print(message.returnResult);
    Serial.print(" Result ");
    Serial.print(message.result);
    Serial.print(" Switch CH? ");
    Serial.println(message.switchChannel);
     //} end of if(message.filterChar[0]=='H')
   delay(1000);
  } // End of if (radio.available())
} // end of void loop()
