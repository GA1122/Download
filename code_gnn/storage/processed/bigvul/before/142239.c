  void Unmount(Profile* profile) {
    VolumeManager::Get(profile)->RemoveVolumeForTesting(
        root_path(), volume_type_, device_type_, read_only_);
  }
