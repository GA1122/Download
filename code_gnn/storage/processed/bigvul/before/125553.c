RemoveEntryFromDirectoryAndCollectChangedDirectories(
    GDataDirectory* directory,
    GDataEntry* entry,
    std::set<FilePath>* changed_dirs) {
  GetChildDirectoryPaths(entry, changed_dirs);
  directory->RemoveEntry(entry);
}
