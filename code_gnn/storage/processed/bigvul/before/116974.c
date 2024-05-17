void IndexedDBDispatcher::ResetCursorPrefetchCaches(int32 exception_cursor_id) {
  typedef std::map<int32, RendererWebIDBCursorImpl*>::iterator Iterator;
  for (Iterator i = cursors_.begin(); i != cursors_.end(); ++i) {
    if (i->first == exception_cursor_id)
      continue;
    i->second->ResetPrefetchCache();
  }
}
