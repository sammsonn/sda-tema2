**Nume: SAMSON Alexandru-Valentin**
**Grupă: 312CC**

## Tema 2 - Compresia imaginilor utilizând arbori

### Descriere

* Funcția averageRed calculează și returnează valoarea medie a componentei
roșii a tuturor pixelilor din regiunea dreptunghiulară dată. Funcția folosește
două bucle for imbricate pentru a itera prin toți pixelii din regiunea
dreptunghiulară, începând de la (x,y) și continuând până la (x + dimensiune,
y + dimensiune). În interiorul buclelor for, funcția adaugă componenta roșie a
pixelului curent la variabila roșie. După iterarea tuturor pixelilor din
regiunea dreptunghiulară, funcția calculează valoarea medie a componentei roșii
împărțind totalul roșu la numărul de pixeli s și returnează această valoare.
Același lucru este realizat și pentru averageGreen și averageBlue.

* Funcția mean calculează scorul de similitudine dintre o zonă dreptunghiulară
dată de pixeli și o culoare de referință. Funcția calculează mai întâi valorile
medii de roșu, verde și albastru pentru zona dreptunghiulară. Apoi, parcurge
aria dreptunghiulară a pixelilor și calculează diferențele pătrate dintre
valorile de roșu, verde și albastru ale fiecărui pixel și valoarea medie
corespunzătoare. Aceste diferențe pătrate sunt adăugate la un total curent
(medie) pentru fiecare pixel din zona dreptunghiulară. După ce bucla este
terminată, funcția împarte diferența totală pătrată cu de 3 ori dimensiunea
pătratului suprafeței dreptunghiulare pentru a obține scorul mediu de
similitudine pentru zona dreptunghiulară. În cele din urmă, funcția returnează
acest scor ca număr întreg.

* Funcția buildTree construiește un arbore cuaternar pentru o anumită zonă
dreptunghiulară de pixeli. Arborele este construit recursiv prin împărțirea
zonei în pătrate mai mici până când valoarea medie a culorii pătratului este
mai mică sau egală cu valoarea limită. Funcția începe prin a calcula valorile
RGB medii pentru pătratul curent folosind funcțiile averageRed, averageGreen și
averageBlue. Dacă valoarea medie a culorii pătratului este mai mare decât
limita, pătratul este împărțit în patru pătrate mai mici și se construiește
recursiv un arbore pentru fiecare dintre aceste pătrate mai mici. În cele din
urmă, funcția returnează nodul rădăcină al arborelui cuaternar.

* Funcția levelsNumber calculează numărul de niveluri dintr-un arbore
cuaternar. Dacă pointerul de intrare nu este NULL, funcția se autoapelează
recursiv pe fiecare dintre cei patru copii ai nodului curent și determină
numărul maxim de niveluri în sub-arborele fiecărui copil. Apoi adaugă 1 la
maximul acestor valori pentru a ține cont de nodul curent și returnează această
valoare ca număr de niveluri din arbore. Aplicând funcția max() la nivelurile
fiecărui subarbore, funcția este capabilă să determine numărul maxim de
niveluri din arbore.

* Funcția blocksNumber calculează numărul de pătrate complete care rămân după
împărțirea unei imagini în pătrate. Funcția verifică dacă scorul mediu al
blocului curent, calculat folosind funcția mean(), este mai mic sau egal cu
limita dată. Dacă este, atunci blocul este omogen și poate fi considerat un
singur bloc. În acest caz, funcția returnează 1. Dacă scorul mediu al blocului
este mai mare decât limita, atunci blocul nu este omogen, iar funcția numără
recursiv numărul de blocuri din fiecare dintre cele patru cadrane ale nodului
curent. Pentru a face acest lucru, funcția se autoapelează cu coordonatele și
dimensiunea fiecărui cadran și adună numărul de blocuri returnate de fiecare
apel. Rezultatul final este suma numărului de blocuri din toate cele patru
cadrane.

* Funcția biggestSize calculează lungimea laturii celui mai mare pătrat care
rămâne neîmpărțit într-un arbore dat. Dacă nodul curent nu are copii (adică
este un pătrat nedivizat), funcția returnează dimensiunea curentă a pătratului.
În caz contrar, funcția calculează recursiv cea mai mare dimensiune nedivizată
în fiecare copil al nodului curent. Pentru a face acest lucru, împarte
dimensiunea curentă cu 2 și transmite această dimensiune nouă fiecărui apel
recursiv. Funcția ia valoarea maximă dintre toate rezultatele obținute din
apelurile recursive și o returnează.

