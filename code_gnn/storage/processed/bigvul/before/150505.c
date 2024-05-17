CheckClientDownloadRequest::CheckClientDownloadRequest(
    download::DownloadItem* item,
    CheckDownloadCallback callback,
    DownloadProtectionService* service,
    scoped_refptr<SafeBrowsingDatabaseManager> database_manager,
    scoped_refptr<BinaryFeatureExtractor> binary_feature_extractor)
    : CheckClientDownloadRequestBase(
          item->GetURL(),
          item->GetTargetFilePath(),
          item->GetFullPath(),
          item->GetTabUrl(),
          item->GetTabReferrerUrl(),
          content::DownloadItemUtils::GetBrowserContext(item),
          std::move(callback),
          service,
          std::move(database_manager),
          std::move(binary_feature_extractor)),
      item_(item) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  item_->AddObserver(this);
  DVLOG(2) << "Starting SafeBrowsing download check for: "
           << item_->DebugString(true);
}
