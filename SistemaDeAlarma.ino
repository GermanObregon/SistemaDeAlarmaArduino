#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

const byte filas = 4;
const byte columnas = 4;
char keys[filas][columnas] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pinesFilas[filas] = {9,8,7,6};
byte pinesColumnas[columnas] = {5,4,3,2};
Keypad teclado = Keypad(makeKeymap(keys),pinesFilas,pinesColumnas,filas,columnas);
char tecla;
char clave[7];
char clave_default[7] = "123456";
byte indice = 0;
LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7);

String men1 = "Alarma ACTIVADA";
String men2 = "Alarma DESACT.";
int cantidadMov = 0;
int stringStart = 0 ;
int stringStop = 16;
int contador = 0;

bool estadoAlarma = false;
int cursor;
bool backLight = true;
bool estadoBocina = false;
int PIR = 12;
int estadoPIR;
int RELE = 10;


void setup() {
  
  lcd.setBacklightPin(3 , POSITIVE);
  lcd.setBacklight(HIGH);
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  pinMode(PIR , INPUT);
  pinMode(RELE , OUTPUT);
  digitalWrite(RELE, HIGH);
  delay(20000);
  

}
void ImprimirEnPantalla(String men){
    if(cantidadMov != men.length() - 16){
      cantidadMov = men.length() - 16;
      }
    
    if(men.length()<=16){
      lcd.setCursor(0,0);
      lcd.print(men);
    }
    else if (men.length() > 16) {
      lcd.setCursor(0,0);
      lcd.print(men.substring(stringStart,stringStop));
      delay(1000);
      lcd.clear();
    }
    
    if(contador != cantidadMov){
      stringStart++;
      stringStop++;
      contador++;
    }
    else{
      stringStart = 0;
      stringStop = 16;
      contador = 0;
    }
 }

  void ActivarAlarma(){
      cursor = 0;
      lcd.clear();
      while(indice!=6){
        ImprimirEnPantalla("Ingrese Pass.:");
        tecla = teclado.getKey();
        if(tecla == 'C'){
          loop();
          }
        if(tecla){
        lcd.setCursor(cursor,1);
        clave[indice] = tecla;
        lcd.print(tecla);
        indice++;
        cursor++;
        
        }
      }
            
      if (indice == 6){
        if(!strcmp(clave , clave_default)){
          lcd.clear();
          ImprimirEnPantalla("Pass. Correcto");
          if(estadoAlarma == true){
              estadoAlarma = false;
              OnOffBocina();
              stringStart = 0;
              stringStop = 16;
              contador = 0;
              delay(2000);
              lcd.clear();
            }
            else{
              estadoAlarma = true;
              delay(2000);
              lcd.clear();    
            }
          indice = 0;
        }
        else{
          lcd.clear();
          ImprimirEnPantalla("Pass. Incorrecto");
          delay(2000);
          lcd.clear();
          
          indice = 0;
          ActivarAlarma();
          }
      }
}
 
  void CambiarContrasena(){
        cursor = 0;
        lcd.clear();
        while(indice!=6){
          ImprimirEnPantalla("Actual Pass.:");
          tecla = teclado.getKey();
          if(tecla == 'C'){
            loop();
            }
          if(tecla){
            lcd.setCursor(cursor,1);
            clave[indice] = tecla;
            lcd.print(tecla);
            indice++;
            cursor++;          
            }
        }
         if (indice == 6){
          if(!strcmp(clave , clave_default)){
            lcd.clear();
            ImprimirEnPantalla("Pass. Correcto");
            delay(2000);
            cursor = 0;
            lcd.clear();
            indice = 0;
            
            while(indice!=6){
              ImprimirEnPantalla("Nuevo Pass.:");
              tecla = teclado.getKey();
              if(tecla){
              lcd.setCursor(cursor,1);
              clave_default[indice] = tecla;
              lcd.print(tecla);
              indice++;
              cursor++;
              }
            }
            
            ImprimirEnPantalla("Cambio Correcto");
              stringStart = 0;
              stringStop = 16;
              contador = 0;
              delay(2000);
              lcd.clear();
              indice = 0;
          }
          else{
            ImprimirEnPantalla("Pass Incorrecto");
            indice = 0;
            cursor = 0;
            delay(2000);
            CambiarContrasena();
            }
        }
  }
void OnOffLed(){
  if (backLight == true){
    backLight = false;
    lcd.setBacklightPin(3 , NEGATIVE);
    }
  else{
    backLight = true;
    lcd.setBacklightPin(3 , POSITIVE);
  }
}
 
 void Estado(){
    if(estadoAlarma == true){
      ImprimirEnPantalla(men1);
    }
    else{
       ImprimirEnPantalla(men2);
    }
 }
 
 void OnOffBocina(){
  if(estadoBocina == false){
    estadoBocina = true;
    }
  else{
    estadoBocina = false;
    digitalWrite(RELE , HIGH);
    }
  if (estadoBocina == true){
    digitalWrite(RELE , LOW);
    }
 }
 
 void Sensor(){
  if(estadoAlarma == true){
    estadoPIR = digitalRead(PIR);
    
    if(estadoPIR == HIGH){
      Serial.println("Sonando Alarma");
      if(estadoBocina == false){
        OnOffBocina();
        }
      delay(500);
      }
    }
  }
 void menu(){
  tecla = teclado.getKey();
  if(tecla){
    if (tecla == 'A'){
      ActivarAlarma();    
    }
    if (tecla == 'B'){
      CambiarContrasena();    
    }
    if (tecla == 'D'){
      OnOffLed();    
    }
  
  }
 }

void loop() {
  
  menu();
  Sensor();
  Estado();
}
 
   
  
  

 
