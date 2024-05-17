  FilePath GetDatabasePath() {
    return temp_dir_.path().Append(
        DirectoryManager::GetSyncDataDatabaseFilename());
  }
