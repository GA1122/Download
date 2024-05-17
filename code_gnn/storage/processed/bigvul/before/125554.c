void GDataWapiFeedProcessor::UpdateFileCountUmaHistograms(
    const FeedToFileResourceMapUmaStats& uma_stats) const {
  const int num_total_files =
      uma_stats.num_hosted_documents + uma_stats.num_regular_files;
  UMA_HISTOGRAM_COUNTS("GData.NumberOfRegularFiles",
                       uma_stats.num_regular_files);
  UMA_HISTOGRAM_COUNTS("GData.NumberOfHostedDocuments",
                       uma_stats.num_hosted_documents);
  UMA_HISTOGRAM_COUNTS("GData.NumberOfTotalFiles", num_total_files);
  const std::vector<int> all_entry_kinds = DocumentEntry::GetAllEntryKinds();
  for (FeedToFileResourceMapUmaStats::EntryKindToCountMap::const_iterator iter =
           uma_stats.num_files_with_entry_kind.begin();
       iter != uma_stats.num_files_with_entry_kind.end();
       ++iter) {
    const DocumentEntry::EntryKind kind = iter->first;
    const int count = iter->second;
    for (int i = 0; i < count; ++i) {
      UMA_HISTOGRAM_CUSTOM_ENUMERATION(
          "GData.EntryKind", kind, all_entry_kinds);
    }
  }
}
