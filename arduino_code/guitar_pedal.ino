/** DSP Filter For Guitar. This filter works in 3 different ways:
 * 
 * First state is a clean guitar.
 * 
 * Second state is a distortion.
 * 
 * Third state is a Flanger
 *
 */



#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>




// byte array[4000];
float Ts, Fs, p; //sampleCounter
int Entrada = A0;
int Potenciometer = A1;
int button = 52;
//int deltapi=A3;
float x, y, aux;
char i = 0;
int Effect = 1;
// Filtro distorcion:
float a, k;

// Captura sample:

float sample[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int sampleLength = 20, sampleCounter=0;
int firstSample = 0,poteparapantalla=0;

// Filtro pasabajos: // fc =
float pasabajo[] = {0.007208453, 0.009279120, 0.015254977, 0.024591563, 0.036399924, 0.049534452, 0.062707537, 0.074619369, 0.084089565, 0.090177069, 0.092275933, 0.090177069, 0.084089565, 0.074619369, 0.062707537, 0.049534452, 0.036399924, 0.0245915, 0.015254977, 0.009279120, 0.007208453};

// Filtro pasaaltos: // fc =
float pasaaltos[] = { -0.000633684536378499, -0.000813651322003520, -0.00133463273299489, -0.00214719937735983, -0.00317277596807213, -0.00431136703738754, -0.00545144422357284, -0.00648100950481548, -0.00729872804404562, -0.00782401509878111, 0.992628046329433, -0.00782401509878111, -0.00729872804404562, -0.00648100950481548, -0.00545144422357284, -0.00431136703738754, -0.00317277596807213, -0.00214719937735983, -0.00133463273299489, -0.000813651322003520, -0.000633684536378499};


// Filtro reverb
#define sizebuffer 8000 // Tamaño del buffer
int bufer[sizebuffer]; // declaro el bufer circular

#define t1 (sizebuffer -(sizebuffer / 5))
#define t2 (sizebuffer - 1)

long loopcount = 0; //  Contador para debugear escrituras en el puerto serial


void setup()
{

  
  
  //sector oled
  
// I/O Declaration
  pinMode(DAC0, OUTPUT);
  pinMode(DAC1, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(button, INPUT);
 


  Fs = 50000; // Fs en kHz
  Ts = 1 / (Fs * 1000); // Ts en microsegundos

  analogWriteResolution(12);
  analogReadResolution(12);

  // Filtro distorcion: // VOY A VER SI SIRVE O SI LO SACO
  a = 0.1;
  k = (2 * a) / (1 - a);


  // inicializo el buffer en cero
  for (int i = 0; i < sizebuffer; i++){
    bufer[i] = 0;
  }
}

void guitarTest()
{
  x = analogRead(Entrada);
  analogWrite(DAC0, analogRead(Entrada));
}

void buttonRead()
{
  if (digitalRead(button) == LOW)
  {
    Effect++;
    while (digitalRead(button) == LOW);
  }
  if (Effect == 3){
    Effect=0;
    for (int i = 0; i < sizebuffer; i++){
    bufer[i] = 0;
  }
    float sample[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  }
  
}

void guitarDistortion()
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
  sampleCounter++;

  if (sampleCounter == 40000)
  {
    sampleCounter = 0;
    p = analogRead(Potenciometer);

    a = (p/(5*4097.0))+0.8;
    k = (2 * a) / (1 - a);
    poteparapantalla = int(100*(p/4097.0));

  } 
  analogWrite(DAC0, y);
  delayMicroseconds(Ts);
}





void sampleCapture ()
{
  firstSample = analogRead(A0);
  i = 1;
  while (i <= (sampleLength))
  {
    sample[20 - i + 1] = sample[sampleLength - i];
    i++;
  }
  sample[0] = firstSample;
}



void guitarFlanger(){

  for(int i=0; i<4000;i++){
    
  sampleCapture();
  int k=round(5*sin(2*3.141561*(50)*i));
  int j=round(3*sin(2*3.141561*(60)*i));
  analogWrite(DAC0, (0.3*sample[5]+0.3*sample[5+k]+0.3*sample[5+j]));
  delayMicroseconds(Ts);
  }
  
}

void guitarOverdrive(){

//if (sampleCounter == 40000)   // UNA VEZ QUE ANDE REGULO EL POTENCIOMETRO
//  {
//    sampleCounter = 0;
//    p = analogRead(Potenciometer);
//
//    a = (p/(5*4097.0))+0.8;
//    k = (2 * a) / (1 - a);
//    poteparapantalla = int(100*(p/4097.0));
//
//  } 

//------------ STARTS HERE-----------------------//
x=analogRead(Entrada);
int potenciometer = 0.92;
int constantK = 2*potenciometer/(1-potenciometer);

y = 1.7*((1+constantK)*(x)/(1+constantK*abs(x)));

//for(int i=0;i<sampleLength;i++){
//  y[i]=(1+constantK)*(x[i])/(1+constantK*abs(x[i]));
//}

analogWrite(DAC0,y);
}

void newButtonRead()
{
  if (digitalRead(button) == LOW)
  { while (digitalRead(button) == LOW);
    switch(Effect){
    case 0:{
    while (digitalRead(button) == LOW);
    Effect=1;
    break;}

    case 1:{
      while (digitalRead(button) == LOW);
      Effect=2;
      break;}
   case 2:{
    while (digitalRead(button) == LOW);
    Effect=0;
    for (int i = 0; i < sizebuffer; i++){
    bufer[i] = 0;
  }
    float sample[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    break;}
   default:
   break;
      }
  }
  while (digitalRead(button) == LOW);
}





void loop()
{
  switch (Effect)
  {
    case 0:
      guitarTest();
    break;

    case 1:
      //guitarDistortion();
      guitarOverdrive();
      sampleCounter++;
//      if(sampleCounter>300000){
//        sampleCounter=0;
//      }
      break;
//    case 2:
//      //guitarFlanger();
//      break;

    default:
    break;
  }
  newButtonRead();
}




  
