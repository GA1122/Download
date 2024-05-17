void IndexedDBCursor::Close() {
  if (closed_)
    return;
  IDB_ASYNC_TRACE_END("IndexedDBCursor::open", this);
  IDB_TRACE("IndexedDBCursor::Close");
  closed_ = true;
  cursor_.reset();
  saved_cursor_.reset();
  transaction_ = nullptr;
}
