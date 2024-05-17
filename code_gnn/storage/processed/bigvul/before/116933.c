void IndexedDBDispatcher::CursorDestroyed(int32 cursor_id) {
  cursors_.erase(cursor_id);
}
