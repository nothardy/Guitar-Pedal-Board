// sector oled
  /**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };
// sector oled

byte array[4000];
float Ts, Fs, p, contador,contador2;
int Entrada = A0,deltap=0, deltapi=0;
int Pote = A1;
int boton = 52, variablepantalla=0;
//int deltapi=A3;
float x, y, aux;
char i = 0;
int Estado = 0;
// Filtro distorcion:
float a, k;

// Captura muestras:
//int muestras[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float muestras[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int largo = 20, counter=0;
int m1 = 0,poteparapantalla=0;

// Filtro pasabajos: // fc =
float pasabajo[] = {0.007208453, 0.009279120, 0.015254977, 0.024591563, 0.036399924, 0.049534452, 0.062707537, 0.074619369, 0.084089565, 0.090177069, 0.092275933, 0.090177069, 0.084089565, 0.074619369, 0.062707537, 0.049534452, 0.036399924, 0.0245915, 0.015254977, 0.009279120, 0.007208453};

// Filtro pasaaltos: // fc =
float pasaaltos[] = { -0.000633684536378499, -0.000813651322003520, -0.00133463273299489, -0.00214719937735983, -0.00317277596807213, -0.00431136703738754, -0.00545144422357284, -0.00648100950481548, -0.00729872804404562, -0.00782401509878111, 0.992628046329433, -0.00782401509878111, -0.00729872804404562, -0.00648100950481548, -0.00545144422357284, -0.00431136703738754, -0.00317277596807213, -0.00214719937735983, -0.00133463273299489, -0.000813651322003520, -0.000633684536378499};


// Filtro reverb
#define sizebuffer 8000 // Tamaño del buffer
int bufer[sizebuffer]; // declaro el bufer circular

#define t1 (sizebuffer -(sizebuffer / 5))
#define t2 (sizebuffer - 1)
int muestra1; //valores anteriores de la muestra, muestra1 y muestra2
int muestra2;
int Salida; // Valor que va a ir a la salida y va ser guardado en el buffer en la posicion muestraactual
int muestraactual = 0; //  El valor de mustra actual, incrementado despues de cada muestreo.
int muestra; //Variable temporal usada para indexar los valores anteriores de muestra
long loopcount = 0; //  Contador para debugear escrituras en el puerto serial


void setup()
{

  //sector oled
 Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  

  // Clear the buffer
  //display.clearDisplay();

//  // Draw a single pixel in white
//  display.drawPixel(10, 10, WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  //display.display();
 // delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

//  testdrawline();      // Draw many lines
//
//  testdrawrect();      // Draw rectangles (outlines)
//
//  testfillrect();      // Draw rectangles (filled)
//
//  testdrawcircle();    // Draw circles (outlines)
//
//  testfillcircle();    // Draw circles (filled)
//
//  testdrawroundrect(); // Draw rounded rectangles (outlines)
//
//  testfillroundrect(); // Draw rounded rectangles (filled)
//
//  testdrawtriangle();  // Draw triangles (outlines)
//
//  testfilltriangle();  // Draw triangles (filled)
//
//  testdrawchar();      // Draw characters of the default font
//
//  testdrawstyles();    // Draw 'stylized' characters
//
//  testscrolltext();    // Draw scrolling text
//
//  testdrawbitmap();    // Draw a small bitmap image

//  // Invert and restore display, pausing in-between
//  display.invertDisplay(true);
//  delay(1000);
//  display.invertDisplay(false);
//  delay(1000);
//
//  testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps

  //sector oled
  
  //Serial.begin(9600);

  pinMode(DAC0, OUTPUT);
  pinMode(DAC1, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(boton, INPUT);
 


  Fs = 50000; // Fs en kHz
  Ts = 1 / (Fs * 1000); // Ts en microsegundos

  analogWriteResolution(12);
  analogReadResolution(12);

  // Filtro distorcion:
  a = 0.1;
  k = (2 * a) / (1 - a);


  // inicializo el buffer en cero
  for (int i = 0; i < sizebuffer; i++)
    bufer[i] = 0;
inicio();
//attachInterrupt(A3, refresh, CHANGE);
//attachInterrupt(A3, refresh, CHANGE);


}

void loop()
{
  switch (Estado)
  {
    case 0:
    
      if(variablepantalla==0)
      {
        
         sinfiltro();
      }
      variablepantalla=1;
    
      Prueba();
  
    break;
    case 1:
      if(variablepantalla==1)
      {
        lowpass();
      }
      variablepantalla=2,
      PasaBajos();
      break;

    case 2:
      if(variablepantalla==2)
      {
        highpass();
      }
      variablepantalla=3;
      PasaAltos();
      break;

    case 3:
      
      
      Distorsion();
      if(variablepantalla==3)
      {
        distortion();
      }
      variablepantalla=4;
      counter++;
      if(counter>300000){
        distortion();
        counter=0;
      }
      
      break;

     case 4:
     
      if(variablepantalla==4){
     flangerpantalla();
      }
      variablepantalla=5;
      Flanger();
      break;

      case 5:
      if(variablepantalla==5)
      {
        overdrive();
      }
      variablepantalla=0;
      Overdrive();
      break;

  }
  LeerBoton();
  //  Prueba();
}

void Prueba()
{
  CapturaMuestras();
  analogWrite(DAC0, muestras[5]);
  delayMicroseconds(Ts);
}

void LeerBoton()
{
  if (digitalRead(boton) == LOW)
  {
    Estado++;
    while (digitalRead(boton) == LOW);
  }
  if (Estado == 6)
  {
    Estado = 0;
  }
}

void Distorsion()
{
  Fs = 90000;
  Ts = 1 / (Fs * 1000);
  x = analogRead(Entrada);
  x=(x-2048)/2048;
  if (x > 0)
  {
    y = ((x + (k * x)) / (1 + (k * x)));                   //*x;//*x/270; // a=0.5 y x/256
  }
  if (x < 0)
  {
    y = ((x + (k * x)) / (1 + (k * (-x))));              //*x;//*x/256; // a=0.5 y x/256
  }
  y = (y * 2048) + 2048;
  contador++;
//  contador2++;
//  if(contador2 == 80000)
//  {
//    contador2=0;
//    //nivel();
//  }
  if (contador == 40000)
  {
    contador = 0;
    p = analogRead(Pote);
//    Serial.println(p);
    a = (p/(5*4097.0))+0.8;
    k = (2 * a) / (1 - a);
    poteparapantalla = int(100*(p/4097.0));
//    deltap=100-poteparapantalla;
//    if(deltap>75)
//    {//deltapi=80;
//      digitalWrite(A3,80);}
//    if(deltap>50 && deltap<75)
//    {deltapi=50;
//    digitalWrite(A3,50);}
//    if(deltap>25 && deltap<50)
//    {deltapi=25;
//    digitalWrite(A3,25);}
//    if(deltap<25)
//    {deltapi=10;
//    digitalWrite(A3,10);}
    
    //distortion();
  } 
  analogWrite(DAC0, y);
  analogWrite(DAC1, x);
  delayMicroseconds(Ts);
}

void PasaBajos()
{
  CapturaMuestras();
  y = 0;
  i = 0;
  while (i < largo)//4030
  {
    aux = muestras[20 - i] * pasabajo[i];
    y = y + aux;
    i++;
  }
  analogWrite(DAC0, y);
  analogWrite(DAC1, m1);
  delayMicroseconds(Ts);
}

void PasaAltos()
{
  CapturaMuestras();
  y = 0;
  i = 0;
  while (i < largo)
  {
    aux = muestras[20 - i] * pasaaltos[i];
    y = y + aux;
    i++;
  }
  analogWrite(DAC0, y);
  analogWrite(DAC1, m1);
  delayMicroseconds(Ts);
}

void CapturaMuestras ()
{
  m1 = analogRead(A0);
  i = 1;
  while (i <= (largo))
  {
    muestras[20 - i + 1] = muestras[largo - i];
    i++;
  }
  muestras[0] = m1;
}

//sector oled
void testdrawline() {
  int16_t i;

  display.clearDisplay(); // Clear display buffer

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
  for(i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, WHITE);
    display.display();
    delay(1);
  }
  for(i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE);
    display.display();
    delay(1);
  }

  delay(2000); // Pause for 2 seconds
}

void testdrawrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testfillrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillcircle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(1);
  }

  delay(2000);
}

void testdrawroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // The INVERSE color is used so round-rects alternate white/black
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawtriangle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfilltriangle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
    // The INVERSE color is used so triangles alternate white/black
    display.fillTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}

void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMFLAKES][3];

  // Initialize 'snowflake' positions
  for(f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
    icons[f][YPOS]   = -LOGO_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
  }

  for(;;) { // Loop forever...
    display.clearDisplay(); // Clear the display buffer

    // Draw each snowflake:
    for(f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
    }

    display.display(); // Show the display buffer on the screen
    delay(200);        // Pause for 1/10 second

    // Then update coordinates of each flake...
    for(f=0; f< NUMFLAKES; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - LOGO_WIDTH, display.width());
        icons[f][YPOS]   = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
}

void inicio(void){
  display.clearDisplay();
  display.display();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

//  // Not all the characters will fit on the display. This is normal.
//  // Library will draw what it can and the rest will be clipped.
//  for(int16_t i=0; i<256; i++) {
//    if(i == '\n') display.write(' ');
//    else          display.write(i);
//  }
display.println(F("Proyecto DSP 2019"));

  display.display();
  delay(6000);
}

void sinfiltro(void)
{
  display.clearDisplay();
  display.display();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

//  // Not all the characters will fit on the display. This is normal.
//  // Library will draw what it can and the rest will be clipped.
//  for(int16_t i=0; i<256; i++) {
//    if(i == '\n') display.write(' ');
//    else          display.write(i);
//  }
display.println(F("1) Sin Filtro"));


  display.display();
  
}
void lowpass(void)
{
  display.clearDisplay();
  display.display();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

//  // Not all the characters will fit on the display. This is normal.
//  // Library will draw what it can and the rest will be clipped.
//  for(int16_t i=0; i<256; i++) {
//    if(i == '\n') display.write(' ');
//    else          display.write(i);
//  }
display.println(F("2) Filtro Pasabajo"));
  display.display();
  
}
void highpass(void)
{
  display.clearDisplay();
  display.display();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

//  // Not all the characters will fit on the display. This is normal.
//  // Library will draw what it can and the rest will be clipped.
//  for(int16_t i=0; i<256; i++) {
//    if(i == '\n') display.write(' ');
//    else          display.write(i);
//  }
display.println(F("3) Filtro Pasaalto"));
  display.display();
  //delay(2000);
}
void distortion(void)
{
  display.clearDisplay();
  display.display();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

//  // Not all the characters will fit on the display. This is normal.
//  // Library will draw what it can and the rest will be clipped.
//  for(int16_t i=0; i<256; i++) {
//    if(i == '\n') display.write(' ');
//    else          display.write(i);
//  }
display.println(F("4) Filtro Distorsivo"));
display.display();
display.setCursor(0,20);
display.println((String)"Nivel:" +poteparapantalla );
//display.println(F(text)); 
  display.display();
  //delay(2000);
}

//void nivel(void){
//
//  display.println(F("  "));
//  display.display();
//  display.println((String)"Nivel:" +poteparapantalla );
////display.println(F(text)); 
//  display.display();
//}
void refresh(){
  if(Estado==3){
  distortion();
  }
}

void flangerpantalla(){
  display.clearDisplay();
  display.display();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

//  // Not all the characters will fit on the display. This is normal.
//  // Library will draw what it can and the rest will be clipped.
//  for(int16_t i=0; i<256; i++) {
//    if(i == '\n') display.write(' ');
//    else          display.write(i);
//  }
display.println(F("5) Filtro Flanger"));
  display.display();
}

void Flanger(){

  for(int i=0; i<4000;i++){
    
  CapturaMuestras();
  int k=round(5*sin(2*3.141561*(50)*i));
  int j=round(3*sin(2*3.141561*(60)*i));
  analogWrite(DAC0, 0.5*muestras[5]+muestras[5+k]+muestras[5+j]);
  delayMicroseconds(Ts);
  }
  
}

void Overdrive(){

//  for(int i = 0; i < 4000; i ++) { // set up a loop    
//    int k=round(sin(2*3.141561*(0.03/Fs)*i));
//        //array[i] = array[i]+ array[i - k]/2; //removes noise and some delay
//        analogWrite(DAC0, (array[i]+array[i-k])/1.3);
//        array[i] = analogRead(A0);
//}
CapturaMuestras();
int k=round(sin(2*3.141561*(0.3/Fs)*i));
  analogWrite(DAC0, muestras[5]+ muestras[5]);
  delayMicroseconds(Ts);
      
}
void overdrive(){
  display.clearDisplay();
  display.display();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

//  // Not all the characters will fit on the display. This is normal.
//  // Library will draw what it can and the rest will be clipped.
//  for(int16_t i=0; i<256; i++) {
//    if(i == '\n') display.write(' ');
//    else          display.write(i);
//  }
display.println(F("6) Filtro Overdrive"));
  display.display();
}

//sector oled
