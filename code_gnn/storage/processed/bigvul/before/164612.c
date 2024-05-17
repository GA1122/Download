IndexedDBDatabaseError IndexedDBDatabase::CreateError(
    uint16_t code,
    const base::string16& message,
    IndexedDBTransaction* transaction) {
  transaction->IncrementNumErrorsSent();
  return IndexedDBDatabaseError(code, message);
}
