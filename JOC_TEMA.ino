#include "LedControl.h"
#include <binary.h>
#include <string.h>

LedControl lc = LedControl(12,11,10,1);
const int SW_pin = 2;
const int X_pin = 0;
const int Y_pin = 1;
const int FAILED_PIN = 13;
const int LVL_PIN = 3;
const int COMPLETED_PIN = 7;
byte obj[8]; //randurile
int i = 0; //numara pasii functiei loop
int j = 0; 
int p = 3; //pozitia mijlocului navei
int lvl = 5; //parametru ce ajusteaza viteza jocului

void generate(int row)
{
  if (obj[row] == B00000000) //verificam daca randul "row" nu are deja un obstacol
    obj[row] = B00000001; //randul cu indicele "row" primeste un obstacol in capat
  else generate(random(0,7)); //daca are obstacol incercam alt rand pana gasim unul gol (exista fix 3 randuri cu obstacole concomitent)
  lc.setRow(0, row, obj[row]);
}

void move_cursor(const char dir[3] )
{
  //functia este apelata la fiecare iteratie chiar daca nu mutam nava deoarece vrem sa ramana pe aceeasi coloana indiferent de cum se muta liniile
  lc.setLed(0, p-1, 0, false);
  lc.setLed(0, p, 0, false);
  lc.setLed(0, p+1, 0, false);
  lc.setLed(0, p, 1, false); 
  //am sters nava din pozitia precedenta
  if (strcmp(dir, "up") == 0 && p > 1) p--; //verificam daca avem loc sa mutam nava in sus
  if (strcmp(dir, "dw") == 0 && p < 6) p++; //verificam daca avem loc sa mutam nava in jos
  lc.setLed(0, p-1, 0, true);
  lc.setLed(0, p, 0, true);
  lc.setLed(0, p+1, 0, true);
  lc.setLed(0, p, 1, true); 
  //nava a fost reafisata
}

int check ()
{
  if ((obj[p] & B11000000) != 0) return 0;
  if ((obj[p-1] & B10000000) != 0) return 0;
  if ((obj[p+1] & B10000000) != 0) return 0;
  //daca una dintre operatiile din if-urile de mai sus (& logic) este diferita de 0 inseamna ca exista coliziune intre nava si obstacol
  return 1;
}

void software_reboot()
{
  asm volatile ("jmp 0");
}

void flash_LED(int LED)
{
  digitalWrite(LED, LOW);
  delay(400);
  digitalWrite(LED, HIGH);
  delay(400);
}

void read_joystick()
{
  if (analogRead(Y_pin) > 700)  move_cursor("dw"); //muta nava in jos
  if (analogRead(Y_pin) < 300)  move_cursor("up"); //muta nava in sus
  if (analogRead(Y_pin) >= 300 && analogRead(Y_pin) <=700) move_cursor("st"); //reafiseaza nava la aceeasi pozitie
}

void end_check()
{
  if (i >= 200 && lvl != 1)
  {
    lvl--;
    i = 0;
    flash_LED(lvl + 2);
    flash_LED(lvl + 2);
    flash_LED(lvl + 2);
    flash_LED(lvl + 2);
    for (j = 0; j <= 7; j++) //sterge toate obiectele pentru a pregati urmatorul nivel
      obj[j] = B00000000; 
    delay(1000);
  }
  if (i >= 200 && lvl == 1)
  {
    digitalWrite(COMPLETED_PIN, HIGH); 
    while (digitalRead(SW_pin) == HIGH); //nu face nimic pana cand SW_pin nu trece in starea LOW (butonul este apasat)
    software_reboot();
  }
}

void generate_check()
{
  if (i % (lvl*3) == 0) 
    generate(random(0, 7)); //genereaza un nou obstacol pe un rand aleator intre 0 si 7    
}

void routine_check()
{
  if (i%lvl == 0)
  {
    for (j = 0; j <= 7; j++)
    {
      obj[j] = obj[j] << 1; //muta randul (byte-ul) j la stanga cu un bit
      lc.setRow(0, j, obj[j]); //afiseaza noul byte pe ecran
    }
    if (check() == 0) 
    {
      digitalWrite(FAILED_PIN, HIGH);
      lc.shutdown(0, true);
      while (digitalRead(SW_pin) == HIGH);
      software_reboot();   
    }
  }
}

void setup()
{
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  pinMode(SW_pin, INPUT);
  pinMode(SW_pin, INPUT_PULLUP);
  pinMode(LVL_PIN, OUTPUT);
  pinMode(LVL_PIN + 1, OUTPUT);
  pinMode(LVL_PIN + 2, OUTPUT);
  pinMode(LVL_PIN + 3, OUTPUT);
  pinMode(COMPLETED_PIN, OUTPUT);
  pinMode(FAILED_PIN, OUTPUT);
  
  flash_LED(LVL_PIN + 3);
  flash_LED(LVL_PIN + 2);
  flash_LED(LVL_PIN + 1);
  flash_LED(LVL_PIN);
  flash_LED(COMPLETED_PIN);
  flash_LED(FAILED_PIN);
  
  while (digitalRead(SW_pin) == HIGH); //asteapta apasarea butonului de pe joystick (trecerea in starea low)
  
  digitalWrite(SW_pin, HIGH);
  digitalWrite(COMPLETED_PIN, LOW);
  digitalWrite(FAILED_PIN, LOW);
  digitalWrite(LVL_PIN, LOW);
  digitalWrite(LVL_PIN + 1, LOW);
  digitalWrite(LVL_PIN + 2, LOW);
  digitalWrite(LVL_PIN + 3, LOW);
  
  p = 3;
  lvl = 5;
  for (j = 0; j <= 7; j++)
    obj[j] = B00000000; 
}

void loop()
{ 
  generate_check(); //verifica daca e timpul sa genereze un nou obstacol
  routine_check();  //muta obstacolele si verifica daca s-a produs o coliziune
  read_joystick(); //citeste joystickul si muta nava in functie de inputul acestuia
  
  delay(100);
  i++;
  
  end_check(); //verifica daca s-a terminat nivelul/jocul
}
