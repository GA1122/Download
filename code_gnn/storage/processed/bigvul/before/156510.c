bool SQLiteDatabase::TableExists(const String& tablename) {
  if (!IsOpen())
    return false;

  String statement =
      "SELECT name FROM sqlite_master WHERE type = 'table' AND name = '" +
      tablename + "';";

  SQLiteStatement sql(*this, statement);
  sql.Prepare();
  return sql.Step() == SQLITE_ROW;
}
