IndexedDBCursor::IndexedDBCursor(
    std::unique_ptr<IndexedDBBackingStore::Cursor> cursor,
    indexed_db::CursorType cursor_type,
    blink::WebIDBTaskType task_type,
    IndexedDBTransaction* transaction)
    : task_type_(task_type),
      cursor_type_(cursor_type),
      transaction_(transaction),
      cursor_(std::move(cursor)),
      closed_(false),
      ptr_factory_(this) {
  IDB_ASYNC_TRACE_BEGIN("IndexedDBCursor::open", this);
 }
