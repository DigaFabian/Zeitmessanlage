# Projekt: Zeitmessanlage BKT 2018/2019

## Inhalte

- Vorwort
- Anforderungen
- Realisierung
    - Hardware
    - Software
- Bedienung
- About

## Vorwort

Bei diesem Projekt handelt es sich um eine mögliche Umsetzung eines Zeiterfassungssystemes für die Gleichmäßigkeitsprüfung (GLP) von Oldtimern.

Bei der Umsetzung des Projekts lag der Fokus stets auf Effizienz, Funktionalität und Stabilität.

Dieses Projekt wurde geplant und entwickelt in einer Kooperation zwischen der Firma Di-Soric und dem Berufskolleg für Technik Lüdenscheid.

## Anforderungen

Bei dem gesamten System handelt es sich um einen portablen Aufbau, welcher sich der Situation vor Ort anpassen, aber dennoch einfach und zuverlässig bedienbar bleiben muss.

Es ist dabei vorgesehen, dass zwei Reflexionslichtschranken am Start- und Endpunkt eines Streckenabschnittes oder Parcours platziert werden, welche während des Programmablaufs von den Fahrzeugen ausgelöst werden.

Das Bedienkonzept des Systems ist sehr schlicht gehalten: Neben dem primären Ein-/Ausschalter gibt es lediglich einen weiteren Taster, welcher als Interaktionsmöglichkeit mit der Messanlage existiert.
Dieser wird beispielsweise für das Zurücksetzen (Reset) oder Bestätigen von Fehlermeldungen genutzt.

Für den Aufbau sind neben der Energieversorgung der Anlage ausschließlich die Anschlüsse der beiden Lichtschranken zu beachten.
*Mehr zu der Bedienung und dem Aufbau der Anlage unter » Bedienung «.*

## Realisierung

### Hardware

In dem Projekt wurde auf die folgende Hardware zurückgegriffen:
- Arduino Nano (Atmel AT328P)
- 2 × [di-soric OR 31 K 5000 P3-T4 Reflexionslichtschranke](https://www.di-soric.com/de/OR-31-K-5000-P3-T4-37530.html)
- Je ein Verbindungskabel für jede Lichtschranke
- LCD 1602
- 12 Volt Netzteil

*Schaltplan*
![](https://i.imgur.com/hfkFB1G.png)
*Bestückungsplan*
![](https://i.imgur.com/gazGtSt.png)





### Software

Die Software wurde ausschließlich in C geschrieben und kommt ohne externe Abhängigkeiten aus.

![](https://i.imgur.com/dPxv0dr.png)

Wir haben uns bei der Umsetzung für das State Machine Design-Pattern entschieden, da dieses während der Laufzeit einen durchgehend kontrollierten Programmfluss ermöglicht und Fehler so präventiv vermieden werden können.

Eine gut nachvollziehbare, wenig verschachtelte Struktur des Codes verspricht eine effiziente Wartung und mögliche Erweiterungen des Projektes.

Als Entwicklungsumgebung wurde die IDE [Atmel Studio 7](https://www.microchip.com/mplab/avr-support/atmel-studio-7) verwendet.

Eine native Bibliothek zur Ansteuerung des LCDs wurde uns im Rahmen dieses Projektes zur Verfügung gestellt:
[libLCD162CstaticLib.a](https://github.com/DigaFabian/Zeitmessanlage/blob/master/Zeitmessanlage/libLCD162CstaticLib.a) (Author: A. Broxtermann)

## Bedienung

Das Bedienkonzept des gesamten Systems wurde entsprechend den Projektanforderungen sehr intuitiv und minimalistisch gehalten, um mögliche Bedienungsfehler seitens des Anwenders im Vorfeld zu vermeiden und einen schnellen, effizienten und zuverlässigen Aufbau und eine einfache Nutzung zu gewährleisten.

### Aufbau

Für den Anwender sind bei dem Aufbau des Systems nur wenige, einfache Schritte erforderlich.

- Zunächst werden die Lichtschranken 1 und 2 mit dem enthaltenen Stativ am Start- bzw. Endpunkt der Teststrecke platziert. Die Konnektivität mit der Steuerungseinheit (Arduino) erfolgt jeweils über die beidseitig steckbaren Anschlussleitungen, welche sowohl mit der Lichtschranke, als auch mit dem dafür vorgesehenen Port der Steuerungseinheit verbunden werden.
- Die Energieversorgung der Steuerungseinheit wird situationsbedingt zweckdienlich über ein 12 Volt Netzteil zur Verfügung gestellt, welches an die KFZ-Bordspannungssteckdose eines Fahrzeuges angeschlossen wird.

### Setup

Nachdem die Versorgung mit Energie für die Steuerungseinheit hergestellt wurde, beginnt diese unmittelbar mit einem Initialisierungsprozess, um eine zuverlässige Funktionalität zu garantieren.

Der Anwender muss hier lediglich den Anweisungen auf dem LC-Display des Gerätes folgen und beide Lichtschranken je ein Mal manuell innerhalb des konfigurierten Zeitfensters (standardmäßig 10 Sekunden) unterbrechen bzw. auslösen.

Im Falle, dass das Zeitfenster nicht eingehalten werden kann, wird die Steuerungseinheit mit einer Fehlermeldung auf diesen Umstand hinweisen.
An diesem Punkt sollten die angeschlossenen Leitungen der Peripherie erneut überprüft und gegebenenfalls korrigiert werden.
Ist auch dies geschehen, kann der Taster betätigt werden, woraufhin erneut mit dem Initialisierungsprozess begonnen wird.

Ist die Funktion beider Lichtschranken mithilfe des Anwenders erfolgreich validiert worden, so beginnt der normale Programmfluss und es können Messungen durchgeführt werden.

### Messungen

Sobald die Initialisierung abgeschlossen ist, können Messungen mit dem System durchgeführt werden.
Der Ablauf für einen Messvorgang verläuft dabei wie folgt:

1. Die Steuerungseinheit signalisiert durch die Anzeige der Nachricht » *Ready to measure* «, dass der erste Sensor aktiv ist. Wird der Sensor ausgelöst, beginnt die Zeitmessung.
2. Wurde der Sensor innerhalb einer spezifischen Wartezeit *nicht* ausgelöst, wird die zweite Lichtschranke, mit welcher die Messung beendet werden kann, aktiviert.
3. Um die Messung zu beenden, wird die zweite Lichtschranke ausgelöst. Das Ergebnis der Messung wird mit einer Genauigkeit von 1/100 Sekunde auf dem LC-Display der Steuerungseinheit angezeigt.
4. Um den nächsten Messprozess zu beginnen, wird der Taster der Steuerungseinheit betätigt. Eine erneute Initialisierung ist hier *nicht* mehr erforderlich.

*Hinweis: während die Messung läuft, kann diese durch Betätigung des Tasters an der Steuerungseinheit unterbrochen werden. Anschließend ist eine erneute Initialisierung erforderlich.*

## About

Dieses Projekt wird gepflegt von [Fabian Diga](https://github.com/DigaFabian), Adrian Grubba und [Stefan F. Naujoks](https://github.com/2sfn).
