IRremote verwendet den gleichen Timer wie Funktion tone(). Es werden deshalb Störungen geben und der InfrarotßModul wird die Signale nicht empfangen können. Aus dem Grund wird entweder die Einstellung im Bibliothekordner von IRremote geändert(in dem Fall ggf. für Arduino Uno => __AVR_ATmega328P__) oder die globale Variable vor dem Bibliothekimport aktualisiert:
**#define IR_USE_AVR_TIMER1**

**Schaltplan des Projektes in: /include/Schaltplan.png**
