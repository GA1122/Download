int DownloadManagerImpl::NonMaliciousInProgressCount() const {
  int count = 0;
  for (const auto& it : downloads_) {
    if (it.second->GetState() == download::DownloadItem::IN_PROGRESS &&
        it.second->GetDangerType() !=
            download::DOWNLOAD_DANGER_TYPE_DANGEROUS_URL &&
        it.second->GetDangerType() !=
            download::DOWNLOAD_DANGER_TYPE_DANGEROUS_CONTENT &&
        it.second->GetDangerType() !=
            download::DOWNLOAD_DANGER_TYPE_DANGEROUS_HOST &&
        it.second->GetDangerType() !=
            download::DOWNLOAD_DANGER_TYPE_POTENTIALLY_UNWANTED) {
      ++count;
    }
  }
  return count;
}
