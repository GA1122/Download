void AddEntryToSearchResults(
    std::vector<SearchResultInfo>* results,
    const SearchCallback& callback,
    const base::Closure& entry_skipped_callback,
    GDataFileError error,
    bool run_callback,
    const GURL& next_feed,
    GDataEntry* entry) {
  if (entry) {
    const bool is_directory = entry->AsGDataDirectory() != NULL;
    results->push_back(SearchResultInfo(entry->GetFilePath(), is_directory));
  } else {
    if (!entry_skipped_callback.is_null())
      entry_skipped_callback.Run();
  }

  if (run_callback) {
    scoped_ptr<std::vector<SearchResultInfo> > result_vec(results);
    if (!callback.is_null())
      callback.Run(error, next_feed, result_vec.Pass());
  }
}
