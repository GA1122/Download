void IndexedDBDatabase::TransactionCreated() {
  UMA_HISTOGRAM_COUNTS_1000(
      "WebCore.IndexedDB.Database.OutstandingTransactionCount",
      transaction_count_);
  ++transaction_count_;
}
