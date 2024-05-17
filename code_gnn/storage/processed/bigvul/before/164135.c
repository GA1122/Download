bool AppCacheDatabase::UpdateLastAccessTime(int64_t group_id, base::Time time) {
  if (!LazyUpdateLastAccessTime(group_id, time))
    return false;
  return CommitLazyLastAccessTimes();
}
