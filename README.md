# DC-DC
DC-DC step-up voltage converter with digital control

Устройство представляет из себя повышающий DC-DC преобразователь с функцией установки выходного напряжения по UART. 

Схема преобразователя предельно проста: микроконтроллер в 8-ногом корпусе, транзисторный ключ на полевом транзисторе, который коммутирует дроссель и обратная связь на делителе напряжения из двух резисторов (см. схему).
Принцип работы так же прост, как и схема: при установке уровня необходимого напряжения с помощью команды по UART, микроконтроллер начинает генерировать ШИМ сигнал, который подаётся на затвор коммутирующего транзистора, постепенно повышая скважность ШИМ сигнала. Одновременно с этим происходит опрос АЦП и сравнение результата преобразования с рассчитанной эталонной величиной. Затем в зависимости от результата сравнения скважность ШИМ изменяется в большую или меньшую сторону. Таким образом организуется обратная связь и, соответственно, стабилизация выходного напряжения. Частота ШИМ составляет около 30 KHz.
Управление преобразователем осуществляется с помощью команды по UART. Команда состоит из 5 последовательно отправляемых байт и имеет следующий формат: 1 байт - 0xDC (старт байт), 2 байт - 0xDC (маркер пакета), 3 байт - выходное напряжение (целая часть), 4 байт - выходное напряжение (сотые), 5 бйт - 0xCD (стоп байт). Например, надо задать выходное напряжение 12,38В, отправляем последовательно: 0xDC, 0xDC, 0x0C(12), 0x26(38), 0xCD - после получения пятого байта напряжение будет задано.
Настройка UART: 9600, 1 - stop, no parity.

При сборке схемы очень важно подобрать правильный дроссель (не только его индуктивность, но и толщину провода намотки, чтобы коммутирующий транзистор мог справиться с его коммутацией). Также необходимо наиболее точно подобрать сопротивление резисторов делителя напряжения в цепи обратной связи.
При прошивке микроконтроллера программой STC-ISP необходимо установить частоту внутреннего кварцевого генератора на 30MHz и включить ножку Reset.
Вся эта конструкция была протестирована в другом моём проекте и успешно работает. При питании преобразователя от USB он стабильно держит выходное напряжение не менее  25 Вольт (больше под нагрузкой я не тестировал, хотя предел делителя напряжения теоретически 33 Вольта при питании микроконтроллера от 3,3 Вольт).

Проект несложно адаптировать под другие микроконтроллеры, которые имеют на борту хотя бы по одному каналу АЦП и ШИМ и UART (можно даже без UART, если сделать установку напряжения с помощью подачи пачки импульсов на пин МК). Изначально проект был разработан и опробован на Nuvoton MS51FB9AE, затем доведён до ума на STM32G030. Всё, что нужно сделать - это настроить контроллёр (таймер) PWM, настроить канал АЦП, прерывание по приёму байта по UART и написать функцию задержки на 1 mS (если нет таковой встроенной). При наличии более богатой периферии МК преобразование АЦП можно запускать по таймеру, а в прерывании по окончанию преобразования делать сравнение результата АЦП. Тогда можно будет разгрузить МК для выполнения еще какой-либо функции, например - отслеживания и индикации ошибки. Но и в таком виде устройство работает очень хорошо.
