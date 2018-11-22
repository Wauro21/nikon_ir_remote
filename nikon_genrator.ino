void setup() {
  //definir salida
  #include <util/delay.h>
  #define F_CPU 8000000
  pinMode(0, OUTPUT);
  TCCR0A  = _BV(COM0B1) | _BV(WGM01)  | _BV(WGM00);//ajustes para reloj 8 [MHz]
  TCCR0B  = _BV(WGM02)  | _BV(CS01); // fout = finterna / [prescalador * (OCR0A +1)]
  OCR0A = 25;//25
  OCR0B = 12;//12

}

void loop() {
  _delay_ms(60);
  pulse();
  pulse();
  _delay_ms(2000);
}
void pulse() {
  TCCR0A |= _BV(COM0B1);
  _delay_us(2000);
  TCCR0A &= ~_BV(COM0B1); 
  _delay_ms(28);
  TCCR0A |= _BV(COM0B1);
  _delay_us(390);
  TCCR0A &= ~_BV(COM0B1);  
  _delay_us(1580);
  TCCR0A |= _BV(COM0B1);
  _delay_us(410);
  TCCR0A &= ~_BV(COM0B1);  
  _delay_us(3580);
  TCCR0A |= _BV(COM0B1);
  _delay_us(400);
  TCCR0A &= ~_BV(COM0B1);  
  _delay_ms(63);
}
