#include <SPI.h>
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

//+++++++++++++++++++++++Création d'objet pour les Feux+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define Rouge 2
Adafruit_NeoPixel red = Adafruit_NeoPixel(24, Rouge, NEO_GRB + NEO_KHZ800);

#define Orange 3
Adafruit_NeoPixel oran = Adafruit_NeoPixel(24, Orange, NEO_GRB + NEO_KHZ800);

#define Vert 4
Adafruit_NeoPixel green = Adafruit_NeoPixel(24, Vert, NEO_GRB + NEO_KHZ800);


//++++++++++++++++++++++++++++++WiFi information+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

char ssid[] = "F6";      //  Mon network SSID (name)
char pass[] = "azerty84000";   // Mon network password
int keyIndex = 0;                 // Mon network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(5500);

WiFiClient IHM;


//+++++++++++++++++++++++++++++Variable du feu++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

String Commande;

int TempR;
int TempV;
int TempO = 5;

char Mod;

int FeuRouge;
int FeuOrange;
int FeuVert;

int Test;

boolean Connect = false ;

char Manuel;

//+++++++++++++++++++++++++++++++Fonction+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID : ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("Adresse IP : ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Force du signal (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.print("En attente de commandes...");
  Serial.println();
}

void rouge() {
  int i;
  for (i = 0 ; i < 24 ; i++) {
    red.setPixelColor(i, 255, 0 , 0);
    red.show();

  }
}

void orange() {
  int i;
  for (i = 0 ; i < 24 ; i++) {
    oran.setPixelColor(i, 255, 120 , 0);
    oran.show();

  }
}

void vert() {
  int i;
  for (i = 0 ; i < 24 ; i++) {
    green.setPixelColor(i, 0, 255 , 0);
    green.show();

  }
}

void rougeOFF() {
  int i;
  for (i = 0 ; i < 24 ; i++) {
    red.setPixelColor(i, 0, 0 , 0);
    red.show();

  }
}

void orangeOFF() {
  int i;
  for (i = 0 ; i < 24 ; i++) {
    oran.setPixelColor(i, 0, 0 , 0);
    oran.show();

  }
}

void vertOFF() {
  int i;
  for (i = 0 ; i < 24 ; i++) {
    green.setPixelColor(i, 0, 0 , 0);
    green.show();

  }
}

void Traitement () {
  Commande = IHM.readStringUntil("\n");
  Serial.println(Commande) ;
  Mod = Commande[0];
  Serial.println(Mod);
  switch (Mod)
  {
    case 'A':
      TempR = Commande.substring(1, Commande.indexOf('R') ).toInt();
      Serial.println(TempR);
      TempV = Commande.substring(Commande.indexOf('R') + 1 , Commande.indexOf('V') ).toInt();
      Serial.println(TempV);
      IHM.println('W');   //Pour confirmer la réception
      break;
    case 'M':
      Manuel = Commande[1];
      Serial.println(Manuel);
      break;
    case 'B':
      int sensorValue = analogRead(A0);
      Serial.print(sensorValue);
      float pourcent = map(sensorValue, 720, 1023, 0, 100);
      Serial.println(" : " + String(pourcent) + " %");
      break;
  }
}

void setup() {
  Serial.begin(9600);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);       //ne pas continuer
  }

  String fv = WiFi.firmwareVersion();
  if ( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");

  // attempt to connect to Wifi network:


  while ( status != WL_CONNECTED) {
    Serial.println("*** Serveur Web WiFi ***");
    Serial.print("En attente de connexion au reseau : ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);
  }

  server.begin();
  printWifiStatus();
  red.begin();
  red.setBrightness(100);
  red.show();
  oran.begin();
  oran.setBrightness(100);
  oran.show();
  green.begin();
  green.setBrightness(100);
  green.show();

}

void loop() {

  IHM = server.available();
  if (IHM) {
    
    Commande = "";
    Mod = "";
    
    if (IHM.available()) {
      Traitement();
    }

    //++++++++++++++++++++++++++++++++++++++Rendu du mod de fonctionnement +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    while (Mod == 'A')
    {
        //++++++++++++++PASSAGE AU ROUGE+++++++++++++

      if (Mod == 'A') {
        Test = 0;
        rouge();
        Serial.println("Rouge Allumer");
        while (Test < TempR)
        {
          if (Serial.available())
          {
            Traitement();
          }
          Test++;
          delay(1000);
          Serial.println("Pour une duree de :");
          Serial.print(TempR - Test);
          Serial.println(" Seconde(s)");
        }
        rougeOFF();
      }
      //++++++++++++++PASSAGE AU VERT+++++++++++++

      if (Mod == 'A') {
        Test = 0;
        vert();
        Serial.println("Vert Allumer");
        while (Test < TempV)
        {
          if (Serial.available())
          {
            Traitement();
          }
          Test++;
          delay(1000);
          Serial.println("Pour une duree de :");
          Serial.print(TempV - Test);
          Serial.println(" Seconde(s)");
        }
        vertOFF();
      }
      //++++++++++++++PASSAGE AU ORANGE+++++++++++++


      if (Mod == 'A') {
        Test = 0;
        orange();
        Serial.println("Orange Allumer");
        while (Test < TempO)
        {
          if (Serial.available())
          {
            Traitement();
          }
          Test++;
          delay(1000);
          Serial.println("Pour une duree de :");
          Serial.print(TempO - Test);
          Serial.println(" Seconde(s)");
        }
        orangeOFF();

      }
    }
    Serial.println(Mod);
    if (Mod == 'M') {
      Serial.println("mode mannuel");
      if (Manuel == 'R') {
        orangeOFF();
        vertOFF();
        rouge();
        Serial.print('K'); //Message de confirmation
      }

      if (Manuel == 'O') {
        orange();
        vertOFF();
        rougeOFF();
        Serial.print('K'); //Message de confirmation
      }

      if (Manuel == 'V') {
        orangeOFF();
        vert();
        rougeOFF();
        Serial.print('K'); //Message de confirmation
      }

    }
  }
}

