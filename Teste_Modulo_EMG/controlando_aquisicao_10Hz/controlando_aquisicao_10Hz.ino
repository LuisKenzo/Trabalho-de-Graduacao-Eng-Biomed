  /////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Luís Kenzo Takayama
//Aquisição de sinais EMG - utilizando módulo EMG com filtro RC na saída
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Fazer função de calibração - apenas para ajustar os parâmetros 




//Declaração de variáveis
//Parâmetros
long frequencia = 10;                 //frequência de aquisição do sinal, neste caso, 10 sinais por segundo
float fator1 = 0.02, fator2 = 0.01;
float incerteza = 0.13;
float pico_basal = 130; // Mínimo para ser considerado um pico de contração
int filtro_horizontal=200000; //intervalo de 200ms valores não pode ter mais de 1 contração

//Matemáticas e variáveis auxiliares
unsigned long atraso_us = (1000000.0/frequencia);//Matemática para encontrar o período em us com a frequência inserida
int medida;
unsigned long tempo_atual, tempo_anterior;
float mediaMovel_1, mediaMovel_2=0;       //Para começar os cálculos, mas isto deixa os primeiros valores inclinados ao zero
float mediaMovel_2_UP,mediaMovel_2_DOWN;
int Cruzamento,Soma_Cruzamento,contracao=0;
float tempo_cruzamento_atual,tempo_cruzamento_anterior; 

void setup() {
  Serial.begin(115200);                 //em 115200 permite amostragem de até 2000hz
  Serial.println("Configuracao_ok!"); 
  tempo_anterior = micros();            //Condição inicial - Retorna tempo em microsegundos desde que Arduino começou a executar o programa
  tempo_cruzamento_anterior=micros();   //Condição inicial
  //Títulos dos dados
  Serial.print("Biceps_D,"); 
  Serial.print("mediaMovel_1,");
  Serial.print("mediaMovel_2,");
  Serial.println("Contracao"); 
}

void loop() { 
    tempo_atual = micros(); 
    
    // o abs() evita problemas com overflow da função micros()    
    if (abs(tempo_atual - tempo_anterior) >= atraso_us) { //Quando a diferença de tempo for >= ao Passo, fazemos a coleta do dado = AMOSTRAGEM
      tempo_anterior = tempo_atual;       //Redefine o tempo anterior para a próxima iteração
      medida = analogRead(A0);            //Coleta dado
      Serial.print(medida); 
      Serial.print(",");
      
      //Cálculos de médias móveis
      mediaMovel_1 = mediaMovel_1*(1-fator1)+ medida*(fator1);
      mediaMovel_2 = mediaMovel_2*(1-fator2)+ medida*(fator2);
      mediaMovel_2_UP = mediaMovel_2+incerteza;
      mediaMovel_2_DOWN = mediaMovel_2-incerteza;
      
      // Detectação se houve cruzamento das médias - Filtro vertical
      if(mediaMovel_1 < mediaMovel_2_UP  &&  mediaMovel_1 > mediaMovel_2_DOWN  && mediaMovel_1>pico_basal){ 
        tempo_cruzamento_atual=micros();
        
        //Se houve cruzamento em período maior que o filtro horizontal, então houve contração
        if (abs(tempo_cruzamento_atual-tempo_cruzamento_anterior)>= filtro_horizontal){
          contracao=contracao+1;
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
