void SQLiteDatabase::SetMaximumSize(int64_t size) {
  if (size < 0)
    size = 0;

  int current_page_size = PageSize();

  DCHECK(current_page_size || !db_);
  int64_t new_max_page_count = current_page_size ? size / current_page_size : 0;

  MutexLocker locker(authorizer_lock_);
  EnableAuthorizer(false);

  SQLiteStatement statement(
      *this, "PRAGMA max_page_count = " + String::Number(new_max_page_count));
  statement.Prepare();
  if (statement.Step() != kSQLResultRow)
    DLOG(ERROR) << "Failed to set maximum size of database to " << size
                << " bytes";

  EnableAuthorizer(true);
}
