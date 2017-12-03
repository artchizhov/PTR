# PTR
Programmable time relay

This project contains software for creating a programmable time relay with several outputs.

The relay operates on the AVR microcontroller ATmega328. The relay has a keyboard for set parameters.

The relay works as follows:
You set the steps, set the time of each step and the state of each output channel.
When the device is started, it executes the configured program and performs each configured step, switching ON (1) and OFF (0) those relays that must be turned on and off at each step of the program.

You do not need to program anything from the computer, everything is configured with 4 buttons and other two - start and pause the program.

The settings are stored in the internal memory of the microcontroller.

The project is written with the C programming language in AVR Studio IDE.

########################################################################################

Программируемое реле времени

Этот проект содержит программное обеспечение для создания программируемого реле времени с несколькими выходами.

Реле работает на микроконтроллере AVR ATmega328. У реле есть клавиатура для настройки параметров работы.

Реле работает следующим образом:
Вы устанавливаете шаги, устанавливаете время каждого шага и состояние каждого выходного канала.
После запуска устройства, оно выполняет сконфигурированную программу и выполняет каждый сконфигурированный шаг, включая (1) и выключая (0) те реле, которые должны быть включены и выключены на каждом шаге программы.

Вам не нужно ничего программировать с компьютера, все настраевается 4мя кнопками, а еще двумя осуществляется запуск и остановка программы.

Настройки сохраняются во внутренней памяти микроконтроллера.

Проект написан на языке программирования C в среде AVR Studio.

Описание файлов/директорий:
bin-firmware - скомпилированные бинарные файлы, готовые для загрузки в микроконтроллер.
doc - инструкции и сопутствующие документы.
src - исходные коды проекта.

Загрузка программы в микроконтроллер:
Для загрузки "прошивки" можно использовать программу SinaProg.
В качестве программатора может быть использован USBAsp программатор для микроконтроллеров AVR.
Подключенние осуществляется по стандартной схеме подключения для микроконтроллеров AVR с использованием внутрисхемного программирования (ISP/ISCP).

Схема подключения:
В doc/wiring.png имеется принципиальная схема подключения нагрузок, LCD-экрана, кнопок управления и необходимых, для работы микроконтроллера, компонентов. ВНИМАНИЕ! Номиналы деталей указаны для примера! При сборке реальной схемы необходимы исправления для конкретных компонентов.
