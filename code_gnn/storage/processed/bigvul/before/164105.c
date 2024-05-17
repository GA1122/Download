bool AppCacheDatabase::FindGroupForManifestUrl(
    const GURL& manifest_url, GroupRecord* record) {
  DCHECK(record);
  if (!LazyOpen(kDontCreate))
    return false;

  static const char kSql[] =
      "SELECT group_id, origin, manifest_url,"
      "       creation_time, last_access_time,"
      "       last_full_update_check_time,"
      "       first_evictable_error_time"
      "  FROM Groups WHERE manifest_url = ?";

  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));
  statement.BindString(0, manifest_url.spec());

  if (!statement.Step())
    return false;

  ReadGroupRecord(statement, record);
  DCHECK(record->manifest_url == manifest_url);
  return true;
}
