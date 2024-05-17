IndexedDBDatabaseError IndexedDBDatabase::CreateError(
    uint16_t code,
    const char* message,
    IndexedDBTransaction* transaction) {
  transaction->IncrementNumErrorsSent();
  return IndexedDBDatabaseError(code, message);
}
