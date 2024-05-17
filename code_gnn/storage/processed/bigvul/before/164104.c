bool AppCacheDatabase::FindGroupForCache(int64_t cache_id,
                                         GroupRecord* record) {
  DCHECK(record);
  if (!LazyOpen(kDontCreate))
    return false;

  static const char kSql[] =
      "SELECT g.group_id, g.origin, g.manifest_url,"
      "       g.creation_time, g.last_access_time,"
      "       g.last_full_update_check_time,"
      "       g.first_evictable_error_time"
      "  FROM Groups g, Caches c"
      "  WHERE c.cache_id = ? AND c.group_id = g.group_id";

  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));
  statement.BindInt64(0, cache_id);

  if (!statement.Step())
    return false;

  ReadGroupRecord(statement, record);
  return true;
}
