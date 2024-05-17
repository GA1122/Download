void GDataWapiFeedProcessor::AddEntryToDirectoryAndCollectChangedDirectories(
    GDataEntry* entry,
    GDataDirectory* directory,
    GDataDirectoryService* orphaned_dir_service,
    std::set<FilePath>* changed_dirs) {
  directory->AddEntry(entry);
  if (entry->AsGDataDirectory() && directory != orphaned_dir_service->root())
    changed_dirs->insert(entry->GetFilePath());
}
