void SQLiteDatabase::Close() {
  if (db_) {
    sqlite3* db = db_;
    {
      MutexLocker locker(database_closing_mutex_);
      db_ = nullptr;
    }
    sqlite3_close(db);
  }

  opening_thread_ = 0;
  open_error_ = SQLITE_ERROR;
  open_error_message_ = CString();
}
