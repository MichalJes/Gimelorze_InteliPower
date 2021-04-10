# **Gimelorze_InteliPower**
Projekt zakładał zaprojektowanie, oprogramowanie i wykonanie rozproszonego systemu nadzorującego oraz sterującego listą zasilającą.

## Power_Cord
Główny moduł komunikujący się z serwerem centralnym. Pozwala on na pomiar prądu w czasie rzeczywistym. Dodatkowo zaimplementowane jest zabezpieczenie wykonane za pomocą fotorezystora który wykrywa wyciągnięcie wtyczki, co skutkuje natychmiastowym rozłączeniem gniazdka. Kolejną funkcją jest układ identyfikacji poszczególnych urządzeń za pomocą tokenów. Każde z urządzeń ma dedykowany limit prądu. Dzięki temu możemy odpowiednio je zabezpieczyć.

## Motion_Detector
Układ wykrywający ruch w pomieszczeniu. Komunikuje się on z serwerem centralnym i pozwala wykryć czy ktoś opuścił pomieszczenie. Jeśli listwa wykryje znaczący pobór prądu, a układ nie został "ostrzerzony" wyśle on sygnał o żadaniu natychmiastowego odłączenia gniazdka. To żadanie zostanie przekazane przez serwer centralny do listy która rozłączy dane gniazdko.

## Main_Gateway
Serwer centralny odpowiadający za koordyację i komunikację pomiędzy pozostałymi urządzeniami. Posiada on prosty interfejsc HTML pozwalający na konfigurację całego projektu. Dodatkowo serwer w zamyśle jest bramką do interfejsu chmurowego z którego można sterować układami nie będą podłączonym pod sieć lokalną. 
