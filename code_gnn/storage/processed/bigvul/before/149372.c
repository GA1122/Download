CheckClientDownloadRequest::CheckClientDownloadRequest(
    download::DownloadItem* item,
    CheckDownloadRepeatingCallback callback,
    DownloadProtectionService* service,
    scoped_refptr<SafeBrowsingDatabaseManager> database_manager,
    scoped_refptr<BinaryFeatureExtractor> binary_feature_extractor)
    : CheckClientDownloadRequestBase(
          item->GetURL(),
          item->GetTargetFilePath(),
          item->GetFullPath(),
          {item->GetTabUrl(), item->GetTabReferrerUrl()},
