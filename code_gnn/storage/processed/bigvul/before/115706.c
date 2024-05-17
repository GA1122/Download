void ScreenRecorder::DoCapture() {
  DCHECK_EQ(capture_loop_, MessageLoop::current());
  if (recordings_ >= max_recordings_ || !is_recording_) {
    frame_skipped_ = true;
    return;
  }

  if (frame_skipped_)
    frame_skipped_ = false;

  ++recordings_;
  DCHECK_LE(recordings_, max_recordings_);

  capture_timer_.Stop();
  capture_timer_.Start(FROM_HERE,
                       scheduler_.NextCaptureDelay(),
                       this,
                       &ScreenRecorder::DoCapture);

  capture_start_time_ = base::Time::Now();
  capturer()->CaptureInvalidRegion(
      base::Bind(&ScreenRecorder::CaptureDoneCallback, this));
}
