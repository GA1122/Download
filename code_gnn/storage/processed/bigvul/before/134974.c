void FakeCrosDisksClient::NotifyMountCompleted(MountError error_code,
                                               const std::string& source_path,
                                               MountType mount_type,
                                               const std::string& mount_path) {
  for (auto& observer : observer_list_) {
    observer.OnMountCompleted(
        MountEntry(error_code, source_path, mount_type, mount_path));
  }
}
