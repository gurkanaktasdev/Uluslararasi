int pinC = 3;         // clock pin              
int pinD = 2;        // data pin
int pinG = 4;       // enkoderin button pini                                                // şimdilik bir problem yok geliştirilmeyi bekliyor.
int enkodersayim = 0;
int enkoderTutucu = 0;  // sayfa sayfa geçişlerde işimize yarayacak değişken. 
int pinDSon;
int dDeger;


uint32_t i = 0;   // kontrol amaçlı 
uint16_t LoRa_Ip = 0001;
int personDead,personDamaged,personEmergency,personCounter; // LoRa ile yollayacağımız veriler 
bool buttonPressed = false;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  userInterface1();
  pinMode(pinC,INPUT);
  pinMode(pinD,INPUT);
  pinMode(pinG,INPUT);   
}

void loop() {
  // put your main code here, to run repeatedly:
  btnCheck();
  rotary();
}
void rotary()
{    
   dDeger = digitalRead(pinD);
   if (dDeger != pinDSon){ 
     if (digitalRead(pinC) != dDeger) {  
      if(enkodersayim==1)                               // buradaki sayım sistemi diğer ikisinden farklı
      {
        enkodersayim = 5;
      }
       enkodersayim --;
       
        Serial.println(enkodersayim);
     } else {
       if(enkodersayim==4)
      {
        enkodersayim = 0;
      }
       enkodersayim++; 
      Serial.println(enkodersayim);
     }
     
     
     Serial.print("sonenkoder :");
     Serial.println(enkodersayim);
     lcd.clear();

     if(enkodersayim != 4)      // bu kısım daha detaylı işllenecek 
     {
      userInterface1();
      lcd.setCursor(0,enkodersayim);
      lcd.print(">");
     }
     else
     {
      enkoderTutucu = enkodersayim;
      userInterface2();
      lcd.setCursor(0,enkodersayim-1);
      lcd.print(">");
     }
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
         rotarySayici();
       }
       if(enkodersayim==2)
       {
         rotarySayici();
       }
       if(enkodersayim==3)
       {
         rotarySayici();
       }
       if(enkodersayim == 4)
       {
         Serial.print("LoRa Ip:");
        Serial.println(LoRa_Ip);
        Serial.print("Yarali Sayisi :");
        Serial.println(personDamaged);
        Serial.print("Enkaz Altindaki Kişi Tahmini :");
        Serial.println(personDead);
        Serial.print("Aciliyet Durumu:");
        Serial.println(personEmergency);
       
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
           userInterface1();
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
void rotarySayici() // kişi sayısını bulmamıza yardımcı olacak.
{  if(enkodersayim == 1 )
    {
      altMenuYarali();
    }
    if(enkodersayim == 2)
    {
      altMenuEnkazA();
    }
    if(enkodersayim == 3)
    {
      altMenuAciliyet();
    }
  
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
            if(enkodersayim==1)
            {
              lcd.clear();
              altMenuYarali();
              lcd.setCursor(9,2);
              lcd.print(personCounter);
            }
            if(enkodersayim==2)
            {
              lcd.clear();
              altMenuEnkazA();
              lcd.setCursor(9,3);
              lcd.print(personCounter);
            }
            if(enkodersayim==3)
            {
              lcd.clear();
              altMenuAciliyet();
              lcd.setCursor(9,3);
              personCounter = personCounter % 6;
              if(personCounter<0)
              {
                personCounter=0;
              }
              lcd.print(personCounter);
            }
          } 
          pinDSon = dDeger;
   }
   if(enkodersayim==1)
   {
    personDamaged = personCounter;  // daha sonro veriyi LoRa ile iletmek için değer tutuyorum.
   }
   if(enkodersayim==2)
   {
    personDead = personCounter;  // daha sonro veriyi LoRa ile iletmek için değer tutuyorum.
   }
   if(enkodersayim==3)
   {
    personEmergency = personCounter;  // daha sonro veriyi LoRa ile iletmek için değer tutuyorum.
   }
   personCounter = 0;
   delay(200);
   lcd.clear();
   userInterface1();
   lcd.setCursor(0,enkodersayim);
   lcd.print(">");
}

void userInterface1()
{ 
  
  lcd.setCursor(1,0);
  lcd.print("--SEMA ILK YARDIM--");
  lcd.setCursor(1,1);
  lcd.print("Yarali Sayisi  :");
  lcd.setCursor(17,1);
  lcd.print(personDamaged);
  lcd.setCursor(1,2);
  lcd.print("Enkaz Altinda..:");
  lcd.setCursor(17,2);
  lcd.print(personDead);
  lcd.setCursor(1,3);
  lcd.print("Aciliyet Durumu:");
  lcd.setCursor(17, 3);
  lcd.print(personEmergency);
}
void userInterface2()
{ 
  lcd.setCursor(1,0);
  lcd.print("Yarali Sayisi  :");
  lcd.setCursor(17,0);
  lcd.print(personDamaged);
  lcd.setCursor(1,1);
  lcd.print("Enkaz Altinda..:");
  lcd.setCursor(17,1);
  lcd.print(personDead);
  lcd.setCursor(1,2);
  lcd.print("Aciliyet Durumu:");
  lcd.setCursor(17, 2);
  lcd.print(personEmergency);
  lcd.setCursor(13, 3);
  lcd.print("Gonder");
  
}
void altMenuYarali()
{
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Yarali Sayisi");
  lcd.setCursor(1,1);
  lcd.print("-------------------");
}
void altMenuEnkazA()
{
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Enkaz Altindaki");
  lcd.setCursor(0,1);
  lcd.print("Tahmini Kisi Sayisi");
  lcd.setCursor(0,2);
  lcd.print("-------------------");
}
void altMenuAciliyet()
{
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Aciliyet Durumu");
  lcd.setCursor(4,1);
  lcd.print("1-5 Arasinda");
  lcd.setCursor(0,2);
  lcd.print("-------------------");
}
void warningScreen()
{
  lcd.setCursor(7,0);
  lcd.print("UYARI");
  lcd.setCursor(3,1);
  lcd.print("Hicbir doldurma ");
  lcd.setCursor(1,2);
  lcd.print("Islemi Yapmadiniz ");
}
void successScren()
{
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("Mesajiniz Alindi");
  delay(2000);
  userInterface1();
}
