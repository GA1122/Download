void ScreenRecorder::EncodedDataAvailableCallback(VideoPacket* packet) {
  DCHECK_EQ(encode_loop_, MessageLoop::current());

  if (encoder_stopped_)
    return;

  bool last = (packet->flags() & VideoPacket::LAST_PACKET) != 0;
  if (last) {
    base::TimeDelta encode_time = base::Time::Now() - encode_start_time_;
    int encode_time_ms =
        static_cast<int>(encode_time.InMilliseconds());
    packet->set_encode_time_ms(encode_time_ms);
    scheduler_.RecordEncodeTime(encode_time);
  }

  network_loop_->PostTask(
      FROM_HERE, base::Bind(&ScreenRecorder::DoSendVideoPacket, this, packet));
}
