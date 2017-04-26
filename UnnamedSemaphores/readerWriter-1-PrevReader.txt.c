// Siehe Glatz pp 197/198
// Vorrang fuer Leser
// Sobald ein Leser Zugriff hat, sind beliebig viele Leser zugelassen
// Schreiber wird solange gesperrt, bis alle Leser fertig sind
// Schreiber kann ausgehungert werden.


int saldo;  // Gemeinsame Variable
int nr=0;   // Anzahl Leser (offene Abfragen)

sema* wblock = DijkstraSemCreateNN(1); // Sperrt Schreiber und ersten Leser aus
sema* nrlock = DijkstraSemCreateNN(1); // Exklusiver Zugriff auf Variable nr

int get_balance() { // Wird von Leser nach Bedarf aufgerufen
  int balance;    // Temporaere Ablage fuer gelesenen Wert

  P(nrlock);  // Exklusiven Zugriff auf nr sicherstellen
  nr = nr +1 ;
  if(nr==1) P(wblock); // Erster Leser sperrt Schreiber aus
  V(nrlock);

  balance = saldo; // Kontensaldo auslesen

  P(nrlock);  // Exklusiven Zugriff auf nr sicherstellen
  nr = nr -1; // Erniedrige Zaehler fuer offene Abfragen (Leser)
  if (nr == 0) V(wblock); // Letzter Leser laesst Schreiber wieder zu
  V(nrlock);  //

  return balance; // Funktionswert zurueckgeben
}

void book(int amount) {  // Wird vom Schreiber nach Bedarf aufgerufen

  P(wblock);  // Weitere Schreiber und Leser aussperren
  saldo = saldo + amount; //Buchung ausfuehren
  V(wblock);  // Weitere Schreiber und Leser zulassen
}

