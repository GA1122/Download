bool ShouldUseSiteProcessTracking(BrowserContext* browser_context,
                                  StoragePartition* dest_partition,
                                  const GURL& site_url) {
  StoragePartition* default_partition =
      BrowserContext::GetDefaultStoragePartition(browser_context);
  if (dest_partition != default_partition)
    return false;

  return true;
}
