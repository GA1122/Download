void ScreenRecorder::Start() {
  capture_loop_->PostTask(
      FROM_HERE, base::Bind(&ScreenRecorder::DoStart, this));
}
