int64_t SQLiteDatabase::FreeSpaceSize() {
  int64_t freelist_count = 0;

  {
    MutexLocker locker(authorizer_lock_);
    EnableAuthorizer(false);
    SQLiteStatement statement(*this, "PRAGMA freelist_count");
    freelist_count = statement.GetColumnInt64(0);
    EnableAuthorizer(true);
  }

  return freelist_count * PageSize();
}
