size_t NavigationController::max_entry_count() {
  if (max_entry_count_for_testing_ != kMaxEntryCountForTestingNotSet)
     return max_entry_count_for_testing_;
  return content::kMaxSessionHistoryEntries;
}
