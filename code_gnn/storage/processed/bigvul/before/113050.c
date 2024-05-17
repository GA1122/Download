DownloadPersistentStoreInfo DownloadItemImpl::GetPersistentStoreInfo() const {
  return DownloadPersistentStoreInfo(GetFullPath(),
                                     GetURL(),
                                     GetReferrerUrl(),
                                     GetStartTime(),
                                     GetEndTime(),
                                     GetReceivedBytes(),
                                     GetTotalBytes(),
                                     GetState(),
                                     GetDbHandle(),
                                     GetOpened());
}
