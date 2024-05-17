GDataEntry* GDataDirectoryService::FindEntryByPathSync(
    const FilePath& file_path) {
  if (file_path == root_->GetFilePath())
    return root_.get();

  std::vector<FilePath::StringType> components;
  file_path.GetComponents(&components);
  GDataDirectory* current_dir = root_.get();

  for (size_t i = 1; i < components.size() && current_dir; ++i) {
    GDataEntry* entry = current_dir->FindChild(components[i]);
    if (!entry)
      return NULL;

    if (i == components.size() - 1)   
      return entry;
    else
      current_dir = entry->AsGDataDirectory();
  }
  return NULL;
}
