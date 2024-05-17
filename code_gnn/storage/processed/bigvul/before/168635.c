void IndexedDBDatabase::TransactionCreated(IndexedDBTransaction* transaction) {
  transaction_count_++;
  transaction_coordinator_.DidCreateTransaction(transaction);
}
