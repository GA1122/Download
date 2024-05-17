bool CheckClientDownloadRequest::ShouldUploadBinary(
    DownloadCheckResult result) {
  if (!base::FeatureList::IsEnabled(kUploadForMalwareCheck))
    return false;

  if (policy::BrowserDMTokenStorage::Get()->RetrieveDMToken().empty())
    return false;

  if (result != DownloadCheckResult::SAFE &&
      result != DownloadCheckResult::UNCOMMON &&
      result != DownloadCheckResult::UNKNOWN)
    return false;


  return true;
}
