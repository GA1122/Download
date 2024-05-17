SendTabToSelfLocal SendTabToSelfEntry::AsLocalProto() const {
  SendTabToSelfLocal local_entry;
  auto* pb_entry = local_entry.mutable_specifics();

  pb_entry->set_guid(GetGUID());
  pb_entry->set_title(GetTitle());
  pb_entry->set_url(GetURL().spec());
  pb_entry->set_shared_time_usec(TimeToProtoTime(GetSharedTime()));
  pb_entry->set_navigation_time_usec(
      TimeToProtoTime(GetOriginalNavigationTime()));
  pb_entry->set_device_name(GetDeviceName());
  pb_entry->set_target_device_sync_cache_guid(GetTargetDeviceSyncCacheGuid());
  local_entry.set_notification_dismissed(GetNotificationDismissed());

  return local_entry;
}
