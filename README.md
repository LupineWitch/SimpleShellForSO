# Projekt Prosty interpreter poleceń

## Opis:
**[10p]** Zaprojektuj i zaimplementuj prosty interpreter poleceń. Interpreter pobiera ze standardowego wejścia pojedynczy wiersz. Następnie dokonuje prostej analizy wiersza dzieląc go na słowa separowane spacjami. Pierwsze słowo jest nazwą programu, który należy uruchomić (wykorzystując zmienną PATH), a pozostałe są argumentami. Shell uruchamia program i standardowo czeka na zakończenie jego pracy, chyba że ostatnim słowem jest znak & co powoduje uruchomienie programu w tle, jak w normalnym shellu bash. Shell kończy pracę gdy otrzyma znak końca pliku. Dzięki temu możliwe jest przygotowanie prostych skryptów, które można uruchamiać z wiersza poleceń bash-a, jeżeli pierwsza linia skryptu ma postać #!/home/student/moj_shell  (gdzie po ! podaje się ścieżkę do programu shella). Dodatkowe opcje to:


a) **[6p]** możliwość przekierowania standardowego wyjścia polecenia przy pomocy >>

b) **[9p]** możliwość tworzenia potoków o dowolnej długości przy pomocy znaku |

c) **[9p]** historia poleceń - shell przechowuje (w zewnętrznym pliku w katalogu domowym użytkownika - tak że historia powinna "przetrwać" zakończenie shella) dokładną treść 20 poleceń, a wysłanie sygnału SIGINT powoduje wyświetlenie historii na standardowym wyjściu - **uwaga na niuanse związane z obsługą sygnałów !!!**


**Uwaga:** *Jako minimum, oprócz części podstawowej, projekt powinien mieć zrealizowany jeden z punktów a)-c)*


**EDIT** *(23 marca 2018): Z uwagi na problemy z implementacją nie obowiązuje uruchamianie skryptów z basha*

**[Link do CEZa](https://cez2.wi.pb.edu.pl/moodle/mod/page/view.php?id=5247)**
