void GDataDirectoryService::GetEntryByResourceIdAsync(
    const std::string& resource_id,
    const GetEntryByResourceIdCallback& callback) {
  GDataEntry* entry = GetEntryByResourceId(resource_id);
  callback.Run(entry);
}
