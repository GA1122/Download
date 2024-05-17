void FakeCrosDisksClient::SetCustomMountPointCallback(
    FakeCrosDisksClient::CustomMountPointCallback custom_mount_point_callback) {
  custom_mount_point_callback_ = std::move(custom_mount_point_callback);
}
