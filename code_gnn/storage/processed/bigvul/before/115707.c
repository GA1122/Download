void ScreenRecorder::DoEncode(
    scoped_refptr<CaptureData> capture_data) {
  DCHECK_EQ(encode_loop_, MessageLoop::current());

  if (!capture_data || capture_data->dirty_region().isEmpty()) {
    VideoPacket* packet = new VideoPacket();
    packet->set_flags(VideoPacket::LAST_PARTITION);
    network_loop_->PostTask(
        FROM_HERE, base::Bind(&ScreenRecorder::DoSendVideoPacket,
                              this, packet));
    return;
  }

  encode_start_time_ = base::Time::Now();
  encoder()->Encode(
      capture_data, false,
      base::Bind(&ScreenRecorder::EncodedDataAvailableCallback, this));
}
