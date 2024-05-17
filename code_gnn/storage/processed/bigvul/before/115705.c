void ScreenRecorder::CaptureDoneCallback(
    scoped_refptr<CaptureData> capture_data) {
  DCHECK_EQ(capture_loop_, MessageLoop::current());

  if (!is_recording_)
    return;

  if (capture_data) {
    base::TimeDelta capture_time = base::Time::Now() - capture_start_time_;
    int capture_time_ms =
        static_cast<int>(capture_time.InMilliseconds());
    capture_data->set_capture_time_ms(capture_time_ms);
    scheduler_.RecordCaptureTime(capture_time);

    capture_data->set_client_sequence_number(sequence_number_);
  }

  encode_loop_->PostTask(
      FROM_HERE, base::Bind(&ScreenRecorder::DoEncode, this, capture_data));
}
