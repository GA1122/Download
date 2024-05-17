bool AppCacheDatabase::FindGroupsForOrigin(const url::Origin& origin,
                                           std::vector<GroupRecord>* records) {
  DCHECK(records && records->empty());
  if (!LazyOpen(kDontCreate))
    return false;

  static const char kSql[] =
      "SELECT group_id, origin, manifest_url,"
      "       creation_time, last_access_time,"
      "       last_full_update_check_time,"
      "       first_evictable_error_time"
      "   FROM Groups WHERE origin = ?";

  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));
  statement.BindString(0, SerializeOrigin(origin));

  while (statement.Step()) {
    records->push_back(GroupRecord());
    ReadGroupRecord(statement, &records->back());
    DCHECK(records->back().origin == origin);
  }

  return statement.Succeeded();
}
