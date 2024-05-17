base::FilePath DriveFsHost::GetMountPath() const {
  return mount_state_ && mount_state_->mounted()
             ? mount_state_->mount_path()
             : base::FilePath("/media/fuse")
                   .Append(base::StrCat(
                       {"drivefs-", delegate_->GetObfuscatedAccountId()}));
}
