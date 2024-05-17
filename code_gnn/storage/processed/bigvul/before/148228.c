leveldb::Status IndexedDBCursor::PrefetchReset(int used_prefetches,
                                               int  ) {
  IDB_TRACE("IndexedDBCursor::PrefetchReset");
  cursor_.swap(saved_cursor_);
  saved_cursor_.reset();
  leveldb::Status s;

  if (closed_)
    return s;
  if (cursor_){
    DCHECK_GT(used_prefetches, 0);
    for (int i = 0; i < used_prefetches - 1; ++i) {
      bool ok = cursor_->Continue(&s);
      DCHECK(ok);
    }
  }

  return s;
}
