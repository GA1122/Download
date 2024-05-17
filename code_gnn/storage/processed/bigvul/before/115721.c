Capturer* ScreenRecorder::capturer() {
  DCHECK_EQ(capture_loop_, MessageLoop::current());
  DCHECK(capturer_);
  return capturer_;
}
