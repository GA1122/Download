int SQLiteDatabase::PageSize() {
  if (page_size_ == -1) {
    MutexLocker locker(authorizer_lock_);
    EnableAuthorizer(false);

    SQLiteStatement statement(*this, "PRAGMA page_size");
    page_size_ = statement.GetColumnInt(0);

    EnableAuthorizer(true);
  }

  return page_size_;
}
