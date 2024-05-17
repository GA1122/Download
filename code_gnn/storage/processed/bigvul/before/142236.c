  void Unmount(Profile* profile) {
    VolumeManager::Get(profile)->RemoveVolumeForTesting(
        root_path(), volume_type_, device_type_, read_only_, device_path_,
        drive_label_);
  }
