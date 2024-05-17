DriveFsHost::~DriveFsHost() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  chromeos::disks::DiskMountManager::GetInstance()->RemoveObserver(this);
}
