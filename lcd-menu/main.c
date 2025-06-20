#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "symbole.h"
#include "ds18b20_lib.h"
#include <avr/interrupt.h>

#define CLK PC2
#define DT  PC3
#define SW  PC4
#define RS PD7
#define E PD2
#define D4 PD3
#define D5 PD4
#define D6 PD5
#define D7 PD6

// Temperatura
float temperatura_OW = 0;
volatile uint16_t seconds = 0;//do timera
volatile uint8_t running = 0;
volatile uint8_t position = 0; //1 = IMIE NAZWISKO, 2 = WROC
volatile uint8_t ekran = 0; //0 -Dane, 1 -Podekran1 (ImieNazwisko, Indeks, Wroc),
//2- Podekran1.1 (Imie i Nazwisko)

void select_ekran(void){
  switch(ekran){
    case 0 : ekran1(); break;
    case 1 : podekran1(); break;
    case 2 : imie_nazwisko(); break;
    case 3 : ekran_temperatura(); break;
    case 4: indeks(); break;
    case 5: timer_ekran(); break;
  }
}

void update_ekran1(void){
  switch (position){
    case 0: podekran1(); break;
    case 1: podekran_indeks(); break;
    case 2: wroc(); break;
    default: podekran1(); break;
  }
}

void update_ekran(void) {
  switch (position) {
    case 0: imie_nazwisko(); break;
    case 1: wroc(); break;
    default: imie_nazwisko(); break;
  }
}

void update_ekran0(void){
  switch (position) {
    case 0: ekran1(); break;
    case 1: timer(); break;
    case 2: temperatura_OW_ekran(); break;
  }
}
void update_ekran3(void){
  switch (position){
    case 0: ekran_temperatura(); break;
    case 1: wroc(); break;
  }
}

void update_ekran4(void){
  switch(position){
    case 0: indeks(); break;
    case 1: wroc(); break;
  }
}

void update_ekran5(void){
  switch(position){
    case 0: timer_ekran(); break;
    case 1: wroc(); break;
  }
}

void ekran1(void){
  //wyswietl Dane
  lcd_clear();
  _delay_ms(2);
  symbol(strzalka);
  symbol(D);
  symbol(a);
  symbol(n);
  symbol(e);
}

void podekran_indeks(void){
  lcd_clear();
  _delay_ms(2);
  symbol(strzalka);
  symbol(I);
  symbol(n);
  symbol(d);
  symbol(e);
  symbol(k);
  symbol(s);
}

void podekran1(void){
  lcd_clear();
  _delay_ms(2);
  symbol(strzalka);
  symbol(I);
  symbol(m);
  symbol(i);
  symbol(e);
  
  nowalinia();
  
  symbol(N);
  symbol(a);
  symbol(z);
  symbol(w);
  symbol(i);
  symbol(s);
  symbol(k);
  symbol(o);
}

void timer(void){
  lcd_clear();
  _delay_ms(2);
  symbol(strzalka);
  symbol(T);
  symbol(i);
  symbol(m);
  symbol(e);
  symbol(r);
}

void timer_ekran(void) {
  lcd_clear();
  symbol(T);
  symbol(i);
  symbol(m);
  symbol(e);
  symbol(r);
  nowalinia();

  uint8_t mins = seconds / 60;
  uint8_t secs = seconds % 60;

  symbol('0' + mins / 10);
  symbol('0' + mins % 10);
  symbol(':');
  symbol('0' + secs / 10);
  symbol('0' + secs % 10);
}

void temperatura_OW_ekran(void){
  lcd_clear();
  _delay_ms(2);
  symbol(strzalka);
  symbol(T);
  symbol(e);
  symbol(m);
  symbol(p);
  symbol(e);
  symbol(r);
  symbol(a);
  symbol(t);
  symbol(u);
  symbol(r);
  symbol(a);
  nowalinia();
  symbol(O);
  symbol(n);
  symbol(e);
  symbol(W);
  symbol(i);
  symbol(r);
  symbol(e);
}

void symbol(uint8_t symbol){
  PORTD |= (1 << RS);
  PORTD = (1 << E);
  PORTD = PORTD | ((1 << RS) | ((symbol & 0xF0) >> 1));
  _delay_ms(1);
  PORTD = PORTD & ~(1 << E);
  _delay_ms(1);
  PORTD |= (1 << RS);
  PORTD = (1 << E);
  PORTD = PORTD | ((1 << RS) | ((symbol & 0x0F) << 3));
  _delay_ms(1);
  PORTD = PORTD & ~(1 << E);
  _delay_ms(1);
  
}

void indeks(void){
  //PORTC|=~(1<<LED_P);
  lcd_clear();
  symbol(l2);
  symbol(l5);
  symbol(l2);
  symbol(l6);
  symbol(l2);
  symbol(l7);
  
}

void nowalinia(void){
  //  RS na 0
  PORTD &=~(1<<RS);
  //  E
  PORTD |=(1<<PD2);
  //  PD7   PD6   PD5   PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  PORTD |= ((1<<PD6)|(1<<PD5));
  PORTD &=~(1<<PD2);
  
  PORTD |=(1<<PD2);
  //  PD7   PD6   PD5   PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  PORTD &=~(1<<PD2);
  _delay_us(100);
}

