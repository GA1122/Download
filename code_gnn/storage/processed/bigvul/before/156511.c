int64_t SQLiteDatabase::TotalSize() {
  int64_t page_count = 0;

  {
    MutexLocker locker(authorizer_lock_);
    EnableAuthorizer(false);
    SQLiteStatement statement(*this, "PRAGMA page_count");
    page_count = statement.GetColumnInt64(0);
    EnableAuthorizer(true);
  }

  return page_count * PageSize();
}
