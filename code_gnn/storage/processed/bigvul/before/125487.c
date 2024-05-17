void GDataDirectoryService::AddEntryToResourceMap(GDataEntry* entry) {
  DVLOG(1) << "AddEntryToResourceMap " << entry->resource_id();
  resource_map_.insert(std::make_pair(entry->resource_id(), entry));
}