void imie_nazwisko(void){
  lcd_clear();
  symbol(M);
  symbol(a);
  symbol(r);
  symbol(t);
  symbol(a);
  
  nowalinia();
  
  symbol(O);
  symbol(s);
  symbol(t);
  symbol(r);
  symbol(o);
  symbol(w);
  symbol(s);
  symbol(k);
  symbol(a);
  
}

void wroc(void){
  lcd_clear();
  _delay_ms(2);
  symbol(strzalka);
  symbol(W);
  symbol(r);
  symbol(o);
  symbol(c);
}

void lcd_clear(void){
  PORTD &= ~(1<<RS);
  PORTD |= (1<<PD2);
  //  PD7   PD6   PD5   PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));

  PORTD &= ~(1<<PD2);
  PORTD |= (1<<PD2);
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  PORTD |=   (1<<PD3);
  PORTD &= ~(1<<PD2);
  PORTD |= (1<<RS);
  _delay_ms(100);
}

void lcd_init(void)
{
  
  DDRD |=   (1<<PD7); // ustawiamy kierunek linii podswietlenia LCD jako WYJSCIE
  PORTD |=  (1<<PD7); // zalaczamy podswietlenie LCD - stan wysoki
  //Ustawienie wszystkich 4 linii danych jako WYjScia
  //  PD7   PD6   PD5   PD4
  DDRD |= (1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3);
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  //  E   RW    RS
  DDRD |= (1<<PD2)|(1<<PD1)|(1<<RS);
  PORTD |= (1<<PD2)|(1<<PD1)|(1<<RS);

  _delay_ms(15);
  //  E   RW    RS
  PORTD &= ~((1<<PD2)|(1<<PD1)|(1<<RS));
  
  //  E
  PORTD |= (1<<PD2);
  //     PD5    PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  PORTD |=           (1<<PD4)|(1<<PD3); // tryb 8-bitowy
  //  E
  PORTD &= ~(1<<PD2);
  _delay_ms(5);

  //  E
  PORTD |= (1<<PD2);
  //     PD5    PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  PORTD |=                     (1<<PD4)|(1<<PD3); // tryb 8-bitowy
  PORTD &= ~(1<<PD2);
  _delay_us(100);

  //  E
  PORTD |= (1<<PD2);
  //     PD5    PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  PORTD |=           (1<<PD4)|(1<<PD3); // tryb 8-bitowy
  PORTD &= ~(1<<PD2);
  _delay_us(100);

  //  E
  PORTD |= (1<<PD2);
  //     PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  PORTD |=           (1<<PD4);  // tryb 4-bitowy
  //  RS
  PORTD &= ~(1<<PD2);
  _delay_us(100);
  
  
  // tryb 4-bitowy, 2 wiersze, znak 5x7
  //  E
  PORTD |= (1<<PD2);
  //  PD7   PD6   PD5   PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  PORTD |=          (1<<PD4);
  //  E
  PORTD &= ~(1<<PD2);
  //  E
  PORTD |= (1<<PD2);
  // PD7    PD6   PD5   PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  PORTD |=   (1<<PD6);
  //  E
  PORTD &= ~(1<<PD2);
  
  _delay_us(100);
  
  //  E
  PORTD |= (1<<PD2);
  //  PD7   PD6   PD5   PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  //  E
  PORTD &= ~(1<<PD2);
  //  E
  PORTD |= (1<<PD2);
  // PD7    PD6   PD5   PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  PORTD |=   (1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3);
  //  E
  PORTD &= ~(1<<PD2);
  
  _delay_us(100);

  //  E
  PORTD |= (1<<PD2);
  //  PD7   PD6   PD5   PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  //  E
  PORTD &= ~(1<<PD2);
  //  E
  PORTD |= (1<<PD2);
  // PD7    PD6   PD5   PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  PORTD |=      (1<<PD5)|(1<<PD4);
  //  E
  PORTD &= ~(1<<PD2);

  _delay_us(100);
  // kasowanie ekranu
  //  E
  PORTD |= (1<<PD2);
  //  PD7   PD6   PD5   PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  //  E
  PORTD &= ~(1<<PD2);
  //  E
  PORTD |= (1<<PD2);
  // PD7    PD6   PD5   PD4
  PORTD &= ~((1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3));
  PORTD |=   (1<<PD3);
  //  E
  PORTD &= ~(1<<PD2);
  _delay_ms(100);
}

