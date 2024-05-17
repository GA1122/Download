  void MaybeNotifyDelegateOnMounted() {
    if (mounted()) {
      host_->timer_->Stop();
      host_->mount_observer_->OnMounted(mount_path());
    }
  }
