# Kullman Alexandru, 323CA

# Tema 3 - Aplicatie Client Web. Comunicatie cu REST API.

# Info

- Aplicatia este bazata pe mai multe fisiere sursa, organizate pe baza functionalitatii acestora.
	- client.c - gestioneaza comenzile utilizatorului, primite de la tastatura
	- prompts.c - ofera o interfata utilizatorului, in functie de comanda, pentru a sti ce date
	  urmeaza a fi introduse
	- commands.c - executa o actiune specifica fiecarei comenzi, trimite cereri serverului si
	  interpreteaza raspunsul
	- helpers.c - functii utile pentru conectarea si comunicatia dintre client si server
	- requests.c - functii pentru construirea mesajului ce va fi trimis ulterior serverului
	- parson.c - biblioteca pentru lucrul cu string-uri in format JSON
	= buffer.c - functii auxiliare de lucru cu buffer, utilizate in helpers.c

- Clientul poate trimite mai multe request-uri catre server, iar aplicatia are obligatia de a-i
  returna rezultatul comenzii, fie ca este succes sau eroare. Actiunile pe care clientul le poate
  desfasura sunt: inregistrarea, conectarea cu un cont la server, modificarea propriei biblioteci,
  preluarea informatiilor despre cartile aflate in biblioteca, iesirea din cont si inchiderea
  aplicatiei. Clientul poate cere date prin intermediul unor cereri HTTP interpretate de API-ul
  serverului

# Functionalitatea aplicatiei

 * Se asteapta input de la tastatura din partea utilizatorului. In cazul unei comenzi valide, se va
   oferi o interfata pentru introducerea informatiilor sau se va intoarce direct rezultatul, iar in
   cazul unei comenzi invalide, se va afisa o lista ajutatoare cu posibilele actiuni pe care le
   poate face clientul.

 * Dupa ce a fost trimisa o cerere HTTP, raspunsul va fi evaluat prin intermediul functiei
   **get_response_status**
	* Aceasta functie decide daca request-ul s-a finalizat cu succes sau a existat o eroare.
  	  Un mesaj corespunzator va informa utilizatorul despre rezultatul request-ului, pe baza
        raspunsului de la server.

 * Comanda **register**, i se va cere clientului sa introduca un username si o parola, fiind informat ca
   s-a creat un cont nou in caz de succes. Username-ul nu trebuie sa exista deja in baza de date a
   serverului.

 * Comanda **login**, situatia este similara cu cea de la register, se va verifica daca exista un
   cont cu acel username si daca parola introdusa corespunde contului. Se va intoarce un cookie de
   conectare pentru sesiunea curenta pe care aplicatia il va trimite automat mai departe pentru a
   demonstra ca actualul client este conectat intr-un cont.

 * Comanda de **enter_library** intoarce un token JWT, cu ajutorul caruia se garanteaza accesul
   catre biblioteca asociata contului.
	* Functia **get_token** cauta token-ul in body-ul raspunsului si intoarce Authorization Token-ul
 	  ce va fi transmis mai departe automat de catre aplicatie in viitoarele cereri HTTP ce solicita
        date din biblioteca.

 * Comanda **get_books** intoarce o lista cu informatii sumare (id si titlu) pentru cartile aflate
   in biblioteca utilizatorului.
	* Functia **print_books_info** ia fiecare carte in parte si se foloseste de biblioteca parson
        pentru a putea extrage campurile (id si title) si a le afisa utilizatorului

 * Comanda **get_book** returneaza o lista de informatii detaliate pentru o anumita carte,
   specificata de client prin intermediul id-ului.
	* Functia **print_book_detailed** se foloseste de biblioteca parson pentru a privi string-ul dat
	  ca parametru ca pe un obiect JSON. Astfel, se pot extrage valorile campurilor, fiind afisate 
	  intr-un format mai simplu pentru utilizator.

 * Comanda **add_book** va oferi un prompt pentru ca utilizatorul sa completeze informatiile cartii
   pe care o doreste sa adauge in lista din cont. Un mesaj de eroare se va afisa in cazul in care
   numarul de pagini introdus nu este valid.

 * Comanda **delete_book** sterge o carte din biblioteca, pe baza id-ului.

 * Comanda **logout** reseteaza Authorization Token-ul si Connection Cookie-ul. Clientul care
   a dat comanda nu va mai fi conectat din acel moment la contul sau.

 * Comanda **exit** opreste executia aplicatiei, care va elibera resursele folosite in final.


# Tipuri de request-uri
	
 * Mesajul de request este compus prin adaugarea host-ului, ip-ului si tuturor headerlor necesare
   pentru ca acesta sa fie valid. 
	1. GET - se cer date de la server fara ca acestea sa fie modificate. Se pot seta query-uri si
	   	   cookie-uri.
 	2. POST - se cer date de la server, urmand sa fie adaugate/modificate. Se pot seta cookie-uri,
                iar body-ului mesajului va contine informatia ce va fi interpretata de server
		    pentru a indeplini actiunea dorita
	3. DELETE - se cer date de la server cu scopul de a fi inlaturate. Se pot seta cookie-uri


# Surse si materiale utilizate in rezolvarea temei:

https://ocw.cs.pub.ro/courses/pc/laboratoare/10
https://github.com/kgabis/parson (biblioteca pentru utilizarea JSON)
https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input


 