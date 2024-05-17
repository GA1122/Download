void ScreenRecorder::DoFinishOneRecording() {
  DCHECK_EQ(capture_loop_, MessageLoop::current());

  if (!is_recording_)
    return;

  --recordings_;
  DCHECK_GE(recordings_, 0);

  if (frame_skipped_)
    DoCapture();
}
