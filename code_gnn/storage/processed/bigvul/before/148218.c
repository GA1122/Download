void IndexedDBCursor::Advance(uint32_t count,
                              scoped_refptr<IndexedDBCallbacks> callbacks) {
  IDB_TRACE("IndexedDBCursor::Advance");

  if (closed_) {
    callbacks->OnError(CreateCursorClosedError());
    return;
  }

  transaction_->ScheduleTask(
      task_type_,
      BindWeakOperation(&IndexedDBCursor::CursorAdvanceOperation,
                         ptr_factory_.GetWeakPtr(), count, callbacks));
 }
