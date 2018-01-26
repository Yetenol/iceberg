#include "Config.h"
#include "Pin.h"
#include "Pilot.h"
#include "Utility.h"

#include <SPI.h>
#include <Pixy.h>
#include <Wire.h>
#include <HMC6352.h>
#include <PID_v1.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

// Einstellungen: FAHREN
#define PWR 60          // maximale Motorstärke
#define ROT_MULTI 0.35
#define ROT_MAX 0.5     // der maximale Wert der Rotation
int driveRot = 0;       // korrigiere Kompass
int driveDir = 0;       // Zielrichtung
Pilot m;                // OBJEKTINITIALISIERUNG

// Einstellungen: KOMPASS
int heading = 0;                // Wert des Kompass
int startHeading;               // Startwert des Kompass
int rotation;                   // rotationswert für die Motoren
unsigned long turningTimer = 0;
HMC6352 c;                      // OBJEKTINITIALISIERUNG

// Einstellungen: BLUETOOTH
String bluetoothBuffer = "";
String command = "";
bool  activeListening = false;
unsigned long bluetoothTimer = 0;
unsigned long heartbeatTimer = 0;

// Einstellungen: WICHTUNG DER PID-REGLER
double pidFilterP = 0.32; // p:proportional
double pidFilterI = 0.03; // i:vorausschauend
double pidFilterD = 0.03; // d:Schwung herausnehmen (nicht zu weit drehen)
double pidSetpoint;       // Nulllevel [-180 bis 180]:Winkel des Tours
double pidIn;             // Kompasswert [-180 bis 180]
double pidOut;            // Rotationsstärke [-255 bis 255]
PID myPID(&pidIn, &pidOut, &pidSetpoint, pidFilterP, pidFilterI, pidFilterD, DIRECT); // OBJEKTINITIALISIERUNG

// Einstellungen: PIXY
#define SPEED 100             // Geschwindigkeit des Roboters in %
#define PIXY_BALL_NUMMER 1    // Pixy-Signature des Balls
#define X_CENTER ((PIXY_MAX_X-PIXY_MIN_X)/2)  //Die Mitte des Bildes der Pixy (in Pixeln)
uint16_t blocks;              // hier werden die erkannten Bloecke gespeichert
unsigned long lastPixy = 0;   // Timer zum Auslesen der Pixy
byte blockAnzahl = 0;         // Anzahl der erkannten Bloecke
int ball;                     // Abweichung der Ball X-Koordinate
int highX;                    // Position des Balls (x-Koordinate)
int highY;                    // Position des Balls (y-Koordinate)
int xAbw = 0;                 // die Abweichung des Balls von der Mitte des Pixybildes
boolean ballSicht;            // ob wir den Ball sehen
Pixy pixy;                    // OBJEKTINITIALISIERUNG

// Einstellungen: DISPLAY
Adafruit_SSD1306 d(PIN_4);    // OBJEKTINITIALISIERUNG

// Einstellungen: STATUS-LEDS & LED-MATRIX
#define PWR_LED 20            // Helligkeit der Status-Leds
boolean stateFine = true;     // liegt kein Fehler vor?
boolean canSeeBall = false;   // sieht die Kamera den Ball?
boolean isOnTheBall = false;  // besitzen der Roboter den Ball?
Adafruit_NeoPixel matrix = Adafruit_NeoPixel(12, MATRIX_LED, NEO_GRB + NEO_KHZ800); // OBJEKTINITIALISIERUNG (LED-MATRIX)
Adafruit_NeoPixel stateLed = Adafruit_NeoPixel(3, STATE_LED, NEO_GRB + NEO_KHZ800); // OBJEKTINITIALISIERUNG (STATUS-LEDS)

//###################################################################################################

void setup() {
  DEBUG_SERIAL.begin(9600);   // Start der Seriellen Kommunikation
  BLUETOOTH_SERIAL.begin(38400);
  Wire.begin();         // Start der I2C-Kommunikation

  setupDisplay();       // initialisiere Display mit Iceberg Schriftzug
  pinModes();           // setzt die PinModes
  setupMotor();         // setzt Pins und Winkel des Pilot Objekts
  pixy.init();          // initialisiere Kamera

  delay(1000);

  //Torrichtung [-180 bis 179] merken
  startHeading = c.getHeading() - 180; //merkt sich Startwert des Kompass

  m.setMotEn(true);     // aktiviere die Motoren

  // Kalibriere Kompass: Drehe und messe kontinuierlich Kompasswerte
  if (!digitalRead(SWITCH_A)) {
    m.drive(0, 0, 8);   // Roboter drehr sich um eigene Achse
    c.calibration();
  }

  // merke Torrichtung
  m.brake(true);        // Roboter bremst aktiv
  c.setOutputMode(0);   // Kompass initialisieren
  pidSetpoint = 0;
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-255, 255);

  matrix.begin();   // MATRIX-LEDS initialisieren
  stateLed.begin(); // STATUS-LEDS initialisieren

  debugln("setup done");
}

//###################################################################################################

