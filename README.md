PGRprojekt
==========

Projekt do predmetu PGR: Jednoklávesová hra s použitím OpenGL
                         "﻿g-switch"


POHYB HRACA PRIORITNE
> cúvanie - stena pred ním [blocked] [sprite m+1..n]
> "státie na mieste" > behanie - len, keď má pôdu pod nohami - možné prepinanie X
                       [running, !blocked] [sprite 0..l]
                     > skakanie "padanie v opacnom smere" - zroluje sa (bolo stlačené x)
                       [press x, !running] [sprite l+1..m]
                     > padanie - neskákanie (nebolo stlačené x - nemá pôdu pod nohami)
                       [!running] [sprite 0]
?> posun dopredu - len, keď má turbo pod nohami, 
?                  ak je už "v strede obrazovky" zrýchlenie vykreslovania speed

MIMO OBRAZOVKU
            TOP_BORDER
LEFT_BORDER .......... RIGHT_BORDER
                0

cize pre <-1 a >16 netreba generovat steny


TODO:

Tereza:
> textury: sprite pre hraca (napr. x-suradnice 0..n), textura steny, textura pozadia
> v rohu vypisovat skore, napr. "<p.position.x>m (BEST <p.best>m)"
  asi riesit skor cez SDL (poznas width x height), nez openGL

Petr:
> prechod medzi levelmi:
  bud 1) plynulo nadviazat
?     2) vykreslit checkpoint - aktualizuje štartovaciu pozíciu / pred dalsim levelom
?     3) vykreslit turbo
> pred startom vykreslit 1. prvok [0][0] po LEFT_BORDER
  na konci opakovane vykreslit posledný prvok "donekonečna" = cieľ


Veronika:
> hranicne podmienky pri zmene levelu pri kolizii
?> turbo

