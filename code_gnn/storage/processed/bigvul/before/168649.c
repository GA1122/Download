void IndexedDBTransaction::GrabSnapshotThenStart() {
  DCHECK(!backing_store_transaction_begun_);
  transaction_->Begin();
  backing_store_transaction_begun_ = true;
  Start();
}
