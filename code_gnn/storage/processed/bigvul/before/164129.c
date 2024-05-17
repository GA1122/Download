void AppCacheDatabase::ReadOnlineWhiteListRecord(
    const sql::Statement& statement, OnlineWhiteListRecord* record) {
  record->cache_id = statement.ColumnInt64(0);
  record->namespace_url = GURL(statement.ColumnString(1));
  record->is_pattern = statement.ColumnBool(2);
}
