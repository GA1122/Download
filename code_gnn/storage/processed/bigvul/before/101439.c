bool DirectoryBackingStore::CheckIntegrity(sqlite3* handle, string* error)
    const {
  sqlite_utils::SQLStatement statement;
  statement.prepare(handle, "PRAGMA integrity_check(1)");
  if (SQLITE_ROW != statement.step()) {
    *error =  sqlite3_errmsg(handle);
    return false;
  }
  string integrity_result = statement.column_text(0);
  if (integrity_result != "ok") {
    *error = integrity_result;
    return false;
  }
  return true;
}
