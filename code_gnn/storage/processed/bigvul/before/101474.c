int DirectoryBackingStore::SafeDropTable(const char* table_name) {
  string query = "DROP TABLE IF EXISTS ";
  query.append(table_name);
  sqlite_utils::SQLStatement statement;
  int result = statement.prepare(load_dbhandle_, query.data(),
                                 query.size());
  if (SQLITE_OK == result) {
    result = statement.step();
    if (SQLITE_DONE == result)
      statement.finalize();
  }

  return result;
}
