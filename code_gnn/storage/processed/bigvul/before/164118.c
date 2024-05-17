bool AppCacheDatabase::InsertGroup(const GroupRecord* record) {
  if (!LazyOpen(kCreateIfNeeded))
    return false;

  static const char kSql[] =
      "INSERT INTO Groups"
      "  (group_id, origin, manifest_url, creation_time, last_access_time,"
      "   last_full_update_check_time, first_evictable_error_time)"
      "  VALUES(?, ?, ?, ?, ?, ?, ?)";
  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));
  statement.BindInt64(0, record->group_id);
  statement.BindString(1, SerializeOrigin(record->origin));
  statement.BindString(2, record->manifest_url.spec());
  statement.BindInt64(3, record->creation_time.ToInternalValue());
  statement.BindInt64(4, record->last_access_time.ToInternalValue());
  statement.BindInt64(5, record->last_full_update_check_time.ToInternalValue());
  statement.BindInt64(6, record->first_evictable_error_time.ToInternalValue());
  return statement.Run();
}
