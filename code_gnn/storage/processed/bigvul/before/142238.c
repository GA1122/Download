  void Unmount(Profile* profile) {
    VolumeManager::Get(profile)->RemoveAndroidFilesDirectoryForTesting(
        root_path());
  }
