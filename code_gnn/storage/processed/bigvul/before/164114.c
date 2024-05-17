bool AppCacheDatabase::GetAllOriginUsage(
    std::map<url::Origin, int64_t>* usage_map) {
  std::set<url::Origin> origins;
  if (!FindOriginsWithGroups(&origins))
    return false;
  for (const auto& origin : origins)
    (*usage_map)[origin] = GetOriginUsage(origin);
  return true;
}
