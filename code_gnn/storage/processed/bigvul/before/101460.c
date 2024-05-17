bool DirectoryBackingStore::MigrateToSpecifics(
    const char* old_columns,
    const char* specifics_column,
    void (*handler_function)(sqlite_utils::SQLStatement* old_value_query,
                             int old_value_column,
                             sync_pb::EntitySpecifics* mutable_new_value)) {
  std::string query_sql = base::StringPrintf(
      "SELECT metahandle, %s, %s FROM metas", specifics_column, old_columns);
  std::string update_sql = base::StringPrintf(
      "UPDATE metas SET %s = ? WHERE metahandle = ?", specifics_column);
  sqlite_utils::SQLStatement query;
  query.prepare(load_dbhandle_, query_sql.c_str());
  while (query.step() == SQLITE_ROW) {
    int64 metahandle = query.column_int64(0);
    std::string new_value_bytes;
    query.column_blob_as_string(1, &new_value_bytes);
    sync_pb::EntitySpecifics new_value;
    new_value.ParseFromString(new_value_bytes);
    handler_function(&query, 2, &new_value);
    new_value.SerializeToString(&new_value_bytes);

    sqlite_utils::SQLStatement update;
    update.prepare(load_dbhandle_, update_sql.data(), update_sql.length());
    update.bind_blob(0, new_value_bytes.data(), new_value_bytes.length());
    update.bind_int64(1, metahandle);
    if (update.step() != SQLITE_DONE) {
      NOTREACHED();
      return false;
    }
  }
  return true;
}
