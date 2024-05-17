int SQLiteDatabase::LastError() {
  return db_ ? sqlite3_errcode(db_) : open_error_;
}
