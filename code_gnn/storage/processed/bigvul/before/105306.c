bool AutofillDownloadManager::CheckCacheForQueryRequest(
    const std::vector<std::string>& forms_in_query,
    std::string* query_data) const {
  std::string signature = GetCombinedSignature(forms_in_query);
  for (QueryRequestCache::const_iterator it = cached_forms_.begin();
       it != cached_forms_.end(); ++it) {
    if (it->first == signature) {
      *query_data = it->second;
      return true;
    }
  }
  return false;
}
