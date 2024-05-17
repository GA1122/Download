void AppCacheDatabase::ReadGroupRecord(
    const sql::Statement& statement, GroupRecord* record) {
  record->group_id = statement.ColumnInt64(0);
  record->origin = url::Origin::Create(GURL(statement.ColumnString(1)));
  record->manifest_url = GURL(statement.ColumnString(2));
  record->creation_time =
      base::Time::FromInternalValue(statement.ColumnInt64(3));

  const auto found = lazy_last_access_times_.find(record->group_id);
  if (found != lazy_last_access_times_.end()) {
    record->last_access_time = found->second;
  } else {
    record->last_access_time =
        base::Time::FromInternalValue(statement.ColumnInt64(4));
  }

  record->last_full_update_check_time =
      base::Time::FromInternalValue(statement.ColumnInt64(5));
  record->first_evictable_error_time =
      base::Time::FromInternalValue(statement.ColumnInt64(6));
}
