#include <WiFi.h> // Fornece funções e classes para conectar-se a redes Wi-Fi e gerenciá-las
#include <NTPClient.h> // Fornece uma implementação de cliente NTP (Network Time Protocol) para sincronização de tempo em projetos
#include <WiFiClientSecure.h> // Fornece classes e funções para permitir a comunicação segura sobre uma conexão Wi-Fi usando o protocolo HTTPS
#include <WiFiUdp.h> // Fornece classes e funções para permitir a comunicação por meio do protocolo UDP (User Datagram Protocol) usando uma conexão Wi-Fi
#include <HTTPClient.h> // Fornece classes e funções para permitir a realização de requisições HTTP em projetos
#include <ArduinoJson.h> // Ela permite que você analise e crie dados JSON, facilitando a interação com serviços web e a troca de informações estruturadas

// Definindo a rede WiFi
const char* ssid = "NomeDaRedeWiFi";
const char* password = "SenhaDaRedeWiFi";

// Definindo o BOT que vai receber a mensagem no telegram
#define botToken  "TokemDoBotDoTelegram" // Coloque o tokem do bot do telegram

// Informe o ID do telegram
// Pode ser informado mais de um ID
// O ID é responsavel em enviar a mensagem para o dispositivo certo
std::vector<String> chatIds = {
    "ID_1", // ID do primeiro chat no Telegram
    "ID_2", // ID do segundo chat no Telegram
    "ID_3", // ID do terceiro chat no Telegram
};

// Definindo os pinos dos componentes
const int buzzerPin = 12; // Pino do BUZZER 
const int buttonPin = 14; // Pino do PUSH_BOTTON
const int ledPin = 13; // Pino do LED

// Faz a requisição NTP do horario
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
WiFiClientSecure secured_client;

// Inicializando os componentes
unsigned long previousMillis = 0;
unsigned long intervalo = 12 * 60 * 60 * 1000; // Define o intervalo entre os alarmes, está definido em 12 horas entre cada alarme.
bool buzzerAtivo = false;
bool botaoPressionado = false;
bool avisoSonoroInicial = true; // Estado do aviso sonoro inicial

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT); // Define o pino do LED como saída

  Serial.begin(19200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }

  timeClient.begin();
  timeClient.setTimeOffset(-10800);

  emitirAvisoSonoro(); // Emitir aviso sonoro inicial ao ligar o LED
}

// Quando ligado na energia e estiver conectado a rede wifi corretamente, ele emite um aviso sonoro
// Se ligado a energia e não etiver conectado ao wifi, ele não aciona o alarme sonoro
void emitirAvisoSonoro() {
  digitalWrite(buzzerPin, HIGH);
  delay(1000); // Duração do aviso sonoro (1 segundo)
  digitalWrite(buzzerPin, LOW);
  avisoSonoroInicial = false; // Desativa o aviso sonoro inicial
}

// Função que envia a mensagem
void enviarMensagemTelegram(const String& mensagem) {
  String url = "https://api.telegram.org/bot" + String(botToken) + "/sendMessage"; //Não precisa mudar nada

  for (const String& chatId : chatIds) {
    String payload = "chat_id=" + chatId + "&text=" + mensagem; //Não precisa mudar nada

    secured_client.setInsecure();

    HTTPClient http;
    http.begin(secured_client, url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Não precisa mudar nada

    int httpCode = http.POST(payload);

// Verifica se a mensagem foi enviada ou não
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        Serial.println("Mensagem enviada com sucesso!");
        Serial.println("Resposta do servidor:");
        Serial.println(response);
      } else {
        Serial.println("Erro ao enviar mensagem. Código HTTP: " + String(httpCode));
      }
    } else {
      Serial.println("Falha na conexão com o servidor.");
    }

    http.end();
  }
}

void loop() {
  timeClient.update();// Atualiza o horario

  unsigned long currentMillis = millis();

// Verifica se esta no horario e aciona o BUZZER e o LED
  if (currentMillis - previousMillis >= intervalo) {
    previousMillis = currentMillis;
    buzzerAtivo = true;
    botaoPressionado = false;  // Reinicia o estado do botão

    if (avisoSonoroInicial) {
      emitirAvisoSonoro();
    }
  }

// Verifica se o botão foi apertado
  if (digitalRead(buttonPin) == LOW && !botaoPressionado) {
    // Botão pressionado pela primeira vez
    botaoPressionado = true;
    buzzerAtivo = !buzzerAtivo;
    delay(100); // Debounce

// Se o botão foi apertado, envia uma mensagem para o telegram com o horario
    if (botaoPressionado) {
      // Buzzer está ativo, envia a mensagem para o Telegram
      String mensagem = "O remédio foi tomado em " + timeClient.getFormattedTime();
      enviarMensagemTelegram(mensagem);
      Serial.println(mensagem);
    }
  }

// Se o BUZZER estiver acionado e o BOTÂO não foi precionado, vai manter o BUZZER e o LED ativo
  if (buzzerAtivo && !botaoPressionado) {
    // Buzzer está ativo
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH); // Acende o LED
  } else {
    // Buzzer está desligado
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW); // Apaga o LED
  }
}
