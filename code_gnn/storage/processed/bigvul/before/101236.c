const std::string& SyncerProtoUtil::NameFromSyncEntity(
    const sync_pb::SyncEntity& entry) {
  if (entry.has_non_unique_name())
    return entry.non_unique_name();
  return entry.name();
}
