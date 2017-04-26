// Siehe Glatz pp 198/199
// Algorithmus nach P. Courtois et al. 1971
// Vorrang fuer Schreiber: wichtig fuer DBs

int saldo;  // Gemeinsame Variable
int nr=0;   // Anzahl Leser (offene Abfragen)
int nw=0;   // Anzahl Schreiber (offene Transaktionen)

sema* rblock = DijkstraSemCreateNN(1);  // Sperrt Leser aus, wenn Marke weg
sema* wblock = DijkstraSemCreateNN(1);  // Sperrt Schreiber aus, wenn Marke weg

sema* nrlock = DijkstraSemCreateNN(1);  // Exklusiver Zugriff auf Variable nr
sema* nwlock = DijkstraSemCreateNN(1);  // Exklusiver Zugriff auf Variable nw

sema* wprev  = DijkstraSemCreateNN(1);  // Sorgt fuer Vorrang der Schreiber

int get_balance() { // Wird von Leser nach Bedarf aufgerufen
  int balance;    // Temporaere Ablage fuer gelesenen Wert
  P(wprev);   // Nur einen Leser gleichzeitig hier reinlassen
  P(rblock);  // Weitere Leser behindern, nicht Schreiber
  P(nrlock);  // Exklusiven Zugriff auf nr sicherstellen
  nr = nr +1 ;
  if(nr==1) P(wblock); // Erster Leser sperrt Schreiber aus
  V(nrlock);
  V(rblock);  // Nun weitere Leser zulassen
  V(wprev);   // Naechsten Leser zulassen
  balance = saldo; // Kontensaldo auslesen
  P(nrlock);  // Exklusiven Zugriff auf nr sicherstellen
  nr = nr -1; // Erniedrige Zaehler fuer offene Abfragen (Leser)
  if (nr == 0) V(wblock); // Letzter Leser laesst Schreiber wieder zu
  V(nrlock);  //
  return balance; // Funktionswert zurueckgeben
}

void book(int amount) {  // Wird vom Schreiber nach Bedarf aufgerufen
  P(nwlock);     // Exklusiven Zugriff auf nw sicherstellen
  nw = nw + 1;   // Erhoehe Zaehler fuer offene Transaktionen (Schreiber)
  if (nw == 1) P(rblock); // Erster Schreiber sperrt Leser aus
  V(nwlock);              // (Bevor er weitere Schreiber aussperrt!)
  P(wblock);  // Weitere Schreiber aussperren
  saldo = saldo + amount; //Buchung ausfuehren
  V(wblock);  // Weitere Schreiber zulassen
  P(nwlock);    // Exklusiven Zugriff auf nw sicherstellen
  nw = nw -1 ;  // Erniedrige Anzahl fuer offene Transaktionen (Schreiber)
  if (nw== 0) V(rblock);  // Letzter Schreiber laesst Leser zu
  V(nwlock);
}

