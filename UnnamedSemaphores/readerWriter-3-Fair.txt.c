// Algorithmus von Jalal Kawash, 2005
// Kein Vorrang, weder fuer Leser noch Schreiber, sondern faire Abarbeitung

int saldo;  // Gemeinsame Variable
int nr=0;   // Anzahl Leser
int nw=0;   // Anzahl Schreiber

sema* lock = DijkstraSemCreateNN(1);   // Garantiert exklusiven Zugriff auf Zaehler nr,nw
sema* access = DijkstraSemCreateNN(1); // Garantiert exklusiven Zugriff auf Daten

int get_balance() { // Wird von Leser nach Bedarf aufgerufen
  int balance;    // Temporaere Ablage fuer gelesenen Wert
  P(lock);  // Exklusiven Zugriff auf nr und nw sicherstellen
  if (nw > 0 || nr == 0) {
    V(lock);   // Lock auf Zaehler freigeben
    P(access); // Erster Leser sperrt Schreiber aus
    P(lock);   // Exklusiven Zugriff auf nr und nw sicherstellen
  }
  nr = nr +1 ;
  V(lock);  // Lock auf Zaehler freigeben

  balance = saldo; // Kontensaldo auslesen
  P(lock);  // Exklusiven Zugriff auf nr sicherstellen
  nr = nr -1; // Erniedrige Zaehler fuer offene Abfragen (Leser)
  if (nr == 0) V(access); // Letzter Leser laesst Schreiber wieder zu
  V(lock);  // Lock auf Zaehler freigeben
  return balance; // Funktionswert zurueckgeben
}

void book(int amount) {  // Wird vom Schreiber nach Bedarf aufgerufen
  P(lock);  // Exklusiven Zugriff auf nr und nw sicherstellen
  nw = nw + 1;
  V(lock);  // Lock auf Zaehler freigeben

  P(access);  // Exklusiven Zugriff des Schreibers auf Daten sichern
  saldo = saldo + amount; //Buchung ausfuehren

  P(lock);  // Exklusiven Zugriff auf nr und nw sicherstellen
  nw = nw - 1;
  V(lock);  // Lock auf Zaehler freigeben
  V(access);  // Exklusiven Zugriff des Schreibers auf Daten beenden
}

