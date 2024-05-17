bool AppCacheDatabase::FindOriginsWithGroups(std::set<url::Origin>* origins) {
  DCHECK(origins && origins->empty());
  if (!LazyOpen(kDontCreate))
    return false;

  static const char kSql[] = "SELECT DISTINCT(origin) FROM Groups";

  sql::Statement statement(db_->GetUniqueStatement(kSql));

  while (statement.Step())
    origins->insert(url::Origin::Create(GURL(statement.ColumnString(0))));

  return statement.Succeeded();
}
