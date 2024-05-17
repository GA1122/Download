bool IndexedDBConnection::IsConnected() {
  return database_.get() != NULL;
}
