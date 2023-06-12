# remedio_hora_certa

Bibliotecas: O código inclui várias bibliotecas necessárias para várias funcionalidades, como conexão com Wi-Fi, manipulação de data e hora,
comunicação com o Telegram e manipulação de JSON.

Configuração da Rede Wi-Fi: São definidas as informações de rede Wi-Fi, como o nome (ssid) e a senha da rede (password).

Configuração do Bot do Telegram: O token do bot do Telegram é definido para permitir o envio de mensagens para o bot. Também são definidos os
IDs dos chats do Telegram para os quais as mensagens serão enviadas. Esses IDs devem ser obtidos a partir dos chats no Telegram.

Definição dos Pinos: São definidos os pinos dos componentes conectados ao ESP32, como o pino do buzzer, do botão e do LED.

Inicialização: Na função setup(), os pinos são configurados como entrada ou saída, a comunicação serial é iniciada e a conexão Wi-Fi é 
estabelecida.

Aviso Sonoro Inicial: O método emitirAvisoSonoro() é chamado para emitir um aviso sonoro inicial ao ligar o LED.

Envio de Mensagens: A função enviarMensagemTelegram() é responsável por enviar mensagens para o Telegram. É feita uma solicitação HTTP para
a API do Telegram usando o token do bot e o ID do chat. A mensagem a ser enviada é passada como parâmetro.

Loop Principal: Na função loop(), o horário é atualizado com timeClient.update(). Em seguida, é verificado se chegou o momento de emitir o
aviso sonoro e enviar uma mensagem para o Telegram. Isso ocorre quando o intervalo definido é atingido. Também é verificado se o botão foi
pressionado. Se o botão foi pressionado pela primeira vez, o buzzer é desativado e uma mensagem é enviada para o Telegram informando que 
a medicação foi tomada. O LED também é desligado. Se o buzzer estiver ativado, ele é mantido ligado junto com o LED. Caso contrário, 
o buzzer e o LED são desligados.

Este é um esboço básico para um sistema de lembrete de medicação que utiliza o ESP32 e o Telegram para enviar notificações.
Você precisará configurar corretamente as informações de rede Wi-Fi, o token do bot do Telegram e os IDs dos chats para que o código
funcione corretamente. Certifique-se de ter as bibliotecas necessárias instaladas no ambiente de desenvolvimento Arduino.
