DownloadItemImpl::GetExternalData(const void* key) const {
  std::map<const void*, ExternalData*>::const_iterator it =
      external_data_map_.find(key);
  return (it == external_data_map_.end()) ? NULL : it->second;
}
