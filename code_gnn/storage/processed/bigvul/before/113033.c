DownloadItemImpl::GetExternalData(const void* key) {
  return const_cast<DownloadItem::ExternalData*>(
      static_cast<const DownloadItemImpl&>(*this).GetExternalData(key));
}
