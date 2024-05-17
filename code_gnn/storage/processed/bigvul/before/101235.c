const std::string& SyncerProtoUtil::NameFromCommitEntryResponse(
    const CommitResponse_EntryResponse& entry) {
  if (entry.has_non_unique_name())
    return entry.non_unique_name();
  return entry.name();
}
