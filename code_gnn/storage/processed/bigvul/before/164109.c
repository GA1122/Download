bool AppCacheDatabase::FindNamespacesForOrigin(
    const url::Origin& origin,
    std::vector<NamespaceRecord>* intercepts,
    std::vector<NamespaceRecord>* fallbacks) {
  DCHECK(intercepts && intercepts->empty());
  DCHECK(fallbacks && fallbacks->empty());
  if (!LazyOpen(kDontCreate))
    return false;

  static const char kSql[] =
      "SELECT cache_id, origin, type, namespace_url, target_url, is_pattern"
      "  FROM Namespaces WHERE origin = ?";

  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));
  statement.BindString(0, SerializeOrigin(origin));

  ReadNamespaceRecords(&statement, intercepts, fallbacks);

  return statement.Succeeded();
}
