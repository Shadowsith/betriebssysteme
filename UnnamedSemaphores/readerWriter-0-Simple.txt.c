// Siehe Glatz pp 197
// Einfacher Ansatz.
// Nur ein Leser oder ein Schreiber hat Zugriff

int saldo;  // Gemeinsame Variable
sema* rwblock = DijkstraSemCreateNN(1); // Sperrt weitere Schreiber oder Leser aus,
// wenn Marke weg

int get_balance() { // Wird von Leser nach Bedarf aufgerufen
  int balance;    // Temporaere Ablage fuer gelesenen Wert

  P(rwblock);  // Weitere Schreiber und Leser aussperren
  balance = saldo; // Kontensaldo auslesen
  V(rwblock);  // Weitere Schreiber und Leser zulassen

  return balance; // Funktionswert zurueckgeben
}

void book(int amount) {  // Wird vom Schreiber nach Bedarf aufgerufen

  P(rwblock);  // Weitere Schreiber und Leser aussperren
  saldo = saldo + amount; //Buchung ausfuehren
  V(rwblock);  // Weitere Schreiber und Leser zulassen
}

