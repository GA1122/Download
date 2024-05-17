void DownloadItemImpl::SetExternalData(
    const void* key, DownloadItem::ExternalData* data) {
  std::map<const void*, ExternalData*>::iterator it =
      external_data_map_.find(key);

  if (it == external_data_map_.end()) {
    external_data_map_[key] = data;
  } else if (it->second != data) {
    delete it->second;
    it->second = data;
  }
}