void loop() {
  ball = highX - X_CENTER;
  m.setMotEn(!digitalRead(SWITCH_MOTOR));

  showState(0, stateFine);
  showState(1, battLow());
  showState(2, millis() % 1000 < 200);

  showMatrix(1, m.getMotEn());
  showMatrix(2, canSeeBall);
  showMatrix(3, isOnTheBall);
  showMatrix(4, millis() - heartbeatTimer < 500);

  // lese die Pixy maximal alle 25ms aus
  if (millis() - lastPixy > 25) {
    readPixy();
  }

  // bluetooth senden
  if (millis() - bluetoothTimer > 100) {
    bluetoothTimer = millis();
    bluetooth("h"); // heartbeat
  }

  // bluetooth auslesen
  command = receiveBluetooth();
  if (command != "") {
    //debugln("[" + (String)millis() + "] " + (String)command);
    switch (command.charAt(0)) {
      case 'h': // heartbeat
        heartbeatTimer = millis();
        break;
      default:
        break;
    }
  }

  d.clearDisplay();
  d.setTextColor(WHITE);
  d.setTextSize(1);
  d.setCursor(0, 0);
  d.println("Iceberg Robots [" + String(numberOfMinutes(millis())) + ":" + String(numberOfSeconds(millis())) + "]");
  d.setTextSize(2);
  d.println("Kom: " + String(heading));
  //d.drawCircle(20, d.height()/2, 20, WHITE);
  if (ballSicht) {
    d.println("Ball:");
    d.drawLine(0.75 * d.width(), 0.58 * d.height(), constrain(map(ball, -160, 160, 0.5 * d.width(), d.width() - 1), 0.5 * d.width(), d.width() - 1), 0.47 * d.height(), WHITE);
  } else {
    d.println("Ball:blind");
  }

  d.display();      // aktualisiere Display
  matrix.show();    // aktualisiere Matrix-Leds
  stateLed.show();  // aktualisiere Status-Leds

  driveRot = ausrichten();

  if (ballSicht) {
    if (-20 < ball && ball < 20) {
      // fahre geradeaus
      driveDir = 90;
    } else {
      // drehe dich zum Ball
      driveDir = map(ball, -160, 160, -50, 230);
    }
  } else {
    // fahre nach hinten
    driveDir = -90;
  }

  //durch addieren von 360° wird dafuer gesorgt, dass die Richtung groesser als 0 ist
  while (motorDir < 0) {
    motorDir += 360;
  }

  delay(1);

}

//###################################################################################################

// Piloten konfigurieren
void setupMotor() {
  m.setAngle(70);

  m.setPins(0, FWD0, BWD0, PWM0);
  m.setPins(1, FWD1, BWD1, PWM1);
  m.setPins(2, FWD2, BWD2, PWM2);
  m.setPins(3, FWD3, BWD3, PWM3);
}

// Bildschirm konfigurieren und Startschriftzug zeigen
void setupDisplay() {
  d.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //Initialisieren des Displays
  d.clearDisplay();     //leert das Display
  d.setTextSize(2);     //setzt Textgroesse
  d.setTextColor(WHITE);//setzt Textfarbe
  d.setCursor(0, 0);    //positioniert Cursor
  d.println("ICEBERG");  //schreibt Text auf das Display
  d.println("ROBOTS");
  d.setTextSize(1);     //setzt Textgroesse
  d.println();
  d.setTextSize(2);     //setzt Textgroesse
  d.println("      2018");
  d.display();          //wendet Aenderungen an
}

// Roboter mittels PID-Regler zum Tor ausrichten
int ausrichten() {
  heading = ((int)((c.getHeading()/*[0 bis 359]*/ - startHeading/*[-359 bis 359]*/) + 360) % 360)/*[0 bis 359]*/ - 180/*[-180 bis 179]*/; //Misst die Kompassabweichung vom Tor

  pidIn = (double) heading;

  double gap = abs(pidSetpoint - pidIn); //distance away from setpoint
  myPID.SetTunings(pidFilterP, pidFilterI, pidFilterD);
  myPID.Compute();

  return -pidOut;
}

// Pixy auslesen: sucht groesten Block in der Farbe des Balls
void readPixy() {
  int greatestBlock = 0; //hier wird die Groeße des groeßten Blocks gespeichert
  highX = 0;             //Position des Balls (X)
  highY = 0;             //Position des Balls (Y)
  blockAnzahl = 0;       //Anzahl der Bloecke

  blocks = pixy.getBlocks();  //lässt sich die Bloecke ausgeben

  for (int j = 0; j < blocks; j++) {                                  //geht alle erkannten Bloecke durch
    if (pixy.blocks[j].signature == PIXY_BALL_NUMMER) {               //Überprueft, ob es sich bei dem Block um den Ball handelt
      if (pixy.blocks[j].height * pixy.blocks[j].width > greatestBlock) { //Wenn der Block der aktuell groesste ist
        greatestBlock = pixy.blocks[j].height * pixy.blocks[j].width;
        highX = pixy.blocks[j].x;                                     //Position des Blocks wird uebernommen
        highY = pixy.blocks[j].y;
      }
      blockAnzahl++;        //Anzahl wird hochgezaehlt
    }
  }

  lastPixy = millis();         //Timer wird gesetzt, da Pixy nur alle 25ms ausgelesen werden darf

  ballSicht = blockAnzahl != 0; //wenn Bloecke in der Farbe des Balls erkannt wurden, dann sehen wir den Ball
}

// Bluetooth auswerten
String receiveBluetooth() {
  if (BLUETOOTH_SERIAL.available() > 0) {
    char c = BLUETOOTH_SERIAL.read();
    if ( activeListening) {
      if (c == START_MARKER) {
        bluetoothBuffer = "";
      } else if (c == END_MARKER) {
        activeListening = false;
        return (bluetoothBuffer);
      } else {
        bluetoothBuffer += c;
      }
    } else {
      if (c == START_MARKER) {
        bluetoothBuffer = "";
        activeListening = true;
      }
    }

  }
  return ("");
}

// Matrix-Led zeigt Boolean-Wert rot oder gruen an
void showMatrix(byte pos, boolean state) {
  matrix.setPixelColor(pos, matrix.Color((!state)*PWR_LED, state * PWR_LED, 0));
}

// Status-Led zeigt Boolean-Wert rot oder gruen an
void showState(byte pos, boolean state) {
  stateLed.setPixelColor(pos, stateLed.Color((!state)*PWR_LED, state * PWR_LED, 0));
}
