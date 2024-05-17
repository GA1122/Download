void SQLiteDatabase::UpdateLastChangesCount() {
  if (!db_)
    return;

  last_changes_count_ = sqlite3_total_changes(db_);
}
