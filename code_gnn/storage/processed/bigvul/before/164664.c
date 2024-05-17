void IndexedDBDatabase::TransactionFinished(
    blink::mojom::IDBTransactionMode mode,
    bool committed) {
  --transaction_count_;
  DCHECK_GE(transaction_count_, 0);


  if (active_request_ &&
      mode == blink::mojom::IDBTransactionMode::VersionChange) {
    active_request_->UpgradeTransactionFinished(committed);
  }
}
