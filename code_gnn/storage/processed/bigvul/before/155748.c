DriveFsHost::DriveFsHost(const base::FilePath& profile_path,
                         DriveFsHost::Delegate* delegate,
                         DriveFsHost::MountObserver* mount_observer,
                         const base::Clock* clock,
                         chromeos::disks::DiskMountManager* disk_mount_manager,
                         std::unique_ptr<base::OneShotTimer> timer)
    : profile_path_(profile_path),
      delegate_(delegate),
      mount_observer_(mount_observer),
      clock_(clock),
      disk_mount_manager_(disk_mount_manager),
      timer_(std::move(timer)),
      account_token_delegate_(std::make_unique<AccountTokenDelegate>(this)) {
  DCHECK(delegate_);
  DCHECK(mount_observer_);
  DCHECK(clock_);
}
