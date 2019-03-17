#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_TFTLCD.h" // Hardware-specific library
#include "TouchScreen.h"

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin


#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 896//850//920
#define TS_MAXY 850//891//940

#define MINPRESSURE 10
#define MAXPRESSURE 1000
// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// LCD Data Bit : 0   1   2   3   4   5   6   7
// Uno dig. pin : 8   9   2   3   4   5   6   7
//                D8  D9  D2  D3  D4  D5  D6  D7
// Uno port/pin : PB0 PB1 PD2 PD3 PD4 PD5 PD6 PD7

// ARDUINO UART PORTS: RX   TX
// PORTS:              D10  D11

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE  0xFC00
#define DARKGREY 0x8410
#define LIGHTGREY 0xC618

#define refrescar   tft.fillScreen
#define impln       tft.println
#define imp         tft.print

#define tftAncho    240
#define tftAlto     320

// /--> cantidadChar: define la cantidad de char que se leen del string enviado por la UART de la Tiva C
// /--> cantidadVariables: define la cantidad de variables que se envian desde la TIVA C
// /--> cantidadCharCheck: define cuantos de esos (cantidadChar) char se deben leer para comprobar cual dato esta recibiendo
#define cantidadVariables             8
// --> Matriz que contiene los identificadores
#define IDvar1  1
#define IDvar2  2
#define IDvar3  3
#define IDvar4  4
#define IDbtnStop 5
#define IDbtnStart 6
#define IDCon 7
#define IDFinish 8
//El orden de los valores de la matriz va segun la matriz "comparador"
float var1, var2, var3, var4, flagStart, flagStop, flagCon, flagFinish;
uint16_t identificador[cantidadVariables] = {  //Esta matriz tiene las ID y los punteros a las variables
  IDvar1,
  IDvar2,
  IDvar3,
  IDvar4,
  IDbtnStop,
  IDbtnStart,
  IDCon,
  IDFinish
};
float *variables [cantidadVariables] = {&var1, &var2, &var3, &var4, &flagStop, &flagStart, &flagCon, &flagFinish};

String inString = "";
bool flagID = false;  //Se usa para ver si se recibio hasta la ID
int inID;             //ID recibida por UART
float inDato;         //Dato recibido por UART
bool flagRx = false;  //Flag que finalizo de recibir por UART

uint8_t estado = 0;
#define waitCon 0
#define waitStart 1
#define waitStop 2
#define finish  3
#define desconectado 4

#define btnStartRadius  100
#define btnStartX   120-btnStartRadius
#define btnStartY   160-btnStartRadius
#define btnStartW   btnStartRadius*2
#define btnStartH   btnStartRadius*2

#define btnStopX  10
#define btnStopY  270
#define btnStopW  240-btnStopX*2
#define btnStopH  40

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint p;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SETUP
void setup(void) {
  Serial.begin(9600);
  tft.reset();
  uint16_t identifier = tft.readID();
  delay(1000);
  if (identifier == 0x0101)
    identifier = 0x9341;
  tft.begin(identifier);
  refrescar(BLACK);
}
void loop(void) {

  if (flagRx == true) {
    //Serial.println("writting");
    write_menu_principal();
    flagRx = false;
  }
}//LOOP

