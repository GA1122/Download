 void ScreenRecorder::UpdateSequenceNumber(int64 sequence_number) {
  if (MessageLoop::current() != capture_loop_) {
    capture_loop_->PostTask(
        FROM_HERE, base::Bind(&ScreenRecorder::UpdateSequenceNumber,
                              this, sequence_number));
    return;
  }

  sequence_number_ = sequence_number;
}
