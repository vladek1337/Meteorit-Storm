Meteorit Storm

Andrei Vlad-Cristian - Grupa 252 - Joi 18:00
        	
 1. Jocul incepe dupa ce cele 6 LED-uri (4 albastre, 1 alb si 1 rosu) se aprind, iar utilizatorul apasa butonul de pe joystick.
 2. La final se vor aprinde cele 4 LED-uri albastre in functie de nivelul la care a ajuns utilizatorul, precum si unul dintre cele 2 LED-uri, alb sau rosu, in functie de actiunea care a declansat terminarea jocului (LED-ul alb daca nava a evadat din ploaia de meteoriti, LED-ul rosu daca nava utilizatorului a intrat in coliziune cu un meteorit). Pentru a reincepe jocul utilizatorul trebuie sa apese butonul de pe joystick.
 3. Cele doua conditii indeplinite sunt
 a) “Sa aiba dificultate” – deoarece dificultatea jocului (viteza de cadere a meteoritilor) creste pe 5 nivele
 b) Dificultatea este indicata prin LED-urile albastre reprezentand scorul
	    1) 0 LED-uri albastre aprinse = nivelul 1;
	    2) 1 LED albastru aprins = nivelul 2;
      3) 2 LED-uri albastre aprinse = nivelul 3;
	    4) 3 LED-uri albastre aprinse = nivelul 4;
      5) 4 LED-uri albastre aprinse = nivelul 5;
  4. Jocul poate fi jucat de orice om cu experienta mediocra timp de 45 secunde.
  5. Cand se aprinde becul alb, utilizatorul a terminat nivelul 5 (ultimul nivel).
  6. Codul respecta stilul sugerat.
 	7. Descrierea jocului
a) LED-urile se vor aprinde succesiv. Dupa ce se aprinde LED-ul rosu utilizatorul poate apasa butonul de pe joystick pentru a lansa jocul. Utilizatorul controleaza o nava aflata intr-o ploaie de meteoriti. Scopul jocului este de a feri nava de obstacole pentru a ajunge cu bine la destinatie. Pe parcursul calatoriei, viteza meteoritilor creste, acest lucru fiind aratat de LED-urile albastre. Cu cat sunt mai multe aprinse cu atat nava este mai aproape de destinatie, dar si meteoritii sunt mai rapizi si mai greu de ocolit. La final, daca nava reuseste sa iasa din furtuna cu bine se va aprinde LED-ul alb. Daca aceasta va fi lovita de un meteorit, LED-ul rosu se va aprinde, nava va fi distrusa, iar jocul se va termina.
b) Placa Arduino este conectata la un controller MAX7219 (care la randul sau este conectat la o matrice de 8x8 LED-uri), la un joystick si la 6 LED-uri (4 albastre, 1 alb si 1 rosu). Fiecare linie a matricei este reprezentata printr-un byte. Un cursor este afisat ca 3 puncte pe coloana 1 si un punct pe cea de-a 2 a. La fiecare parcurgere a functiei predefinite loop() se citeste pozitia joystickului si daca detecteaza input diferit de valoarea de repaus, misca cursorul in sus sau in jos. La un anumit interval de timp (in functie de nivel) toate liniile sunt shiftate spre stanga, apoi se verifica daca nava a intrat in coliziune cu meteoritul (folosind “&” logic). Daca a intrat in coliziune, matricea primeste comanda de shutdown, iar LED-ul rosu este aprins. Dupa un anumit nr. de iteratii se aprinde urmatorul LED albastru, iar functia care shifteaza liniile la stanga este apelata mai des. Cand ajunge la ultimul nivel se aprinde LED-ul alb, iar jocul se termina.

