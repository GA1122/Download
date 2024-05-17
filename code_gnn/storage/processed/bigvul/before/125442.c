void GDataFileSystem::RemoveStaleEntryOnUpload(const std::string& resource_id,
                                               GDataDirectory* parent_dir,
                                               GDataEntry* existing_entry) {
  if (existing_entry &&
      existing_entry->parent() == parent_dir) {
    parent_dir->RemoveEntry(existing_entry);
  } else {
    LOG(ERROR) << "Entry for the existing file not found: " << resource_id;
  }
}