void checkState (void) {
  if (estado == waitCon) {
    serialCheck();
    if (flagCon == 1) {
      estado = waitStart;
      flagCon = 0;
    }
  } else if (estado == waitStart) {
    if (check_boton(btnStartX, btnStartY, btnStartW, btnStartH)) {
      Serial.print(IDbtnStart);  Serial.print("|");
      Serial.print("1");         Serial.print("\n");
      estado = waitStop;
    }
  } else if (estado == waitStop) {
    serialCheck();
    if (check_boton(btnStopX, btnStopY, btnStopW, btnStopH)) {
      Serial.print(IDbtnStop); Serial.print("|");
      Serial.print("1");       Serial.print("\n");
      estado = desconectado;
    }
    if (flagRx == true) {
      write_datos();
      flagRx = false;
    }
    if (flagFinish == 1) {
      estado = finish;
      flagFinish = 0;
    }
  } else if (estado == finish) {
  } else if (estado == desconectado) {
  }
}
void write_datos(void) {
  char valor[8];
  dtostrf(var1, 8, 4 , valor);
  drawString(10 + 14 * 5, 30 + 5 ,             valor, WHITE, ORANGE, 2);
  dtostrf(var2, 8, 4 , valor);
  drawString(10 + 14 * 5, 30 + 5 * 2 + 14,     valor, WHITE, ORANGE, 2);
  dtostrf(var3, 8, 4 , valor);
  drawString(10 + 14 * 5, 30 + 5 * 3 + 14 * 2, valor, WHITE, ORANGE, 2);
  dtostrf(var4, 8, 4 , valor);
  drawString(10 + 14 * 5, 30 + 5 * 4 + 14 * 3, valor, WHITE, ORANGE, 2);
}
void write_menus(void) {
  if (estado == waitCon) {
    waitConMenu();
  } else if (estado == waitStart) {
    waitStartMenu();
  } else if (estado == waitStop) {
    waitStopMenu();
  } else if (estado == finish) {
    inishMenu();
  } else if (estado == desconectado) {
    desconectadoMenu();
  }
}
void waitConMenu (void) {
  tft.fillRoundRect(10, 10, 240 - 10 * 2, 320 - 10 * 2, 50, BLUE);
  drawCenterString (10, 120, 240 - 10 * 2, 3 * 7,                     "Esperando", WHITE, 3);
  drawCenterString (10, 120 + 7 * 3 + 10, 240 - 10 * 2, 3 * 7,        "que conecte", WHITE, 3);
  drawCenterString (10, 120 + 7 * 3 * 2 + 10 * 2, 240 - 10 * 2, 3 * 7,  "el vehiculo", WHITE, 3);
}
void waitStartMenu (void) {
  tft.fillCircle(btnStartX + btnStartRadius, btnStartY + btnStartRadius, btnStartRadius, GREEN);
  drawCenterString(btnStartX, btnStartY, btnStartW, btnStartH, "START", BLACK, 3);
}
void waitStopMenu (void) {
  tft.fillRoundRect(5, 30, 240 - 10,  5 * 5 + 14 * 4, 10, ORANGE);

  drawString(10, 30 + 5 ,             "Tiempo de carga: ", WHITE, ORANGE, 2);
  drawString(10, 30 + 5 * 2 + 14,     "Estado de carga: ", WHITE, ORANGE, 2);
  drawString(10, 30 + 5 * 3 + 14 * 2, "Energia consumida: ", WHITE, ORANGE, 2);
  drawString(10, 30 + 5 * 4 + 14 * 3, "Costo: ", WHITE, ORANGE, 2);

  char valor[8];
  dtostrf(var1, 8, 4 , valor);
  drawString(10 + 14 * 5, 30 + 5 ,             valor, WHITE, ORANGE, 2);
  dtostrf(var2, 8, 4 , valor);
  drawString(10 + 14 * 5, 30 + 5 * 2 + 14,     valor, WHITE, ORANGE, 2);
  dtostrf(var3, 8, 4 , valor);
  drawString(10 + 14 * 5, 30 + 5 * 3 + 14 * 2, valor, WHITE, ORANGE, 2);
  dtostrf(var4, 8, 4 , valor);
  drawString(10 + 14 * 5, 30 + 5 * 4 + 14 * 3, valor, WHITE, ORANGE, 2);

  tft.fillRoundRect(btnStopX, btnStopY, btnStopW, btnStopH, 10, RED);
  drawCenterString(btnStopX, btnStopY, btnStopW, btnStopH, "STOP", WHITE, 3);
}
void finishMenu (void) {
  tft.fillRoundRect(10, 10,
                    240 - 10 * 2, 320 - 10 * 2,
                    50, BLUE);
  tft.fillRoundRect(20, 120 - 10,
                    240 - 20 * 2, 3 * 7 * 2 + 10 * 4,
                    10, RED);
  drawCenterString (30, 120, 240 - 30 * 2,
                    3 * 7,
                    "Carga", WHITE, 3);
  drawCenterString (30, 120 + 7 * 3 + 10,
                    240 - 30 * 2, 3 * 7,
                    "finalizada", WHITE, 3);
}
void desconectadoMenu (void) {
  tft.fillRoundRect(10, 10,
                    240 - 10 * 2, 320 - 10 * 2,
                    50, BLUE);
  tft.fillRoundRect(20, 120 - 10,
                    240 - 20 * 2, 3 * 7 + 10 * 2,
                    10, RED);
  drawCenterString (30, 120, 240 - 30 * 2,
                    3 * 7,
                    "Desconectado", WHITE, 3);
}
void drawString(uint16_t x, uint16_t y, char *c,
                uint16_t color, uint16_t bg, uint8_t size) {
  while (c[0] != 0) {
    tft.drawChar(x, y, c[0], color, bg, size);
    x += size * 6;
    c++;
  }
}
void drawCenterString(uint16_t pox, uint16_t posy,
                      uint16_t _W, uint16_t _H,
                      char *texto, uint16_t color,
                      uint8_t size) {
  char _label[50];
  uint8_t _textsize = size;
  strncpy(_label, texto, 19);
  tft.setCursor((pox + _W / 2) - strlen(_label) * 3 * _textsize,
                (posy + _H / 2) - 4 * _textsize);
  tft.setTextColor(color);
  tft.setTextSize(_textsize);
  tft.print(_label);
}
void map_click (void) {
  //Detecta donde se presiono
  p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //Verifica si se presiono lo suficiente
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    //    Serial.println(p.x); Serial.println(p.y);
  }
}
boolean check_boton(int x, int y, int _x, int _y, int _w, int _h) {
  if ((x > _x && x < (_x + _w)) && (y > _y && y < (_y + _h)) ) return true;
  return false;
}

void serialCheck (void) {
  while (Serial.available() > 0) {
    Serial.println("por alguna razon si no pongo esto no anda");
    int inChar = Serial.read();
    //Serial.println(Serial.read());
    //Verifico si ya lei hasta la ID (hasta que se recibe '|')
    if (!flagID) {
      if (isDigit(inChar)) {
        inString += (char)inChar;
      } else if (inChar == '|') { //Finalizacion de la ID
        inID = inString.toInt();
        //Serial.print("ID: "); Serial.println(inID);
        inString = "";
        flagID = true;
      }
    } else {  //Leo datos
      if (inChar != '\n') {   //Guardo datos hasta que recibo un \n
        inString += (char)inChar;
      } else {
        inDato = inString.toFloat();
        //Serial.print("Dato: "); Serial.println(inDato);
        inString = "";
        flagID = false;
        if (inID == IDFinish && inDato == 1) {
          estado = finish;
        } else {
          for (int i = 0; i < cantidadVariables; i++) {
            if (identificador[i] == inID) {
              //Serial.print("ID igual al i: ");Serial.println(i);
              *(variables[i]) = inDato;
              flagRx = true;
              break;
            }
          }
        }
      }
    }
  } //  if (Serial.available() > 0) {
}
