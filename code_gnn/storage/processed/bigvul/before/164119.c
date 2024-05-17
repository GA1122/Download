bool AppCacheDatabase::InsertNamespace(
    const NamespaceRecord* record) {
  if (!LazyOpen(kCreateIfNeeded))
    return false;

  static const char kSql[] =
      "INSERT INTO Namespaces"
      "  (cache_id, origin, type, namespace_url, target_url, is_pattern)"
      "  VALUES (?, ?, ?, ?, ?, ?)";

  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));
  statement.BindInt64(0, record->cache_id);
  statement.BindString(1, SerializeOrigin(record->origin));
  statement.BindInt(2, record->namespace_.type);
  statement.BindString(3, record->namespace_.namespace_url.spec());
  statement.BindString(4, record->namespace_.target_url.spec());
  statement.BindBool(5, record->namespace_.is_pattern);
  return statement.Run();
}
