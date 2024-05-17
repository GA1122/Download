bool AppCacheDatabase::InsertOnlineWhiteList(
    const OnlineWhiteListRecord* record) {
  if (!LazyOpen(kCreateIfNeeded))
    return false;

  static const char kSql[] =
      "INSERT INTO OnlineWhiteLists (cache_id, namespace_url, is_pattern)"
      "  VALUES (?, ?, ?)";

  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));
  statement.BindInt64(0, record->cache_id);
  statement.BindString(1, record->namespace_url.spec());
  statement.BindBool(2, record->is_pattern);

  return statement.Run();
}
