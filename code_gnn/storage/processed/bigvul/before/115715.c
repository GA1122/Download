void ScreenRecorder::FrameSentCallback(VideoPacket* packet) {
  delete packet;

  if (network_stopped_)
    return;

  capture_loop_->PostTask(
       FROM_HERE, base::Bind(&ScreenRecorder::DoFinishOneRecording, this));
 }
