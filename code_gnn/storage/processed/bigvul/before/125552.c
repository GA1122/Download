void GetChildDirectoryPaths(GDataEntry* entry,
                            std::set<FilePath>* changed_dirs) {
  GDataDirectory* dir = entry->AsGDataDirectory();
  if (!dir)
    return;

  for (GDataDirectoryCollection::const_iterator it =
       dir->child_directories().begin();
       it != dir->child_directories().end(); ++it) {
    GDataDirectory* child_dir = it->second;
    changed_dirs->insert(child_dir->GetFilePath());
    GetChildDirectoryPaths(child_dir, changed_dirs);
  }
}
