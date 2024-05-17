bool RemoveFilter(const std::string& event_name,
                  const std::string& extension_id,
                  base::DictionaryValue* filter) {
  FilteredEventListenerKey key(extension_id, event_name);
  FilteredEventListenerCounts& all_counts = g_filtered_listener_counts.Get();
  FilteredEventListenerCounts::const_iterator counts = all_counts.find(key);
  if (counts == all_counts.end())
    return false;
  if (counts->second->Remove(*filter)) {
    if (counts->second->is_empty())
      all_counts.erase(counts);   
    return true;
  }
  return false;
}
