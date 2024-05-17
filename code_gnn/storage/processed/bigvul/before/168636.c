void IndexedDBDatabase::TransactionFinished(IndexedDBTransaction* transaction,
                                            bool committed) {
  IDB_TRACE1("IndexedDBTransaction::TransactionFinished", "txn.id",
             transaction->id());
  --transaction_count_;
  DCHECK_GE(transaction_count_, 0);

  if (active_request_ &&
      transaction->mode() == blink::kWebIDBTransactionModeVersionChange) {
    active_request_->UpgradeTransactionFinished(committed);
  }
}