int main(void)
{
  DDRC &= ~((1<<CLK) | (1<<DT) | (1<<SW));
  PORTC |= (1<<CLK) | (1<<DT) | (1<<SW); //inputy na enkoder
  
  DDRD = 0xFF;
  PORTD = 0x00;
  setup_onewire_pin(&DDRC, &PORTC, PC0);
  _delay_ms(50);
  lcd_init();
  select_ekran();

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT12);
  sei();
  TCCR1B |= (1 << WGM12);               //CTC
  OCR1A = 15624;                        //(16MHz /(1024 * 1 Hz))-1
  TIMSK1 |= (1 << OCIE1A);             
  TCCR1B |= (1 << CS12) | (1 << CS10); //prescaler 1024

  uint8_t last_state = (PINC & (1<<CLK)) >> CLK;

  while (1)
  {
    uint8_t clk_state = (PINC & (1<<CLK)) >> CLK;
    uint8_t dt_state = (PINC & (1<<DT)) >> DT;

    if (ekran == 0){//main ekran, dane temp timer
       if (clk_state != last_state && clk_state == 0) {//zbocze opadajace clk
        if (dt_state == clk_state) {
          if (position > 0) position--;//lewo
          } else {
          //ruch w lewo
          if (position < 2) position++;// praawo
        }
        _delay_ms(100);
        update_ekran0();
      }
    }
    if (ekran == 1) { //podekran wyboru imie/indeks/wroc
      if (clk_state != last_state && clk_state == 0) { 
        if (dt_state == clk_state) {
          if (position > 0) position--;
          } else {
          //ruch w lewo
          if (position < 2) position++;
        }
        _delay_ms(100);
        update_ekran1();
      }
    }
    
    if (ekran == 2){ //podekran Imie nazwisko
      if (clk_state != last_state && clk_state == 0) {
        if (dt_state == clk_state) {
          if (position > 0) position--;
          } else {
          //ruch w lewo
          if (position < 2) position++;
        }
        _delay_ms(100);
        update_ekran();
      }
    }

    if (ekran == 3){ //tempOW
      if (clk_state != last_state && clk_state == 0) {
        if (dt_state == clk_state) {
          if (position > 0) position--;
          } else {
          //ruch w lewo
          if (position < 2) position++;
        }
        _delay_ms(100);
        update_ekran3();
      }
    }
    if (ekran == 4){ //indeks
      if (clk_state != last_state && clk_state == 0) {
        if (dt_state == clk_state) {
          if (position > 0) position--;
          } else {
          if (position < 2) position++;
        }
        _delay_ms(100);
        update_ekran4();
      }
    }
    if (ekran == 5){ //timer
      if (clk_state != last_state && clk_state == 0) {
        if (dt_state == clk_state) {
          if (position > 0) position--;
          } else {
          if (position < 2) position++;
        }
        _delay_ms(100);
        update_ekran5();
      }
    }
      if (ekran == 5 && running) {
        timer_ekran();
        _delay_ms(500);
      }
        last_state = clk_state;
  }
}
ISR(TIMER1_COMPA_vect) {
  if (running) {
    seconds++;
  }
}

ISR(PCINT1_vect) {
  static uint8_t last_state = 1;

  uint8_t current_state = PINC & (1 << PINC4);
  if (!current_state && last_state) {
    _delay_ms(50);

    if (!(PINC & (1 << PINC4))) {

      if (ekran == 0 && position == 0) {
        ekran = 1;
        select_ekran();
        position = 0;
      }
      else if (ekran == 0 && position == 2) {
        ekran = 3;
        select_ekran();
        position = 0;
      }
      else if (ekran == 1 && position == 1) {
        ekran = 4;
        select_ekran();
        position = 0;
      }
      else if (ekran == 1 && position == 2) {
        ekran = 0;
        select_ekran();
        position = 0;
      }
      else if (ekran == 1) {
        ekran = 2;
        select_ekran();
        position = 0;
      }
      else if (ekran == 2 && position == 1) {
        ekran = 1;
        select_ekran();
        position = 0;
      }
      else if (ekran == 3 && position == 1) {
        ekran = 0;
        select_ekran();
        position = 2;
        update_ekran0();
      }
      else if (ekran == 4 && position == 1) {
        ekran = 1;
        select_ekran();
        position = 1;
        update_ekran1();
      }
      else if (ekran == 0 && position == 1){
        ekran = 5;
        select_ekran();
        position = 0;
        update_ekran5();
      }
      else if (ekran == 5 && position == 0) {
          running = !running;
       select_ekran();
      }
      else if (ekran == 5 && position == 1) {
        ekran = 0;
        position=1;
        select_ekran();
        update_ekran0();
      }
    }
  }

  last_state = current_state;
}
void ekran_temperatura() {
  lcd_clear();
  symbol(T);
  symbol(e);
  symbol(m);
  symbol(p);
  symbol(spacja);
  int16_t temperatura2 = get_temperature_reading(&DDRC, &PORTC, &PINC, PC0);
  int16_t temp_c = temperatura2 / 16;

  uint8_t dziesiatki = temp_c / 10;
  uint8_t jednosci = temp_c % 10;
  uint16_t temp_frac = ((temperatura2 & 0x0F) * 625) / 100;
  uint8_t dziesietne = (temp_frac / 10) % 10;
  uint8_t setne = temp_frac % 10;
  
  if (dziesiatki > 0) symbol('0' + dziesiatki);
  symbol('0' + jednosci);
  symbol('.');
  symbol('0' + dziesietne);
  symbol('0' + setne);
  symbol(stopnie);
  symbol(C);

}

