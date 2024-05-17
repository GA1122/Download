const string& ProcessCommitResponseCommand::GetResultingPostCommitName(
    const sync_pb::SyncEntity& committed_entry,
    const CommitResponse_EntryResponse& entry_response) {
  const string& response_name =
      SyncerProtoUtil::NameFromCommitEntryResponse(entry_response);
  if (!response_name.empty())
    return response_name;
  return SyncerProtoUtil::NameFromSyncEntity(committed_entry);
}
