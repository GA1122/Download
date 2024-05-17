void GDataDirectoryService::RemoveEntryFromResourceMap(GDataEntry* entry) {
  resource_map_.erase(entry->resource_id());
}
