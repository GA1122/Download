void ScreenRecorder::StartCaptureTimer() {
  DCHECK_EQ(capture_loop_, MessageLoop::current());

  capture_timer_.Start(FROM_HERE,
                       scheduler_.NextCaptureDelay(),
                       this,
                       &ScreenRecorder::DoCapture);
}
