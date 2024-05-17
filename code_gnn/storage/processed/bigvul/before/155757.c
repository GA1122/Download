  void OnConnectionError() {
    DCHECK_CALLED_ON_VALID_SEQUENCE(host_->sequence_checker_);
    if (!drivefs_has_terminated_) {
      if (mounted()) {
        host_->mount_observer_->OnUnmounted({});
      } else {
        host_->mount_observer_->OnMountFailed(
            MountObserver::MountFailure::kIpcDisconnect, {});
      }
      drivefs_has_terminated_ = true;
    }
  }
