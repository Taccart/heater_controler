
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <DHT22.h>

#include <TickTwo.h>
#include <TimeOut.h> 

#define PIN_DHT22                D0           
#define PIN_RELAY_HEAT           D4
#define PIN_RELAY_BLOW           D3

#define BLOWING_TIME_MS          900000
#define BACKLIGHT_DELAY_MS       5000
#define REFRESH_LCD_MS           2000
#define REFRESH_BLOWING_STATE_MS 1000
#define REFRESH_HEATING_STATE_MS 1000
#define REFRESH_MEASURES_MS      5000


#define NO_COLOR                 0x0 // none
#define NORMAL_COLOR             0x7 // white
#define FREEZING_COLOR           0x4 // blue
#define HEATING_COLOR            0x3 // yellow
#define BLOWING_COLOR            0x1 // red


#define HEATING_MARGIN_CELCIUS   1
#define MAX_TEMP                 25
#define MIN_TEMP                 0
#define IS_DEBUG                 true

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

DHT22 dht22(PIN_DHT22); 
TimeOut timeout_blower;

int target_temp =MIN_TEMP;
int current_temp = 10;
int current_hygro =99;
bool is_heating = false;
bool is_blowing = false;
int blowing_expires=0 ;
int backlight_shutdown_sec =0;
int backlight_on = false;







void debug(String msg){
 #ifdef IS_DEBUG
  Serial.print("DEBUG ");
  Serial.println(msg);
 #endif
}
void resetFunc(){
    debug("setup_lcd()");
}
void setup_lcd(){
  debug("setup_lcd()");
  lcd.begin(16, 2);
  byte hourglassBottom_char[] = {
    B11111,
    B10001,
    B01010,
    B00100,
    B01110,
    B11111,
    B11111,
    B00000
  };
  byte hourglassTop_char[] = {
    B11111,
    B10001,
    B01010,
    B00100,
    B01110,
    B11111,
    B11111,
    B00000
  };

  byte bigC_char[8] = {
  B01110,
  B11111,
  B11011,
  B11000,
  B11011,
  B11111,
  B01110,
  B00000
  };
  byte bigV_char[8] = {
    B11011,
    B11011,
    B11011,
    B11011,
    B11111,
    B01110,
    B00100,
    B00000
  };
  byte hourglass_char[8] = {
    B11111,B10001,B01110,
    B00100,
    B01010,
    B10001,
    B11111,
    B00000
      };
  lcd.createChar(0x2, hourglass_char);
  lcd.createChar(0x3, bigC_char);
  lcd.createChar(0x4, bigV_char);
 
}
void action_set_heating(bool heating){
    debug("action_set_heating");
   
    is_heating=heating;
    if (is_heating){ 
      Serial.println("start heating");
      digitalWrite(PIN_RELAY_HEAT, HIGH);
    }
    else {
      Serial.println("stop heating");
      digitalWrite(PIN_RELAY_HEAT, LOW);  
      }
}
void action_stop_blowing(){
  debug("action_stop_blowing");
  action_set_blowing(false);
}
void action_set_blowing(bool blowing){
  debug("action_set_blowing");
 
    is_blowing=blowing;  
    if (is_blowing)
    { Serial.print("start blowing" );
      blowing_expires=millis()+BLOWING_TIME_MS;
      Serial.println(blowing_expires);
      
      timeout_blower.timeOut(BLOWING_TIME_MS, action_stop_blowing);
      digitalWrite(PIN_RELAY_BLOW, HIGH);
    }
    else  
    { Serial.println("stop blowing");
      timeout_blower.cancel();
       blowing_expires=0;
      digitalWrite(PIN_RELAY_BLOW, LOW);
     }
    update_lcd_blowing();

}

void update_lcd_backlight(){
  debug("update_lcd_backlight");
  if (backlight_shutdown_sec<millis()/1000) {    backlight_on=false;  }
  if (is_blowing){     lcd.setBacklight(BLOWING_COLOR);   }
  else if (is_heating) {lcd.setBacklight(HEATING_COLOR);   }
  else {
  if (backlight_on) 
    { if (current_temp<target_temp-5)      { lcd.setBacklight(FREEZING_COLOR); //it's freezing! 
    }
      else      {lcd.setBacklight(NORMAL_COLOR);      }
    }
    else    { lcd.setBacklight(NO_COLOR);    }
  }
}
void update_lcd_heating(){
  lcd.setCursor(14,0);
  if (is_heating)   {lcd.print("\x03");}
  else   {lcd.print("C");}
  
}
void update_lcd_target_temp(){
  lcd.setCursor(0,1);
  char *s =(char*)malloc(13 * sizeof(char));
  sprintf(s, " %4d\xDF", target_temp);
  lcd.print(s);free(s);
  
}

