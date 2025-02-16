#include <U8glib.h>
#include <Stepper.h>

bool vilken = true;
bool timme = true;
// boolians för knapptryckning

int spelare=3;
int kort=1;
int kort_kvar=0;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
// inisialiserar skärmen

void setup() {
  pinMode(7, OUTPUT);
  pinMode(6, INPUT);
  digitalWrite(7, HIGH);
  Serial.begin(9600);
}

void loop(){

 for(int i=0; i<100; i++){
    buton_pres(6);
    kort_eller_spelare();
    text(0,15, "kort: "+ String(kort)+ "|pers:" + String(spelare));
    delay(100);
    //ger dig 10 sek att ställa in kort och antal spelare. Samt skriver ut det till skärmen
  }
  
  for(int i=0; i<spelare; i++){
    dela_spelare(spelare);
    mata_kort(kort);
    //roterar och mattar ut korten för x antal spelare
  }

  Stepper myStepper = Stepper(2038, 8, 10, 9, 11);
  myStepper.setSpeed(10);
	myStepper.step(-2038);
  // roterar tillbaka steppern för att sladdarna inte ska traslatill sig

}


void  buton_pres(int pin){
  int read = digitalRead(pin);

  if (read == 1 && timme == true) {
    vilken = !vilken;
    timme = !timme;
  }
  if (read == 0 && timme == false) {
    timme = !timme;
  }
  // kod för att knappen inte ska bounca när man håller inne den
}

void kort_eller_spelare(){
  if (vilken==true){
    spelare=potensometer(A0);
  }
  if (vilken==false){
    kort=potensometer(A0);
  }
  //växlar mellan att ändra antal spelare/kort
}

int potensometer(int pin){
  return map(analogRead(pin), 0,1021,1,10);
  //mapar om potenisometern till 1-10
}

void dela_spelare(int n_spelare){
  float grader=360/n_spelare;
  int stepsPerRevolution=(grader/360)*2038;
  Serial.println(stepsPerRevolution);
  Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
  //Gör om antal spelare till grader/kuggar på stepermotorn
  
  myStepper.setSpeed(10);
	myStepper.step(stepsPerRevolution);
  //roterar stepermotorn
}

void mata_kort(int n_kort){
  for(int i=0; i<n_kort; i++){
    digitalWrite(7, LOW);
    delay(int(180 * pow(2.71, -0.012 * kort_kvar)));
    // exponensialfunktion som kompenserar för att motorn behöver vara på kortare tid när det är färre kort kvar
    //Startar på delay(180) slutar på delay(96)
    kort_kvar++;
    digitalWrite(7, HIGH);
    delay(300);
  }
}

void text(int x, int y, String ord) {
  //skriver ut en string vid p(x,y) på skärmen
  u8g.firstPage();
  do {
     u8g.setFont(u8g_font_unifont);
     u8g.drawStr(x, y, ord.c_str());
  } while (u8g.nextPage());
}