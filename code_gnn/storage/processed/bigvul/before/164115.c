bool AppCacheDatabase::GetDeletableResponseIds(
    std::vector<int64_t>* response_ids,
    int64_t max_rowid,
    int limit) {
  if (!LazyOpen(kDontCreate))
    return false;

  static const char kSql[] =
      "SELECT response_id FROM DeletableResponseIds "
      "  WHERE rowid <= ?"
      "  LIMIT ?";

  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));
  statement.BindInt64(0, max_rowid);
  statement.BindInt64(1, limit);

  while (statement.Step())
    response_ids->push_back(statement.ColumnInt64(0));
  return statement.Succeeded();
}
