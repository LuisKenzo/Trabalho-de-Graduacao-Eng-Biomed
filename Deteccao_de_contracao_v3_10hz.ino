  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Luís Kenzo Takayama
//Aquisição de sinais EMG - utilizando módulo EMG com filtro RC na saída
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// > Mudar Frequência de amostragem para 6 (Mín) e testar 
// > Coletar Serial Print de um esquema de contração -> Fazer gráfico indicando resultado

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//DECLARAÇÃO DE VARIÁVEIS

//Parâmetros
long frequencia = 10;             //frequência de aquisição do sinal, neste caso, 1000 sinais por segundo
float fator1 = 0.30, fator2 = 0.22; //parâmetros de médias móveis
float incerteza = 60;             //Para cálculo de  cruzamento
float pico_basal = 130;           // Mínimo para ser considerado um pico de contração
unsigned long filtro_horizontal = 1500000UL;


//Matemáticas e variáveis auxiliares
unsigned long atraso_us = (1000000.0/frequencia);//Matemática para encontrar o período em us com a frequência inserida
int medida;
unsigned long tempo_atual, tempo_anterior;
float mediaMovel_1, mediaMovel_2=0;       //Para começar os cálculos, mas isto deixa os primeiros valores inclinados ao zero
float mediaMovel_2_UP,mediaMovel_2_DOWN;
int Cruzamento,Soma_Cruzamento,contracao=0;
unsigned long tempo_cruzamento_atual, tempo_cruzamento_anterior;
int Led = 9; // Porta PWM




void setup() {
  Serial.begin(115200);                 //em 115200 permite amostragem de até 2000hz
  Serial.println("Configuracao_ok!"); 
  tempo_anterior = micros();            //Condição inicial - Retorna tempo em microsegundos desde que Arduino começou a executar o programa
  tempo_cruzamento_anterior=micros();   //Condição inicial
  pinMode(Led , OUTPUT);

  
  //Títulos dos dados no serial print
  Serial.print("Biceps_D,"); 
  Serial.print("mediaMovel_1,");
  Serial.print("mediaMovel_2,");
  Serial.println("Contracao"); 
}

void loop() { 
    tempo_atual = micros(); 
    pico_basal = mediaMovel_2 + 50;
    
    // o abs() evita problemas com overflow da função micros()    
    if (abs(tempo_atual - tempo_anterior) >= atraso_us) { //Quando a diferença de tempo for >= ao Passo, fazemos a coleta do dado = AMOSTRAGEM
      tempo_anterior = tempo_atual;       //Redefine o tempo anterior para a próxima iteração
      medida = analogRead(A0);            //Coleta dado
      Serial.print(medida); 
      Serial.print(",");

      int pwm = map(medida, pico_basal, 500, 0, 255); // Converte medida do módulo EMG em intensidade de luz
      analogWrite(Led, pwm);
      
      //Cálculos de médias móveis
      mediaMovel_1 = mediaMovel_1*(1-fator1)+ medida*(fator1);
      mediaMovel_2 = mediaMovel_2*(1-fator2)+ medida*(fator2);
      mediaMovel_2_UP = mediaMovel_2+incerteza;
      mediaMovel_2_DOWN = mediaMovel_2-incerteza;
      
      // Detectação se houve cruzamento das médias - Filtro vertical
      if(mediaMovel_1 < mediaMovel_2_UP   && 
         mediaMovel_1 > mediaMovel_2_DOWN &&
         mediaMovel_1>pico_basal){ 
        
        tempo_cruzamento_atual=micros();
        
        //Se houve cruzamento em período maior que o filtro horizontal, então houve contração
        if ((tempo_cruzamento_atual-tempo_cruzamento_anterior)>= filtro_horizontal){
          contracao++;
          tempo_cruzamento_anterior=tempo_cruzamento_atual;
        }
      }      
      Serial.print(mediaMovel_1);
      Serial.print(",");
      Serial.print(mediaMovel_2);
      Serial.print(",");      
      Serial.println(contracao);
    } 
}
