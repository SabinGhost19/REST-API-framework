ACADEMIA TEHNICĂ MILITARĂ “FERDINAND I”

FACULTATEA DE SISTEME INFORMATICE ŞI SECURITATE CIBERNETICĂ

Specializarea: Calculatoare şi sisteme informatice pentru apărare şi securitate naţională

Proiectarea Sistemelor de Operare-Proiect

C++ Rest API Framework

STAN Sabin-Ștefan

ȘTEOPOAE Anamaria-Peregrina

C-113A

Introducere
O biblioteca pentru dezvoltarea unui REST(Representational State Transfer) API(Application Programming Interface) este un set de instrumente, functionalitati si standard care faciliteaza construirea si gestionarea interfetelor API intr-un mod eficient si organizat.

Beneficii ale utilizarii unei biblioteci pentru REST API

Rapiditate in dezvoltare – Oferind o baza solida, devoltatorii pot econimisi timp si resurse

Modularitate – Permite dezvoltarea de componente independente care pot fi reutilizate in diferite proiecte

Cod mai curat si mentenabil – Separarea responsabilitatilor si organizarea clara a codului faciliteaza mentenanta si extindere aplicatiei

Standardizare – Promoveaza bune practice si standard de proiectare

Scopul proiectului
Dezvoltarea unui framework REST API in C++ are ca scop principal facilitarea semnificativa a procesului de creare a aplicatiilor care ofera servicii accesibile prin intermediul unor endpoint-uri HTTP. Implementarea unui astfel de framework in C++ este complexa, dar ofera o gama larga de beneficii si scopuri. C++ permite dezvoltarea unor mecanisme avansate de paralelism, drept urmare un framework implementat in acest limbaj poate oferi capacitatea de a gestiona un numar mare de cereri simultate in mod eficient. Oferind un set de reguli standardizate, un framework REST API poate asigura comunicarea dintre diferite componente dezvoltate in mod independent, folosind metode HTTP standard (GET, POST, PUT, DELETE etc).

Componente principale
In dezvoltarea proiectului ne vom utiliza de urmatoarele mari componente:

HTTP-Server - Clasa principala care gestioneaza initializarea si rularea serverului web. Aceasta ascultă pe un port specific si procesează cererile venite de la clienti.Se va deschide un socket de fiecare data cand o cerere este acceptata, si se va inchide dupa executarea rutine software dorita de developer. (La primirea unei cereri socket-ul se deschide, citim cererea, procesam datele, trimitem raspunsul, apoi inchidem socket-ul)

Request - O clasa care incapsuleaza cererea HTTP primita de la client. Include metode pentru accesarea URL-ului, metodei HTTP, headerelor,parametrilor si corpului cererii.

Detine functionalitate de parsare JSON , conversia JSON to std::string si invers(serializare/deserializare).

Response - Clasa responsabila pentru construirea răspunsului HTTP. Permite setarea statusului, adaugarea de headere si trimiterea datelor inapoi catre client.

Router - Aceasta clasa dirijeaza cererile HTTP catre handler-ele corespunzatoare bazandu-se pe metoda HTTP si calea URL solicitata. Ofera flexibilitate developerului si utlizarea facila a claselor Request si Response , fiind argumentele principale a functiei pe care acesta si-o creeaza. Se poate folosi de metodele puse la dispozitie de aceste clase pentru prelucrarea detaliilor cererii si gestionarea reaspunsului adecvat in functie de caz.

Middleware - Functii care se execută înainte de handler-ul final al cererii, permitand preprocesarea cererilor, autentificarea utilizatorilor, sau logarea cererilor si raspunsurilor.

Diagrama de clase
image

Detalierea cerintelor specifice
Workflow principal
Initializarea Serverului: Instanța de HTTPServer este configurata (localhost & port) si lansata, ascultand pe portul specificat.

Procesarea Cererilor: Atunci cand o cerere este primita, Router-ul analizeaza URL-ul și metoda pentru a delega cererea catre handler-ul corespunzator.

Executia Middleware-ului: Daca sunt middleware-uri configurate pentru calea ceruta, acestea sunt executate in ordine pentru a preprocesa cererea.

Handler-ul Cererii: Dupa executarea middleware-urilor, controlul este transferat la functia specifica care proceseaza logica service/bussines. (implementata de developer)

Construirea Răspunsului: Clasa Response este folosita pentru a seta statusul HTTP, headerele și pentru a trimite datele raspunsului inapoi catre client.

Finalizarea Cererii: Dupa trimiterea raspunsului, conexiunea poate fi inchisa sau mentinuta pentru cereri ulterioare, in functie de headerul Connection din cererea HTTP.

Cerinte aditionale
Pe langa cerintele principale pe care REST API framework-ul le va avea implementate, vom urmari ca utilizatorii sa beneficieze de o baza cat mai solida si bine structurata in procesul lor de dezvoltare al aplicatiilor web. Astfel, proiectul dezvoltat va creiona si urmatoarele aspecte:

Pentru o buna serializare si deserializare a datelor vom utiliza pachetele JSON, reprezentand o maniera facila de a transmite obiectele sau structurile de date;

Buna gestionare a cererilor primire de la clienti va fi asigurata prin folosirea unui vector de threaduri, care va gestiona taskurile primite; pe masura ce sunt primite, taskurile sunt stocate intr-o coada pentru a putea fi executate asincron intr-o maniera organizata;

Securitatea va fi asigurata prin anumite mecanisme de autentificare si autorizare. Acestea asigura ca doar utilizatorii autorizati au acces la anumite resurse si operatiuni ale API-ului;

Flexibilitatea si dinamica routingului vor fi implementate folosind regex;
