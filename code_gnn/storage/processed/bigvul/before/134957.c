  bool OnMountEvent(
      chromeos::disks::DiskMountManager::MountEvent event,
      chromeos::MountError error_code,
      const chromeos::disks::DiskMountManager::MountPointInfo& mount_info) {
    DCHECK_CALLED_ON_VALID_SEQUENCE(host_->sequence_checker_);
    if (mount_info.mount_type != chromeos::MOUNT_TYPE_NETWORK_STORAGE ||
        mount_info.source_path != source_path_ ||
        event != chromeos::disks::DiskMountManager::MOUNTING) {
      return true;
    }
    if (error_code != chromeos::MOUNT_ERROR_NONE) {
      return false;
    }
    mount_path_ = base::FilePath(mount_info.mount_path);
    DCHECK(!mount_info.mount_path.empty());
    if (mounted()) {
      NotifyDelegateOnMounted();
    }
    return true;
  }
