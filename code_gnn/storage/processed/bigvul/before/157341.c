void WebMediaPlayerImpl::UpdateBackgroundVideoOptimizationState() {
  if (IsHidden()) {
    if (ShouldPauseVideoWhenHidden()) {
      PauseVideoIfNeeded();
    } else if (update_background_status_cb_.IsCancelled()) {
      update_background_status_cb_.Reset(
          base::Bind(&WebMediaPlayerImpl::DisableVideoTrackIfNeeded,
                     base::Unretained(this)));

      main_task_runner_->PostDelayedTask(
          FROM_HERE, update_background_status_cb_.callback(),
          base::TimeDelta::FromSeconds(10));
    }
  } else {
    update_background_status_cb_.Cancel();
    EnableVideoTrackIfNeeded();
  }
}
