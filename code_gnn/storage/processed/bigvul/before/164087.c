bool AppCacheDatabase::AddEntryFlags(const GURL& entry_url,
                                     int64_t cache_id,
                                     int additional_flags) {
  if (!LazyOpen(kDontCreate))
    return false;

  static const char kSql[] =
      "UPDATE Entries SET flags = flags | ? WHERE cache_id = ? AND url = ?";

  sql::Statement statement(db_->GetCachedStatement(SQL_FROM_HERE, kSql));
  statement.BindInt(0, additional_flags);
  statement.BindInt64(1, cache_id);
  statement.BindString(2, entry_url.spec());

  return statement.Run() && db_->GetLastChangeCount();
}
