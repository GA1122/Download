bool AppCacheDatabase::DeleteGroup(int64_t group_id) {
  if (!LazyOpen(kDontCreate))
    return false;

  static const char kSql[] = "DELETE FROM Groups WHERE group_id = ?";
  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));
  statement.BindInt64(0, group_id);
  return statement.Run();
}
