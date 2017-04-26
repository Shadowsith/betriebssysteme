// Siehe Samuel Tardieu, 2011-01-07
// http://www.rfc1149.net/blog/2011/01/07/the-third-readers-writers-problem/
// Dort ohne weitere Quellenangabe auf Literatur
//
// Kein Vorrang, weder fuer Leser noch Schreiber, sondern faire Abarbeitung

int saldo;  // Gemeinsame Variable
int nr=0;   // Anzahl Leser

sema* order = DijkstraSemCreateNN(1);  // Garantiert Reihenfolge
sema* nrlock = DijkstraSemCreateNN(1); // Garantiert exklusiven Zugriff auf Zaehler nr
sema* access = DijkstraSemCreateNN(1); // Garantiert exklusiven Zugriff auf Daten

int get_balance() { // Wird von Leser nach Bedarf aufgerufen
  int balance;    // Temporaere Ablage fuer gelesenen Wert

  P(order);   // Merke Reihenfolge
  P(nrlock);  // Exklusiven Zugriff auf nr sicherstellen
  if(nr==0) P(access); // Erster Leser sperrt Schreiber aus
  nr = nr +1 ;
  V(order);   // Semaphor fuer Reihenfolge freigeben
  V(nrlock);  // Lock auf Zaehler freigeben

  balance = saldo; // Kontensaldo auslesen

  P(nrlock);  // Exklusiven Zugriff auf nr sicherstellen
  nr = nr -1; // Erniedrige Zaehler fuer offene Abfragen (Leser)
  if (nr == 0) V(access); // Letzter Leser laesst Schreiber wieder zu
  V(nrlock);  //

  return balance; // Funktionswert zurueckgeben
}

void book(int amount) {  // Wird vom Schreiber nach Bedarf aufgerufen

  P(order);   // Merke Reihenfolge
  P(access);  // Exklusiven Zugriff des Schreibers auf Daten sichern
  V(order);   // Semaphor fuer Reihenfolge freigeben

  saldo = saldo + amount; //Buchung ausfuehren
  V(access);  // Exklusiven Zugriff des Schreibers auf Daten beenden
}

