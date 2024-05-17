  bool AppCache::AddOrModifyEntry(const GURL& url, const AppCacheEntry& entry) {
    std::pair<EntryMap::iterator, bool> ret =
        entries_.insert(EntryMap::value_type(url, entry));
  
  if (!ret.second)
//   if (!ret.second) {
      ret.first->second.add_types(entry.types());
  else
//   } else {
      cache_size_ += entry.response_size();   
//     padding_size_ += entry.padding_size();
//   }
    return ret.second;
  }