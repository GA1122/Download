int ExecQuery(sqlite3* dbhandle, const char* query) {
  sqlite_utils::SQLStatement statement;
  int result = statement.prepare(dbhandle, query);
  if (SQLITE_OK != result)
    return result;
  do {
    result = statement.step();
  } while (SQLITE_ROW == result);

  return result;
}
