void DriveFsHost::OnMountEvent(
    chromeos::disks::DiskMountManager::MountEvent event,
    chromeos::MountError error_code,
    const chromeos::disks::DiskMountManager::MountPointInfo& mount_info) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!mount_state_) {
    return;
  }
  if (!mount_state_->OnMountEvent(event, error_code, mount_info)) {
    Unmount();
  }
}
