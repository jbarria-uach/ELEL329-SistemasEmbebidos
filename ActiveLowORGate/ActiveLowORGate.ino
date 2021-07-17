#define PA  9
#define PB  10
#define PC  11
#define PD  12
#define OUT 13

void setup() {
  pinMode(PA, INPUT_PULLUP);
  pinMode(PB, INPUT_PULLUP);
  pinMode(PC, INPUT_PULLUP);
  pinMode(PD, INPUT_PULLUP);
  pinMode(OUT, OUTPUT);
}

void loop() {
  /*  
   *  Given the "active low" logic:
   *  __________________________
   * | PA | PB | PC | PD || OUT |
   *  --------------------------
   * |  L |  L |  L |  L ||  L  |
   * |  L |  L |  L |  H ||  L  |
   * |  L |  L |  H |  L ||  L  |
   * |  L |  L |  H |  H ||  L  |
   * |  L |  H |  L |  L ||  L  |
   * |  L |  H |  L |  H ||  L  |
   * |  L |  H |  H |  L ||  L  |
   * |  L |  H |  H |  H ||  L  |
   * |  H |  L |  L |  L ||  L  |
   * |  H |  L |  L |  H ||  L  |
   * |  H |  L |  H |  L ||  L  |
   * |  H |  L |  H |  H ||  L  |
   * |  H |  H |  L |  L ||  L  |
   * |  H |  H |  L |  H ||  L  |
   * |  H |  H |  H |  L ||  L  |
   * |  H |  H |  H |  H ||  H  |
   *  --------------------------
   *  
   *  So: OUT = PA & PB & PC & PD
   */
  digitalWrite(OUT, digitalRead(PA) & digitalRead(PB) \
                    & digitalRead(PC) & digitalRead(PD));
}
