  GDataEntry* FindEntry(const FilePath& file_path) {
    GDataEntry* entry = NULL;
    file_system_->root_->FindEntryByPath(
        file_path, base::Bind(&ReadOnlyFindEntryCallback, &entry));
    return entry;
  }
