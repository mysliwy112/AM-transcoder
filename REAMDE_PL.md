### ANN-decoder
Dekoduje pliki .ann(imacji) z gier na silniku Piklib oraz Bloomoo zrobionych przez Aidem Media.

### Najnowsza wersja
[Tu](https://github.com/mysliwy112/ANN-decoder/releases)

### Instrukcja
Pliki animacji można dekodować, przeciągając je na dekoder (Anndrzem.exe).
Jako, że dekoder jest aplikacją wiersza poleceń to może on przyjmować dodatkowe argumenty.
(Żeby otworzyć wiersz poleceń w danym folderze należy, trzymając Shift, nacisnąć na pustym polu w folderze prawy przycisk myszy i wybrać opcję "Otwórz okno poleceń tutaj") potem po wpisaniu nazwy programu można podać mu parametry.

np. ```anndrzem -h``` wypisuje instrukcje

#Argumenty:
    -h Pokazuje plansze pomocy.
    -f Pozwala wybrać folder docelowy wyekstraktowanego pliku.
    -l Informacje dla programistów (log).
    -o Ekstraktuje obrazy bez tworzenia podfolderu.
    -s Ekstratuje sekwencję (klatki wybranej animacji(pozy) (wpisz "id:" żeby wybrać id pozy)(nazwę pozy do wyekstraktowania można wybrać po rozpoczęciu programu).
    -e Ekstraktuje czystą bitmapę bez nagłówka w formie	RGBA(8,8,8,8).
    -n Ustawia obrazki w jednej pozycji (przydatne do klejenia animacji z sekwencji).

Po argumentach można podać nazwy plików do wyekstraktowania np.:

```anndrzem -n -s PLAY ".\Przygoda\Cat\buty.ann```

Ekstraktuje do folderu buty, obrazy tworzące animację PLAY w pliku buty.ann, przystosowując je do złożenia w animację.

### Uwaga
Ukradłem tą sekcję z siostrzanego projektu [dePIKczer](https://github.com/Dove6/dePIKczer/raw/master/Release/dePIKczer.exe) służącego do dekodowania plików .img z tego samego silnika.

Używanie tego oprogramowania oraz dokonywanie jakichkolwiek zmian w produktach firmy Aidem Media jest niezgodne z umową licencyjną (zawieraną między Nabywcą a Firmą "z chwilą zdjęcia z opakowania płyty CD folii zabezpieczającej"), zgodnie z punktem drugim wspomnianej umowy:  
>Nabywca nie ma prawa do wynajmowania, dzierżawy, wypożyczania i rozpowszechniania programu, a także do przenoszenia licencji na osoby trzecie bez pisemnej zgody Producenta. Niedozwolone jest kopiowanie i modyfikacja fragmentów lub całości programu jak i wykorzystanie go w publikacjach i innych programach.

Grozi to konsekwencjami opisanymi w punkcie siódmym:  
>W przypadku naruszenia przez nabywcę umowy licencyjnej Producent będzie dochodził swoich praw i roszczeń, z wypowiedzeniem umowy włącznie. W razie sporów sądowych właściwy będzie Sąd siedziby Producenta.

Użytkowniku, zostałeś ostrzeżony.
