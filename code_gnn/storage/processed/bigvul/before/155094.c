bool IsEqualForTesting(const SendTabToSelfEntry& a,
                       const SendTabToSelfEntry& b) {
  return a.GetGUID() == b.GetGUID() && a.GetURL() == b.GetURL() &&
         a.GetTitle() == b.GetTitle() &&
         a.GetDeviceName() == b.GetDeviceName() &&
         a.GetTargetDeviceSyncCacheGuid() == b.GetTargetDeviceSyncCacheGuid() &&
         a.GetSharedTime() == b.GetSharedTime() &&
         a.GetOriginalNavigationTime() == b.GetOriginalNavigationTime();
}
