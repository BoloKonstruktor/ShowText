#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <ShowText.h>


namespace LCD {
  const uint8_t ADDR = 0x27;
  const uint8_t COLS = 16;
  const uint8_t ROWS = 2; 
};


LiquidCrystal_PCF8574 lcd( LCD::ADDR );
ShowText show_text1;
ShowText show_text2;


void show1( const char* );
void show2( const char* );
void reload( void );

const char* text[] = {
  "To jest napis testowy, który nie mieści się w linii wyświetlacza",
  "Ale dzięki fenomenalnej bibilotece 'ShowText', zostanie on wyświetlony w całości!" 
};

bool idx = 0;

void setup() {
  Serial.begin( 115200 );
  Wire.begin();
  Wire.beginTransmission( LCD::ADDR );
  int error = Wire.endTransmission();
  
    if( error == 0 ) {
      lcd.begin( LCD::COLS, LCD::ROWS );
      lcd.setBacklight( 255 );
      lcd.home();
      lcd.clear();
      lcd.print( F("Init...") );
    } else {
      Serial.println("[ERROR] LCD not found.");
    }

  //Inicjalizacja
  show_text1.begin( 
    LCD::COLS /*Maksymalna ilość znaków*/, 
    0 /*Pozycja początkowa - wartość domyślna = 0*/
  );
  show_text2.begin( LCD::COLS );

  //Rejestracja funkcji wyświetlającej tekst w linii pierwszej.
  show_text1.registerShowCallback( show1 );
  
  //Rejestracja funkcji wyświetlającej tekst w linii drugiej.
  show_text2.registerShowCallback( show2 ); 
  
  //Rejestracja funkcji wywoływanej po wyświetleniu całego tekstu.
  show_text2.registerEndScrollCallback( reload );
  
  lcd.clear();
  lcd.setCursor( show_text1.getX(), 0 );
  //Ustawienie i zwrócenie krótkiego tekstu z zastosowaniem
  //opcji wyrównania (align, domyślnie ALIGN::CENTER) oraz znaku
  //wypełniającego pustą przestrzeń (domyślnie spacja).
  lcd.print( show_text1.show( " ShowText ", ALIGN::CENTER, "-" ) );
  delay( 2000 );
  lcd.clear();

  //Ustawienie krótkiego tekstu migającego w linii pierwszej.
  show_text1.set( "TEST" );
  show_text1.setBlink(
    true, /*Włączenie migania*/
    500 /*Szybkość migania*/
  );

  //Ustawienie długiego tekstu do wyświetlenia w linii drugiej.
  show_text2.set( text[idx] );
}

void loop() {
  //Wywołanie funkcji wyświetlającej tekst
  show_text1.show();
  show_text2.show();

    if( Serial.available() ){
      String str = Serial.readStringUntil( '\n' );
      int eq = str.indexOf( "=" );

        if( eq > -1 ){
          String cmd = str.substring( 0, eq );
          String value = str.substring( eq+1 );
          cmd.trim();
          cmd.toLowerCase();
          value.trim();

            if( cmd == "blink" ){
              bool blink = atoi( value.c_str() );
              show_text1.setBlink( blink );
            }

            if( cmd == "text" ){
              show_text1.set( value.c_str() );
            }
        }
    }
}

//Wyświetlanie tekstu
void show1( const char* str ){
  lcd.setCursor( show_text1.getX(), 0 );
  lcd.print( str );
}

void show2( const char* str ){
  lcd.setCursor( show_text2.getX(), 1 );
  lcd.print( str );
}

void reload( void ){
  idx ^= 1;
  show_text2.set( text[idx] );

    if( idx ){
      //Zmiana parametrów wyświetlania
      //Argument=true - Wypełnienie linii znakiem ustowionym w setSpace (domyślnie spacja)
      //na początku tekstu. Domyślnie argument=false.
      show_text2.setSpacesOnStart( false );

      //Argument=true - Wypełnienie linii znakiem ustowionym w setSpace (domyślnie spacja)
      //na końcu tekstu. Domyślnie argument=false.
      show_text2.setSpacesOnEnd( true );

      //Opóźnienie przesuwania tekstu na początku (domyślnie 3).
      show_text2.setDelayOnStart( 1 );

      //Opóźnienie powtórnego przesuwania, po przesunięciu całego tekstu (domyślnie 3).
      show_text2.setDelayOnEnd( 5 );

      //Szybkość przesuwania tekstu (domyślnie 500);
      show_text2.setScrollSpeed( 300 );
    }
}
