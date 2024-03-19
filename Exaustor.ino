// Define os pinos do botão e do LED
const int botaoPin[4] = {2,3,4,5} ;
const int relePin[4] = {9,10,11,12};
const int ledPin[4] = {A0,A1,A2,A3};

unsigned long tempoAtivo = 14400000; // 14400000 //4 horas em ms 14400000

// Define a variável para armazenar o tempo em que o botão foi pressionado
unsigned long tempoBotaoPressionado[4];
unsigned long diferencaDeTempo[4];

int buttonState[4] = {LOW,LOW,LOW,LOW};
int lastButtonState[4] = {LOW,LOW,LOW,LOW};

// Define a variável para controlar o estado do relé
bool releAtivado[4] = {false,false,false,false};

void setup() {
  Serial.begin(9600);
  Serial.flush();
  Serial.write("Iniciado");
  Serial.flush();
  Serial.println();
  
  for (byte i = 0; i < 4; i = i + 1) {
    // Configura o pino do botão como entrada
    pinMode(botaoPin[i], INPUT);
    // Configura o pino do RELE como saída
    pinMode(relePin[i], OUTPUT);
    // Configura o pino do LED como saída
    pinMode(ledPin[i], OUTPUT);
    // Inicializa o estado do LED e RELE como desligado
    digitalWrite(relePin[i], HIGH);
    digitalWrite(ledPin[i], LOW);

    // Inicializa a variável de tempo
    tempoBotaoPressionado[i] = 0;
  }
}

void loop() {
  //int i = 3;
  for (byte i = 0; i < 4; i = i + 1) { //sao 4 botoes

    // Lê o estado do botão
    int estadoBotao = digitalRead(botaoPin[i]);
  
    //BOTAO
    // if the button state has changed:
    if (estadoBotao != buttonState[i]){
      buttonState[i] = estadoBotao;
      // Se o botão for pressionado
      if (buttonState[i] == HIGH) {
        Serial.write("Apertou ");
        Serial.print(i);
        Serial.write(" ");
        if (!releAtivado[i]) {
          Serial.write("ligar rele ");
          // Ativa o relé
          digitalWrite(relePin[i], LOW);
          digitalWrite(ledPin[i], HIGH);
          releAtivado[i] = true;
          // Armazena o tempo em que o botão foi pressionado
          tempoBotaoPressionado[i] = millis();
          Serial.print(tempoBotaoPressionado[i]);
        } else {
          Serial.write("desligar rele ");          
          // Desativa o relé
          digitalWrite(relePin[i], HIGH);
          digitalWrite(ledPin[i], LOW);
          releAtivado[i] = false;
        }
        Serial.println();
      } // end iff HIGH
    } //end if state

    // TIMER 
    if (releAtivado[i]) {
      // Verifica se o tempo decorrido desde que o botão foi pressionado é maior que tempoAtivo
      diferencaDeTempo[i] = millis() - tempoBotaoPressionado[i];
      //Serial.write(diferencaDeTempo);
      if (diferencaDeTempo[i] >= tempoAtivo || tempoBotaoPressionado[i] > millis() ) {  //milis tem overflow a cada 50 dias e zera. impedir que fique ligado para sempre
        // Desativa o relé
        Serial.write("timer desligar ");
        Serial.print(i);
        Serial.println();
        digitalWrite(relePin[i], HIGH);
        digitalWrite(ledPin[i], LOW);
        releAtivado[i] = false;
      }
    }//end if rele ativo

  }//end for 

  // Delay a little bit to avoid bouncing
  delay(50);
}
