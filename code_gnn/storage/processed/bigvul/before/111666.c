std::string GDataRootDirectory::CacheEntry::ToString() const {
  std::vector<std::string> cache_states;
  if (GDataFile::IsCachePresent(cache_state))
    cache_states.push_back("present");
  if (GDataFile::IsCachePinned(cache_state))
    cache_states.push_back("pinned");
  if (GDataFile::IsCacheDirty(cache_state))
    cache_states.push_back("dirty");

  return base::StringPrintf("md5=%s, subdir=%s, cache_state=%s",
                            md5.c_str(),
                            CacheSubDirectoryTypeToString(sub_dir_type).c_str(),
                            JoinString(cache_states, ',').c_str());
}
