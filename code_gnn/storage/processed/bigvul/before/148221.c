void IndexedDBCursor::Continue(std::unique_ptr<IndexedDBKey> key,
                               std::unique_ptr<IndexedDBKey> primary_key,
                               scoped_refptr<IndexedDBCallbacks> callbacks) {
  IDB_TRACE("IndexedDBCursor::Continue");

  if (closed_) {
    callbacks->OnError(CreateCursorClosedError());
    return;
  }

  transaction_->ScheduleTask(
      task_type_,
      BindWeakOperation(&IndexedDBCursor::CursorIterationOperation,
                        ptr_factory_.GetWeakPtr(), base::Passed(&key),
                        base::Passed(&primary_key), callbacks));
}
