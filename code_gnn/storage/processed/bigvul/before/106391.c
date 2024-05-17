int PluginSelectionPolicy::FindFirstAllowed(
    const GURL& url,
    const std::vector<webkit::npapi::WebPluginInfo>& info) {
  for (std::vector<webkit::npapi::WebPluginInfo>::size_type i = 0;
       i < info.size(); ++i) {
    if (IsAllowed(url, info[i].path))
      return i;
  }
  return -1;
}
