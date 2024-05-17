bool AppCacheDatabase::FindResponseIdsForCacheHelper(
    int64_t cache_id,
    std::vector<int64_t>* ids_vector,
    std::set<int64_t>* ids_set) {
  DCHECK(ids_vector || ids_set);
  DCHECK(!(ids_vector && ids_set));
  if (!LazyOpen(kDontCreate))
    return false;

  static const char kSql[] =
      "SELECT response_id FROM Entries WHERE cache_id = ?";

  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));

  statement.BindInt64(0, cache_id);
  while (statement.Step()) {
    int64_t id = statement.ColumnInt64(0);
    if (ids_set)
      ids_set->insert(id);
    else
      ids_vector->push_back(id);
  }

  return statement.Succeeded();
}
