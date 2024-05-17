bool SQLiteDatabase::IsAutoCommitOn() const {
  return sqlite3_get_autocommit(db_);
}
