bool DriveFsHost::IsMounted() const {
  return mount_state_ && mount_state_->mounted();
}
