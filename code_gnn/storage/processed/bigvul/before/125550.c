GDataDirectory* GDataWapiFeedProcessor::FindDirectoryForNewEntry(
    GDataEntry* new_entry,
    const FileResourceIdMap& file_map,
    GDataDirectoryService* orphaned_dir_service) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  GDataDirectory* dir = NULL;
  const std::string& parent_id = new_entry->parent_resource_id();
  if (parent_id.empty()) {
    dir = directory_service_->root();
    DVLOG(1) << "Root parent for " << new_entry->base_name();
  } else {
    GDataEntry* entry = directory_service_->GetEntryByResourceId(parent_id);
    dir = entry ? entry->AsGDataDirectory() : NULL;
    if (!dir) {
      FileResourceIdMap::const_iterator find_iter =
          file_map.find(parent_id);
      dir = (find_iter != file_map.end() &&
             find_iter->second) ?
                find_iter->second->AsGDataDirectory() : NULL;
      if (dir) {
        DVLOG(1) << "Found parent for " << new_entry->base_name()
                 << " in file_map " << parent_id;
      } else {
        DVLOG(1) << "Adding orphan " << new_entry->GetFilePath().value();
        dir = orphaned_dir_service->root();
      }
    }
  }
  return dir;
}
