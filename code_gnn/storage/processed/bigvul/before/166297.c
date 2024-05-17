VideoCaptureManager::VideoCaptureManager(
    std::unique_ptr<VideoCaptureProvider> video_capture_provider,
    base::RepeatingCallback<void(const std::string&)> emit_log_message_cb,
    ScreenlockMonitor* monitor)
    : new_capture_session_id_(1),
      video_capture_provider_(std::move(video_capture_provider)),
      emit_log_message_cb_(std::move(emit_log_message_cb)),
      screenlock_monitor_(monitor) {
  if (screenlock_monitor_) {
    screenlock_monitor_->AddObserver(this);
  }
 }
