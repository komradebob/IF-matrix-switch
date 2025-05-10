//
// setup.ino
//
// All the setup code for if_switch
//
// Nov 24, 2024
//


#include "if_switch.h"

void setup() {                // setup
  char i;

  memset(rows, 0, sizeof(rows));
  memset(cols, 0, sizeof(cols));

  strcpy(rows[0], "One");
  strcpy(rows[1], "Two");
  strcpy(rows[2], "Three");
  strcpy(rows[3], "Four");
  strcpy(rows[4], "Five");
  strcpy(rows[5], "Six");
  strcpy(cols[0], "Alpha");
  strcpy(cols[1], "Beta");
  strcpy(cols[2], "Delta");
  strcpy(cols[3], "Gamma");

  // Set the mode for all the outputs

  // 8 bits

  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  // 3 Selects and an output enable

  pinMode(SELECT1, OUTPUT); // Bank 1, Relays 0-7
  pinMode(SELECT2, OUTPUT); // Bank 1, Relays 8-15
  pinMode(SELECT3, OUTPUT); // Bank 2, relays 0-7
  pinMode(OE, OUTPUT);

  // Test the LEDs/relays

  //  cylon(1,1);
  //  cylon(2,1);
  //  cylon(3,1);

  // Clear all the outputs

  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);

  digitalWrite(OE, LOW);
  digitalWrite(SELECT1, LOW);
  digitalWrite(SELECT2, LOW);
  digitalWrite(SELECT3, LOW);

  /////////////////// Clear the latches, turn everything off /////////////////

  latch(1);
  latch(2);
  latch(3);

  eeprom_restore();

  header_color = RESTORED;

  // Open serial communication at a baud rate of BAUDRATE
  Serial.begin(UARTBAUD);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, myIP, myDNS, myGW, myMASK);


  server.begin();
  //#ifdef DEBUG
  Serial.print(F("Setup: server is at "));
  Serial.println(Ethernet.localIP());
  //#endif

#ifdef SYSLOG_ENABLE
    syslog.logf(LOG_INFO, F("Setup Complete"));
#endif

}
////////////////////////////// End Setup /////////////////////
