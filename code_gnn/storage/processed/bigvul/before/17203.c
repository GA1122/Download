base::FilePath BrowserContextIOData::GetCachePath() const {
  const BrowserContextSharedIOData& data = GetSharedData();
  if (data.cache_path.empty()) {
    return GetPath();
  }

  return data.cache_path;
}
