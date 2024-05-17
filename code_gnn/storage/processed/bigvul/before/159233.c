DownloadItemImpl* DownloadManagerImpl::CreateActiveItem(
    uint32_t id,
    const DownloadCreateInfo& info) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!base::ContainsKey(downloads_, id));
  DownloadItemImpl* download = item_factory_->CreateActiveItem(this, id, info);
  downloads_[id] = base::WrapUnique(download);
  downloads_by_guid_[download->GetGuid()] = download;
  return download;
}
