#include <U8glib.h>
#include <Stepper.h>
// biblotek

bool vilken = true;
bool timme = true;
// boolians för knapptryckning

int spelare=3;
int kort=1;
int kort_kvar=0;
// globala variablar

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
// inisialiserar skärmen

void setup() {
  // sätter pins som input o output
  pinMode(7, OUTPUT);
  pinMode(6, INPUT);
  digitalWrite(7, HIGH);// säts till high direkt för att motorn inte ska snura
}

void loop(){

  //ger dig 10 sek att ställa in kort och antal spelare. Samt skriver ut värdena till skärmen
 for(int i=0; i<100; i++){
    buton_pres(6); // kolar om du har tryckt på knappen
    kort_eller_spelare(); //se om den ska ändra antal kort eller spelare
    text(0,15, "kort: "+ String(kort)+ "|pers:" + String(spelare)); // skriver ut antal spelare och kort till lcd skärmen
    delay(100);
  }
  
  // for loop som matar ut kort till en spelare och roterar till nästa. kärs så många gånger det finns spelare
  for(int i=0; i<spelare; i++){
    mata_kort(kort); //delar ut x kort
    dela_spelare(spelare); // roterar x grader
  }

  Stepper myStepper = Stepper(2038, 8, 10, 9, 11);
  myStepper.setSpeed(10);
	myStepper.step(-2038);
  // roterar tillbaka steppern för att sladdarna inte ska traslatill sig
}

// kod som gör att knappen inte ska "bounca" när man håller inne den
void  buton_pres(int pin){
  int read = digitalRead(pin);
  if (read == 1 && timme == true) {
    vilken = !vilken;
    timme = !timme;
  }
  if (read == 0 && timme == false) {
    timme = !timme;
  }
}

//växlar mellan att ändra antal spelare/kort
void kort_eller_spelare(){
  if (vilken==true){
    spelare=potensometer(A0);
  }
  if (vilken==false){
    kort=potensometer(A0);
  }
}

//tar ett pin number som input (intedger) och mapar om signalen ochreturnerar en int från 1-10
int potensometer(int pin){
  return map(analogRead(pin), 0,1021,1,10);  //mapar om potenisometern från 0-1021 till 1-10
}

// Funktion som mattar ut x antal kort
void dela_spelare(int n_spelare){
  float grader=360/n_spelare; //räknar ut hur många grader det är emelan spelarna
  int stepsPerRevolution=(grader/360)*2038; //räknar ut hur många kugar det motsvarar på stepermotorn
  Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11); //inisialiserar steepermotorn o sätter max kugar som stepsPerRevolution
  
  myStepper.setSpeed(10); // sätter steperns hastighet till 10
	myStepper.step(stepsPerRevolution); //roterar så många kugar det är emelan 1 spelare
  //roterar stepermotorn
}

// funktion för att mata ut x antal kort
void mata_kort(int n_kort){
  for(int i=0; i<n_kort; i++){
    digitalWrite(7, LOW); // slår på motorn
    delay(int(180 * pow(2.71, -0.012 * kort_kvar)));
    // exponensialfunktionen kompenserar för att motorn behöver vara på kortare tid destå färre kort det är kvar
    //Startar på delay(180) slutar på delay(96)
    kort_kvar++;
    digitalWrite(7, HIGH); // stänger av motorn
    delay(300);
  }
}

//skriver ut en string vid p(x,y) på skärmen
void text(int x, int y, String ord) {
  u8g.firstPage();
  do {
     u8g.setFont(u8g_font_unifont);
     u8g.drawStr(x, y, ord.c_str());
  } while (u8g.nextPage());
}