* Funcția compress implementează comprimarea unei imagini și scrie datele
comprimate într-un fișier binar. Aceasta scrie mai întâi dimensiunea imaginii.
Adaugă nodul rădăcină la coadă și procesează fiecare nod din coadă într-o
ordine de lățime. Pentru fiecare nod: verifică dacă nodul este o frunză sau nu,
dacă nodul nu este o frunză (adică are copii), acesta scrie un 0 în fișier
pentru a indica faptul că nodul este un nod intern și adaugă copiii nodului în
coadă. Dacă nodul este o frunză, acesta scrie un 1 în fișier pentru a indica
faptul că nodul este o frunză și scrie valorile de culoare RGB ale nodului în
fișier. În cele din urmă, eliberează memoria alocată pentru structura de date
a cozii.

* Funcția decompress implementează decomprimarea unei imagini prin construirea
unui arbore cuaternar dintr-un fișier binar comprimat. Funcția începe prin a
citi dimensiunea imaginii și tipul nodului rădăcină din fișierul binar. Apoi,
funcția adaugă nodul rădăcină la coadă. Funcția intră într-o buclă care continuă
până când coada este goală. În fiecare iterație a buclei, funcția elimină primul
element din coadă și îl atribuie variabilei nodului. Dacă nodul curent nu este o
frunză, funcția efectuează următoarele operații: Citește tipul de nod pentru
sub-arborele din dreapta sus din fișierul binar. Dacă sub-arborele din dreapta
sus nu este o frunză, acesta îl adaugă la coadă. În caz contrar, citește
valorile RGB pentru nodul frunză din fișierul binar și atribuie proprietăților
lui nod->urt. Funcția repetă apoi aceste operații pentru ceilalți trei
sub-arbori: sus-stânga (nod->ult), jos-dreapta (node->drt) și jos-stânga
(nod->dlt). Dacă nodul curent este o frunză, funcția citește valorile RGB
pentru nodul frunză din fișierul binar și le atribuie proprietăților nodului.
După ce toate nodurile au fost procesate, funcția eliberează memoria alocată
pentru coadă folosind funcția freeQueue() și returnează variabila dimensiune,
care este lungimea unei laturi a imaginii decomprimate.

* Funcția buildPixels construiește matricea de pixeli după decompresie pe baza
arborelui cuaternar. Funcția verifică dacă nodul curent este o frunză sau nu.
Dacă nodul curent este o frunză, acesta setează culoarea pixelilor din zona
nodului curent la culoarea nodului curent. Dacă nodul curent nu este o frunză,
funcția traversează recursiv nodurile copil ale nodului curent într-o anumită
ordine (ult, urt, drt, dlt). Pentru fiecare nod copil, funcția calculează noile
valori x, y și mărime pe baza zonei nodului curent și a poziției nodului copil
în acea zonă. Aceste noi valori sunt apoi folosite ca argumente pentru apelul
recursiv la buildPixels() pentru acel nod copil. Acest proces continuă recursiv
până când toate nodurile frunze din arbore au fost procesate și matricea de
pixeli a fost complet construită.

* Funcția createImage scrie mai întâi informațiile antetului imaginii în fișier.
După scrierea antetului, funcția trece peste fiecare pixel din matrice și îl
scrie în fișierul binar ppm.

* Funcția principală , În funcție de argumentul liniei de comandă furnizat,
execută una dintre cele trei sarcini. Pentru sarcinile de comprimare a imaginii,
programul citește datele de imagine dintr-un fișier binar de intrare. Pentru
sarcina 1, imprimă numărul de niveluri din arbore, numărul de blocuri din arbore
și dimensiunea celui mai mare bloc într-un fișier de ieșire. Pentru sarcina 2,
comprimă datele arborelui într-un fișier binar. Pentru sarcina de decomprimare
a imaginii, citește datele comprimate dintr-un fișier binar de intrare și le
decomprimă într-un arbore cuaternar. Apoi construiește o matrice de valori
pixeli după arbore și creează o imagine decomprimată. În cele din urmă,
eliberează memoria alocată pentru arbore și matricea de pixeli și închide
fișierele de intrare și de ieșire.

### Comentarii asupra temei

* Implementarea mi se pare destul de bună, am reușit să nu am leak-uri de
memorie, dar sigur era posibil să fie și mai eficientă sau mai scurtă. Totuși
sunt mulțumit cu această rezolvare.

* Tema această m-a învățat cum să lucrez cu arbori și mi-a reamintit de
recursivitate.

### Punctajul obținut la teste local

* Total: 100.0/100
* Valgrind: 20/20
