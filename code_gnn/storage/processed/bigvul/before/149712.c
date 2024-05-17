void SetFeatureEntries(const std::vector<FeatureEntry>& entries) {
  GetEntriesForTesting()->clear();
  for (const auto& entry : entries)
    GetEntriesForTesting()->push_back(entry);
  FlagsStateSingleton::GetInstance()->RebuildState(*GetEntriesForTesting());
}
