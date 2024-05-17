int SQLiteDatabase::LastChanges() {
  if (!db_)
    return 0;

  return sqlite3_total_changes(db_) - last_changes_count_;
}
