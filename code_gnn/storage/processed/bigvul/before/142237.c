  void Unmount(Profile* profile) {
    auto* volume = VolumeManager::Get(profile);
    volume->RemoveDownloadsDirectoryForTesting();
  }
