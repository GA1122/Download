  void OnTimedOut() {
    DCHECK_CALLED_ON_VALID_SEQUENCE(host_->sequence_checker_);
    host_->timer_->Stop();
    drivefs_has_mounted_ = false;
    drivefs_has_terminated_ = true;
    host_->mount_observer_->OnMountFailed(MountObserver::MountFailure::kTimeout,
                                          {});
  }
