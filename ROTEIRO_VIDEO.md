# Roteiro do vídeo - máximo de 3 minutos

## 0:00 a 0:20 - Apresentação

"Olá, meu nome é Gabriel Castilla Cavaloti. Este é o Sistema IoT para Monitoramento de Cápsula Espacial, desenvolvido no Wokwi."

## 0:20 a 0:50 - Componentes

Mostrar o circuito e explicar:

- DHT22 para temperatura.
- Fotoresistor para luminosidade.
- MPU6050 para vibração.
- LCD para mostrar os dados.
- LEDs e buzzer para os alertas.

## 0:50 a 1:30 - Funcionamento normal

Iniciar a simulação e mostrar o LCD com temperatura, luminosidade, vibração e situação normal. Mostrar também o LED verde.

## 1:30 a 2:10 - Alertas

Alterar um sensor para entrar em atenção e mostrar o LED amarelo. Depois alterar a temperatura para mais de 35 °C ou aumentar a vibração para demonstrar o estado crítico, o LED vermelho e o buzzer.

## 2:10 a 2:40 - Explicação do código

Mostrar rapidamente:

- A leitura dos três sensores.
- A função `verificarNivel()`.
- A função `controlarAlertas()`.
- A atualização do display.

## 2:40 a 3:00 - Conclusão

"O sistema realiza monitoramento contínuo e apresenta alertas visuais e sonoros. Como melhoria futura, os dados poderiam ser enviados para uma central de controle pela internet."
