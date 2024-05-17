leveldb::Status IndexedDBTransaction::CommitPhaseTwo() {
  if (state_ == FINISHED)
    return leveldb::Status::OK();

  DCHECK_EQ(state_, COMMITTING);

  state_ = FINISHED;

  leveldb::Status s;
  bool committed;
  if (!used_) {
    committed = true;
  } else {
    base::TimeDelta active_time = base::Time::Now() - diagnostics_.start_time;
    uint64_t size_kb = transaction_->GetTransactionSize() / 1024;
    switch (mode_) {
      case blink::kWebIDBTransactionModeReadOnly:
        UMA_HISTOGRAM_MEDIUM_TIMES(
            "WebCore.IndexedDB.Transaction.ReadOnly.TimeActive", active_time);
        UMA_HISTOGRAM_COUNTS_1M(
            "WebCore.IndexedDB.Transaction.ReadOnly.SizeOnCommit2", size_kb);
        break;
      case blink::kWebIDBTransactionModeReadWrite:
        UMA_HISTOGRAM_MEDIUM_TIMES(
            "WebCore.IndexedDB.Transaction.ReadWrite.TimeActive", active_time);
        UMA_HISTOGRAM_COUNTS_1M(
            "WebCore.IndexedDB.Transaction.ReadWrite.SizeOnCommit2", size_kb);
        break;
      case blink::kWebIDBTransactionModeVersionChange:
        UMA_HISTOGRAM_MEDIUM_TIMES(
            "WebCore.IndexedDB.Transaction.VersionChange.TimeActive",
            active_time);
        UMA_HISTOGRAM_COUNTS_1M(
            "WebCore.IndexedDB.Transaction.VersionChange.SizeOnCommit2",
            size_kb);
        break;
      default:
        NOTREACHED();
    }

    s = transaction_->CommitPhaseTwo();
    committed = s.ok();
  }

  CloseOpenCursors();
  transaction_->Reset();

  database_->transaction_coordinator().DidFinishTransaction(this);

  if (committed) {
    abort_task_stack_.clear();

    if (!connection_changes_map_.empty()) {
      database_->SendObservations(std::move(connection_changes_map_));
      connection_changes_map_.clear();
    }
    {
      IDB_TRACE1(
          "IndexedDBTransaction::CommitPhaseTwo.TransactionCompleteCallbacks",
          "txn.id", id());
      callbacks_->OnComplete(*this);
    }
    if (!pending_observers_.empty() && connection_)
      connection_->ActivatePendingObservers(std::move(pending_observers_));

    database_->TransactionFinished(this, true);
    connection_->RemoveTransaction(id_);
    return s;
  } else {
    while (!abort_task_stack_.empty())
      abort_task_stack_.pop().Run();

    IndexedDBDatabaseError error;
    if (leveldb_env::IndicatesDiskFull(s)) {
      error = IndexedDBDatabaseError(
          blink::kWebIDBDatabaseExceptionQuotaError,
          "Encountered disk full while committing transaction.");
    } else {
      error =
          IndexedDBDatabaseError(blink::kWebIDBDatabaseExceptionUnknownError,
                                 "Internal error committing transaction.");
    }
    callbacks_->OnAbort(*this, error);
    database_->TransactionFinished(this, false);
  }
  return s;
}
