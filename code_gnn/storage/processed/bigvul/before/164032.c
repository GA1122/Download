download::DownloadItem* DownloadManagerImpl::GetDownloadByGuid(
    const std::string& guid) {
  return base::ContainsKey(downloads_by_guid_, guid) ? downloads_by_guid_[guid]
                                                     : nullptr;
}
