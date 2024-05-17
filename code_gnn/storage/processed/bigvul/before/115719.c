void ScreenRecorder::Stop(const base::Closure& done_task) {
  if (MessageLoop::current() != capture_loop_) {
    capture_loop_->PostTask(FROM_HERE, base::Bind(
        &ScreenRecorder::Stop, this, done_task));
    return;
  }

  DCHECK(!done_task.is_null());

  capture_timer_.Stop();
  is_recording_ = false;

  network_loop_->PostTask(FROM_HERE, base::Bind(
      &ScreenRecorder::DoStopOnNetworkThread, this, done_task));
}
