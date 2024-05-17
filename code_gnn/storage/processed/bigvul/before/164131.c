bool AppCacheDatabase::RunCachedStatementWithIds(
    sql::StatementID statement_id,
    const char* sql,
    const std::vector<int64_t>& ids) {
  DCHECK(sql);
  if (!LazyOpen(kCreateIfNeeded))
    return false;

  sql::Transaction transaction(db_.get());
  if (!transaction.Begin())
    return false;

  sql::Statement statement(db_->GetCachedStatement(statement_id, sql));

  for (const auto& id : ids) {
    statement.BindInt64(0, id);
    if (!statement.Run())
      return false;
    statement.Reset(true);
  }

  return transaction.Commit();
}
