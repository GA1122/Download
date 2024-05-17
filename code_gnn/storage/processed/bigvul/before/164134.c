bool AppCacheDatabase::UpdateEvictionTimes(
    int64_t group_id,
    base::Time last_full_update_check_time,
    base::Time first_evictable_error_time) {
  if (!LazyOpen(kCreateIfNeeded))
    return false;

  static const char kSql[] =
      "UPDATE Groups"
      " SET last_full_update_check_time = ?, first_evictable_error_time = ?"
      " WHERE group_id = ?";
  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));
  statement.BindInt64(0, last_full_update_check_time.ToInternalValue());
  statement.BindInt64(1, first_evictable_error_time.ToInternalValue());
  statement.BindInt64(2, group_id);
  return statement.Run();   
}
