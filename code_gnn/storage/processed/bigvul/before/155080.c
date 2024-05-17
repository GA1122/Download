std::unique_ptr<SendTabToSelfEntry> SendTabToSelfEntry::FromProto(
    const sync_pb::SendTabToSelfSpecifics& pb_entry,
    base::Time now) {
  std::string guid(pb_entry.guid());
  if (guid.empty()) {
    return nullptr;
  }

  GURL url(pb_entry.url());

  if (!url.is_valid()) {
    return nullptr;
  }

  base::Time shared_time = ProtoTimeToTime(pb_entry.shared_time_usec());
  if (shared_time > now) {
    shared_time = now;
  }

  base::Time navigation_time;
  if (pb_entry.has_navigation_time_usec()) {
    navigation_time = ProtoTimeToTime(pb_entry.navigation_time_usec());
  }

  return std::make_unique<SendTabToSelfEntry>(
      guid, url, pb_entry.title(), shared_time, navigation_time,
      pb_entry.device_name(), pb_entry.target_device_sync_cache_guid());
}
