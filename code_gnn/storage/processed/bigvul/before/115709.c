void ScreenRecorder::DoInvalidateFullScreen() {
  DCHECK_EQ(capture_loop_, MessageLoop::current());

  capturer_->InvalidateFullScreen();
}
