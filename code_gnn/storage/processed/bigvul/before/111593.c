  bool RemoveEntry(const FilePath& file_path) {
    return file_system_->RemoveEntryFromFileSystem(file_path) ==
        base::PLATFORM_FILE_OK;
  }
