GDataEntry* GDataDirectoryService::GetEntryByResourceId(
    const std::string& resource) {
  ResourceMap::const_iterator iter = resource_map_.find(resource);
  return iter == resource_map_.end() ? NULL : iter->second;
}
