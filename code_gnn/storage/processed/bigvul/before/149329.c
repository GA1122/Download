void IndexedDBConnection::AbortTransaction(
    IndexedDBTransaction* transaction,
    const IndexedDBDatabaseError& error) {
  IDB_TRACE1("IndexedDBDatabase::Abort(error)", "txn.id", transaction->id());
  transaction->Abort(error);
}
