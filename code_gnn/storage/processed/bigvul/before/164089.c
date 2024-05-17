bool AppCacheDatabase::CommitLazyLastAccessTimes() {
  if (lazy_last_access_times_.empty())
    return true;
  if (!LazyOpen(kDontCreate))
    return false;

  sql::Transaction transaction(db_.get());
  if (!transaction.Begin())
    return false;
  for (const auto& pair : lazy_last_access_times_) {
    static const char kSql[] =
        "UPDATE Groups SET last_access_time = ? WHERE group_id = ?";
    sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));
    statement.BindInt64(0, pair.second.ToInternalValue());   
    statement.BindInt64(1, pair.first);   
    statement.Run();
  }
  lazy_last_access_times_.clear();
  return transaction.Commit();
}
