bool CheckClientDownloadRequest::IsSupportedDownload(
    DownloadCheckResultReason* reason,
    ClientDownloadRequest::DownloadType* type) {
  return IsSupportedDownload(*item_, item_->GetTargetFilePath(), reason, type);
}
