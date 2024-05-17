GDataEntry* GDataRootDirectory::GetEntryByResourceId(
    const std::string& resource) {
  ResourceMap::const_iterator iter = resource_map_.find(resource);
  if (iter == resource_map_.end())
    return NULL;
  return iter->second;
}