void update_lcd_measures(){
  debug("update_lcd_measures");
    //  line1 : temperature
    lcd.setCursor(0,0);
    lcd.print(" ");
  
    char *s =(char*)malloc(13 * sizeof(char));
    sprintf(s, "%+4d\xDF", current_temp);
    lcd.print(s);
    //  line1 : hygrometry
    lcd.setCursor(7,0);
    sprintf(s, "%2d%%h", current_hygro);
    lcd.print(s);
    free(s);
    
}
void update_lcd_blowing(){
  debug("update_lcd_blowing");
  lcd.setCursor(11,1);
 
  if (is_blowing) {
    Serial.println( blowing_expires-millis());
    char *s =(char*)malloc(13 * sizeof(char));
    sprintf(s, "%2dmin", 1+(blowing_expires-millis())/1000/60);
    lcd.print(s);free(s);
  }
  else 
  { lcd.print("       ");
  }
   lcd.setCursor(15,0);
  if (is_blowing) {lcd.print("\x04");}  else   {lcd.print("V");}
}
void update_lcd_all() {
  debug("update_lcd_all");
  update_lcd_measures();
  update_lcd_target_temp();
  update_lcd_backlight();  
  update_lcd_blowing();
  update_lcd_heating();
}

void increment_target_temp(int increment){
  debug("increment_target_temp");
  if (increment <0 && target_temp> MIN_TEMP) {target_temp=target_temp+increment;}
  if (increment >0 && target_temp< MAX_TEMP) {target_temp=target_temp+increment;}
  update_lcd_target_temp();
  Serial.println (target_temp);

    
}
void update_measures_from_DHT(){
  debug("update_measures_from_DHT");
  current_temp = dht22.getTemperature();
  current_hygro = dht22.getHumidity();
}

void update_heating_state(){
   debug("update_heating_state()");
   Serial.println(target_temp);
  Serial.println(current_temp);

  if (target_temp ==0)
    { action_set_heating(false);} 
  else if ( target_temp > current_temp +HEATING_MARGIN_CELCIUS ) 
    { action_set_heating(true);}
  if (target_temp < current_temp-HEATING_MARGIN_CELCIUS ) 
    { action_set_heating(false);} 
  update_lcd_heating();
}
void handleButtons(){
   
    uint8_t buttons = lcd.readButtons();
    if (buttons) {
       debug("handleButtons - pressed");
      backlight_on=true;
    
      if (buttons & BUTTON_RIGHT & BUTTON_LEFT) { 
        debug("BUTTON_RIGHT & BUTTON)LEFT");
        resetFunc();  }
      else if (buttons & BUTTON_UP) { 
        debug("BUTTON_UP");
        increment_target_temp(+1); }
      else if (buttons & BUTTON_DOWN) {
        debug("BUTTON_DOWN");
        increment_target_temp(-1); }
      else if (buttons & BUTTON_LEFT) { 
        debug("BUTTON_LEFT");
        action_set_blowing(false) ;}
      else if (buttons & BUTTON_RIGHT) { 
        debug("BUTTON_RIGHT");
        action_set_blowing(true);   }
      else if (buttons & BUTTON_SELECT) {
        debug("BUTTON_SELECT");
        backlight_shutdown_sec= millis()+BACKLIGHT_DELAY_MS;
        
        
        }
      delay(5);
  }
  else {
     //debug("handleButtons - none");
  }
}


TickTwo ticker_UpdateLCD(update_lcd_all, REFRESH_LCD_MS , 0, MILLIS);
TickTwo ticker_UpdateMeasures(update_measures_from_DHT, REFRESH_MEASURES_MS , 0, MILLIS);
TickTwo ticker_UpdateHeatingState(update_heating_state, REFRESH_HEATING_STATE_MS , 0, MILLIS);

void setup() {
  Serial.begin(115200);
  delay (500);
  debug("Setup()");
  setup_lcd();
  pinMode( PIN_RELAY_HEAT, OUTPUT);
  pinMode( PIN_RELAY_BLOW, OUTPUT);
  ticker_UpdateLCD.start();
  ticker_UpdateMeasures.start();
  ticker_UpdateHeatingState.start();

pinMode( D3, OUTPUT);
pinMode( D4, OUTPUT);
pinMode( RX, OUTPUT);
pinMode( TX, OUTPUT);

}


void loop() { 
  digitalWrite(D3, HIGH); delay(100);
  digitalWrite(D4, HIGH); delay(100);
  digitalWrite(RX, HIGH); delay(100);
  digitalWrite(TX, HIGH); delay(100);
 
  digitalWrite(D3, LOW);  delay(100);
  digitalWrite(D4, LOW);  delay(100);
  digitalWrite(RX, LOW);  delay(100);
  digitalWrite(TX, LOW);  delay(100);
  
//  ticker_UpdateLCD.update();
//  ticker_UpdateMeasures.update();
//  ticker_UpdateHeatingState.update();
//  timeout_blower.handler();
//  handleButtons();
}
