bool AddFilter(const std::string& event_name,
               const std::string& extension_id,
               const base::DictionaryValue& filter) {
  FilteredEventListenerKey key(extension_id, event_name);
  FilteredEventListenerCounts& all_counts = g_filtered_listener_counts.Get();
  FilteredEventListenerCounts::const_iterator counts = all_counts.find(key);
  if (counts == all_counts.end()) {
    counts =
        all_counts
            .insert(std::make_pair(key, base::WrapUnique(new ValueCounter())))
            .first;
  }
  return counts->second->Add(filter);
}
