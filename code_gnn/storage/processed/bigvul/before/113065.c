FilePath DownloadItemImpl::GetUserVerifiedFilePath() const {
  return (safety_state_ == DownloadItem::SAFE) ?
      GetTargetFilePath() : GetFullPath();
}
