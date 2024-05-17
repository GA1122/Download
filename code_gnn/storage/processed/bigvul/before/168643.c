leveldb::Status IndexedDBTransaction::Commit() {
  IDB_TRACE1("IndexedDBTransaction::Commit", "txn.id", id());

  timeout_timer_.Stop();

  if (state_ == FINISHED)
    return leveldb::Status::OK();
  DCHECK_NE(state_, COMMITTING);

  DCHECK(!used_ || state_ == STARTED);
  commit_pending_ = true;

  if (state_ != STARTED)
    return leveldb::Status::OK();

  if (HasPendingTasks())
    return leveldb::Status::OK();

  state_ = COMMITTING;

  leveldb::Status s;
  if (!used_) {
    s = CommitPhaseTwo();
  } else {
    scoped_refptr<IndexedDBBackingStore::BlobWriteCallback> callback(
        new BlobWriteCallbackImpl(ptr_factory_.GetWeakPtr()));
    s = transaction_->CommitPhaseOne(callback);
  }

  return s;
}
