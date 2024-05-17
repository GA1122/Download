bool AppCacheDatabase::FindCachesForOrigin(const url::Origin& origin,
                                           std::vector<CacheRecord>* records) {
  DCHECK(records);
  std::vector<GroupRecord> group_records;
  if (!FindGroupsForOrigin(origin, &group_records))
    return false;

  CacheRecord cache_record;
  for (const auto& record : group_records) {
    if (FindCacheForGroup(record.group_id, &cache_record))
      records->push_back(cache_record);
  }
  return true;
}
