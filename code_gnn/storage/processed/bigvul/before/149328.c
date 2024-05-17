void IndexedDBConnection::AbortTransaction(IndexedDBTransaction* transaction) {
  IDB_TRACE1("IndexedDBDatabase::Abort", "txn.id", transaction->id());
  transaction->Abort();
}
