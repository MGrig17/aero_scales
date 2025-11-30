# aero_scales
...
Итоговый репозиторий
Сборка:
  Скачать репозиторий.
  Загрузить heap_aero.ino
  Составляющие для корректной работы heap_aero.ino, которые должны находиться в папке скетча:
    heap_aero.ino - главный файл программы
    config.h - настройки пинов и параметров
    WeightSensor.h + WeightSensor.cpp - работа с тензодатчиками
    StepperController.h + StepperController.cpp - управление шаговым двигателем
    AngleSensor.h + AngleSensor.cpp - магнитный датчик угла AS5600
    DisplayHandler.h + DisplayHandler.cpp - LCD дисплей
    Potentiometer.h + .cpp
    CommandParser.h + CommandParser.cpp - обработка команд из Serial

  Python-files:
    arduino_controller_v2.py -- управление ардуиновским serial через консоль питон + снятие данных
    plot_data.py -- построение графиков

  
