void PepperVideoRenderer3D::DecodeNextPacket() {
  if (!initialization_finished_ || decode_pending_ || pending_packets_.empty())
    return;

  ++latest_frame_id_;
  frame_decode_timestamps_.push_back(
      FrameDecodeTimestamp(latest_frame_id_, base::TimeTicks::Now()));

  const VideoPacket* packet = pending_packets_.front()->packet();

  int32_t result = video_decoder_.Decode(
      latest_frame_id_, packet->data().size(), packet->data().data(),
      callback_factory_.NewCallback(&PepperVideoRenderer3D::OnDecodeDone));
  CHECK_EQ(result, PP_OK_COMPLETIONPENDING);
  decode_pending_ = true;
}
