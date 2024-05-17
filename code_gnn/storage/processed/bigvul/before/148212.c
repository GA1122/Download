CursorImpl::IDBThreadHelper::IDBThreadHelper(
     std::unique_ptr<IndexedDBCursor> cursor)
     : cursor_(std::move(cursor)) {}
