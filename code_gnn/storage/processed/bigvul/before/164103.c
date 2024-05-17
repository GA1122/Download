bool AppCacheDatabase::FindGroup(int64_t group_id, GroupRecord* record) {
  DCHECK(record);
  if (!LazyOpen(kDontCreate))
    return false;

  static const char kSql[] =
      "SELECT group_id, origin, manifest_url,"
      "       creation_time, last_access_time,"
      "       last_full_update_check_time,"
      "       first_evictable_error_time"
      "  FROM Groups WHERE group_id = ?";

  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));

  statement.BindInt64(0, group_id);
  if (!statement.Step())
    return false;

  ReadGroupRecord(statement, record);
  DCHECK(record->group_id == group_id);
  return true;
}
