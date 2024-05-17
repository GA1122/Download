void IndexedDBTransaction::ProcessTaskQueue() {
  IDB_TRACE1("IndexedDBTransaction::ProcessTaskQueue", "txn.id", id());

  DCHECK(!processing_event_queue_);

  if (!should_process_queue_)
    return;

  processing_event_queue_ = true;

  DCHECK(!IsTaskQueueEmpty());
  should_process_queue_ = false;

  if (!backing_store_transaction_begun_) {
    transaction_->Begin();
    backing_store_transaction_begun_ = true;
  }

  TaskQueue* task_queue =
      pending_preemptive_events_ ? &preemptive_task_queue_ : &task_queue_;
  while (!task_queue->empty() && state_ != FINISHED) {
    DCHECK_EQ(state_, STARTED);
    Operation task(task_queue->pop());
    leveldb::Status result = std::move(task).Run(this);
    if (!pending_preemptive_events_) {
      DCHECK(diagnostics_.tasks_completed < diagnostics_.tasks_scheduled);
      ++diagnostics_.tasks_completed;
    }
    if (!result.ok()) {
      processing_event_queue_ = false;
      database_->ReportError(result);
      return;
    }

    task_queue =
        pending_preemptive_events_ ? &preemptive_task_queue_ : &task_queue_;
  }

  if (!HasPendingTasks() && state_ != FINISHED && commit_pending_) {
    processing_event_queue_ = false;
    leveldb::Status result = Commit();
    if (!result.ok())
      database_->ReportError(result);
    return;
  }

  if (state_ == FINISHED) {
    processing_event_queue_ = false;
    return;
  }

  DCHECK(state_ == STARTED);

  if (mode_ != blink::kWebIDBTransactionModeReadOnly) {
    timeout_timer_.Start(
        FROM_HERE, GetInactivityTimeout(),
        base::Bind(&IndexedDBTransaction::Timeout, ptr_factory_.GetWeakPtr()));
  }
  processing_event_queue_ = false;
}
