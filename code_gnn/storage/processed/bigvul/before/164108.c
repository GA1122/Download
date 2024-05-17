bool AppCacheDatabase::FindNamespacesForCache(
    int64_t cache_id,
    std::vector<NamespaceRecord>* intercepts,
    std::vector<NamespaceRecord>* fallbacks) {
  DCHECK(intercepts && intercepts->empty());
  DCHECK(fallbacks && fallbacks->empty());
  if (!LazyOpen(kDontCreate))
    return false;

  static const char kSql[] =
      "SELECT cache_id, origin, type, namespace_url, target_url, is_pattern"
      "  FROM Namespaces WHERE cache_id = ?";

  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));
  statement.BindInt64(0, cache_id);

  ReadNamespaceRecords(&statement, intercepts, fallbacks);

  return statement.Succeeded();
}
