bool IsEqualForTesting(const SendTabToSelfEntry& entry,
                       const sync_pb::SendTabToSelfSpecifics& specifics) {
  return (
      entry.GetGUID() == specifics.guid() &&
      entry.GetURL() == specifics.url() &&
      entry.GetTitle() == specifics.title() &&
      entry.GetDeviceName() == specifics.device_name() &&
      entry.GetTargetDeviceSyncCacheGuid() ==
          specifics.target_device_sync_cache_guid() &&
      specifics.shared_time_usec() ==
          entry.GetSharedTime().ToDeltaSinceWindowsEpoch().InMicroseconds() &&
      specifics.navigation_time_usec() == entry.GetOriginalNavigationTime()
                                              .ToDeltaSinceWindowsEpoch()
                                              .InMicroseconds());
}
