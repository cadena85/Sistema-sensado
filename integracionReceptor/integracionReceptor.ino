//LibrerÃ­as para LoRa
#include <LoRa.h>
#include <SPI.h>

//Libraries para comunicar con y dibujar en la pantalla OLED integrada
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//OLED pins
#define OLED_SDA 21
#define OLED_SCL 22 
#define OLED_RST 23
#define SCREEN_WIDTH 128   // Ancho del display OLED
#define SCREEN_HEIGHT 64   // Alto del display OLED

//Se definen los pines que serán usados en el módulo transeptor LoRa
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
//433E6 para Asia, 866E6 para Europa, 915E6 para América
#define BAND 433E6 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
String DatoLoRa;//Cadena de texto para recibir datos del otro LoRa.

//Initialize OLED display
void startOLED(){
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER");
}

void startLoRA(){
  Serial.println("Prueba de recepcion LoRa");

  SPI.begin(SCK, MISO, MOSI, SS);  //Definimos pines SPI
  LoRa.setPins(SS, RST, DIO0); //Configuramos el LoRa para enviar

  if (!LoRa.begin(BAND)) {//Intenta transmitir en la banda elegida
    Serial.println("Error iniciando LoRa");//Si no puede transmitir, marca error
    while (1);
  }
  Serial.println("Inicio exitoso de LoRa!");//Mensaje de todo bien en puerto serial
  display.setCursor(0, 30);
  display.print("Inicio exitoso de LoRa!");//Mensaje de todo bien en pantalla OLED
  display.display();
  delay(2000);
}

void servicesHttp() {
  
}

void dataReceiver() {
    int tamanoPaquete = LoRa.parsePacket();  //analizamos paquete
  if (tamanoPaquete) {//Si nos llega paquete de datos
    Serial.print("Paquete recibido ");//Muestra confirmaciÃ³n

    while (LoRa.available()) {//Leemos el paquete
      DatoLoRa = LoRa.readString();//Guardamos cadena en variable
      Serial.print(DatoLoRa);//Lo imprimimos en monitor serial
    }
    //+++++En esta linea se puede agregar el código para enviar 
    // los datos por http ++++ Recomendación, hagan una función 
    
//A partir de aqui sólo es para mostrar en display de lora32
    int rssi = LoRa.packetRssi();//Esto nos imprime la intensidad de seÃ±al recibida
    
    // Mostramos información captada
    display.clearDisplay();
    //display.setCursor(0, 0);
    //display.print("Paquete recibido:");//Imprime datos recibidos
    display.setCursor(0, 0);
    display.print(DatoLoRa);
    display.setCursor(0, 20);
    display.print("RSSI:");//Imprime intensidad de seÃ±al
    display.setCursor(30, 30);
    display.print(rssi);
    display.display();
//Aquí termina de agregar elementos para mostrar en display de lora32
  }
}

void setup() {
  Serial.begin(115200);
  startOLED();
  startLoRA();
}

void loop() {
  dataReceiver();
}
