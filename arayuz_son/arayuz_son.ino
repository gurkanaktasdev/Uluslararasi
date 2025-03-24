int pinC = 3;         // clock pin              
int pinD = 2;        // data pin
int pinG = 4;       // enkoderin button pini                                                // şimdilik bir problem yok geliştirilmeyi bekliyor.
int enkodersayim = 0;
int pinDSon;
int dDeger;
bool saatYonu;   // enkoderın doğru çalışıp çalışmadığını kontrol etmek için
uint32_t i = 0;   // kontrol amaçlı 


#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

bool buttonPressed = false;
int personCounter=0;
int personDamaged=0,personDead=0;

 void setup() { 
  
   pinMode(pinC,INPUT);
   pinMode(pinD,INPUT);
   pinMode(pinG,INPUT);   
   Serial.begin (9600);  
   lcd.begin();
 } 

void loop() {  

   btnCheck();
   rotary();
 
}

void rotary()
{    
   dDeger = digitalRead(pinD);
   if (dDeger != pinDSon){ 
     if (digitalRead(pinC) != dDeger) {  
      if(enkodersayim==0)
      {
        enkodersayim = 4;
      }
       enkodersayim --;
       saatYonu = false;
        Serial.println(enkodersayim);
     } else {
       saatYonu = true;
       enkodersayim++; 
        Serial.println(enkodersayim);
     }
     enkodersayim = (enkodersayim %3)+1;    
     lcd.clear();
     userInterface();
     lcd.setCursor(0,enkodersayim);
     lcd.print(">");
   } 
   pinDSon = dDeger;
}
void btnCheck()   // buttonun basılıp basılmadığını flag yardımı ile kontrol eder.
{
  if (digitalRead(pinG) == LOW && !buttonPressed) {   // bu satırda butona basıldığını anlıyoruz.
        i++;
        buttonPressed = true;             
        Serial.println(i);
        Serial.println(enkodersayim);
       if(enkodersayim==1)
       {
         rotarySayici("Yarali Sayisi");
       }
       if(enkodersayim==2)
       {
         rotarySayici("Olu Sayisi");
       }
       if(enkodersayim == 3)
       {
        Serial.print("Yarali Sayisi :");
        Serial.println(personDamaged);
        Serial.print("Olu Sayisi :");
        Serial.println(personDead);
        if(personDamaged == 0 && personDead == 0)
        {
           lcd.clear();
          
           for (int k=0; k < 5; k++ ) {
              warningScreen();
              lcd.setCursor(9, 3);
              lcd.print(5-k);
              delay(1000);
              lcd.clear();
           }
           userInterface();
        }
        else
        {
            successScren();
        }
       }
    }
    if (digitalRead(pinG) == HIGH) { 
        
        buttonPressed = false; 
        
    } 
}
void userInterface()
{ 
  
  lcd.setCursor(1,0);
  lcd.print("--SEMA ILK YARDIM--");
  lcd.setCursor(1,1);
  lcd.print("Yarali Sayisi:");
  lcd.setCursor(15,1);
  lcd.print(personDamaged);
  lcd.setCursor(1,2);
  lcd.print("Olu Sayisi   :");
  lcd.setCursor(15,2);
  lcd.print(personDead);
  lcd.setCursor(13,3);
  lcd.print("Gonder");
}

void rotarySayici(String mesaj) // kişi sayısını bulmamıza yardımcı olacak.
{  altMenu(mesaj);
   delay(200);
   while(digitalRead(pinG)==LOW)
   {
    dDeger = digitalRead(pinD);
    if (dDeger != pinDSon){ 
      if (digitalRead(pinC) != dDeger) {  
       personCounter--;
     } else { 
       personCounter++;
     }
     lcd.clear();
     altMenu(mesaj);
     lcd.setCursor(9,2);
     lcd.print(personCounter);
   } 
   pinDSon = dDeger;
   }
   if(mesaj == "Yarali Sayisi")
   {
    personDamaged = personCounter;  // daha sonro veriyi LoRa ile iletmek için değer tutuyorum.
   }
   if(mesaj == "Olu Sayisi")
   {
    personDead = personCounter;  // daha sonro veriyi LoRa ile iletmek için değer tutuyorum.
   }
   personCounter = 0;
   delay(200);
   lcd.clear();
   userInterface();
}
void altMenu(String mesaj)
{
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print(mesaj);
  lcd.setCursor(1,1);
  lcd.print("-------------------");
}
void warningScreen()
{
  lcd.setCursor(7,0);
  lcd.print("UYARI");
  lcd.setCursor(3,1);
  lcd.print("Olu ve Yarali ");
  lcd.setCursor(1,2);
  lcd.print("Alanini Doldurunuz");
}
void successScren()
{
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("Mesajiniz Alindi");
  delay(5000);
}

