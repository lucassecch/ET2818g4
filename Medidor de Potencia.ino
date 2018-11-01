#include <LiquidCrystal.h>

// Sensibilidad del sensor en V/A

float SENSIBILITY = 0.066;                    //Modelo 30A
int SAMPLESNUMBER = 100;                      //cantidad de muestras    


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int     cont = 0;       //mantiene cuenta de cuantas personas hay en la casa
int     select = 0;     //Voltage = 0;
                        //Current = 1;
                        //Power   = 2;

#define entran 7
#define salen 8
#define seleccion 9
#define luces 10

void setup() 
{
   Serial.begin(9600);
   lcd.begin(16, 2);
   pinMode(entran, INPUT);
   pinMode(salen, INPUT);
   pinMode(seleccion, INPUT);
   pinMode(luces, OUTPUT);
}
 
void printMeasure(String prefix, float value, String postfix)
{
   Serial.print(prefix);
   Serial.print(value, 3);
   Serial.println(postfix);
}
 
void loop()
{
  
//----------calculos de tension, corriente y potencia----------------------------------------------
   
   float  current = getCorriente(SAMPLESNUMBER);        //calculo corriente
   float  currentRMS = 0.707 * current;                 //calculo Irms
   float  power = 230.0 * currentRMS;                   //calculo potencia
   float  tension = getTension(SAMPLESNUMBER);          //calculo de tension

   
    
   printMeasure("Intensidad: ", current, "A ,");
   printMeasure("Irms: ", currentRMS, "A ,");
   printMeasure("Potencia: ", power, "W");
   delay(1000);

//-----------------VISUALIZACION EN LCD-------------------------------------------------------------

   if(seleccion == HIGH)              //pulador de seleccion de visualización (tension, corriente o potencia)
   {
      select++;
      if(select >= 4)
      {
        select = 0;
      }
   }


   if(select == 0)            //para visualizar tension en display
   {
      lcd.setCursor(0,0);     
      lcd.print("TENSION");
      lcd.setCursor(0, 1);
      lcd.print(currentRMS, "A ,");
   }

    if(select == 1)                   //para visualizar corriente en display
   {
      lcd.setCursor(0,0);     
      lcd.print("CORRIENTE");         //escribo corriente en la linea de arriba
      lcd.setCursor(0, 1);
      lcd.print(tension, "V ,");      //muestro el valor de corriente en el display 
   }

    if(select == 2)                   //para visualizar potencia en display
   {
      lcd.setCursor(0,0);     
      lcd.print("POTENCIA");          //escribo potencia en la linea de arriba
      lcd.setCursor(0, 1);
      lcd.print(power, "W");          //muestro el valor de potencia en el display
   }
   

//----------------ENCENDIDO DE LUCES DE LA CASA----------------------------------------------

   if((entran == 1)&&(salen == 0))          //registra si entra alguien
   {
      cont++;
      delay(1000);
   }

   if((salen == 1)&&(entran == 0))          //registra si sale alguien
   {
      cont--;
      delay(1000);
   }

   if(cont >= 1)                            
   {
      digitalWrite(luces, HIGH);            //enciende luces si hay personas en la casa
   }
   else                                     
   {
      digitalWrite(luces, LOW);             //las apaga si no hay nadie
   }
}


//----------SENSORES DE TENSION Y CORRIENTE----------------------------------------------------
 
float getCorriente(int samplesNumber)       //lectura del pin A0 (corriente)
{
   float voltage;
   float corrienteSum = 0;
   for (int i = 0; i < samplesNumber; i++)
   {
      voltage = analogRead(A0) * 5.0 / 1023.0;
      corrienteSum += (voltage - 2.5) / SENSIBILITY;
   }
   return(corrienteSum / samplesNumber);
}

float getTension(int samplesNumber)         //lectura del pin A1 (tension)
{
   float voltage;
   float tensionSum = 0;
   for (int i = 0; i < samplesNumber; i++)
   {
      voltage = analogRead(A1) * 220 / 1023.0;
      tensionSum += voltage;
   }
   return(tensionSum / samplesNumber);
}


 
