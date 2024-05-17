void PepperVideoRenderer3D::OnPictureReady(int32_t result,
                                           PP_VideoPicture picture) {
  DCHECK(get_picture_pending_);
  get_picture_pending_ = false;

  if (result != PP_OK) {
    LOG(ERROR) << "VideoDecoder::GetPicture() returned " << result;
    event_handler_->OnVideoDecodeError();
    return;
  }

  CHECK(!frame_decode_timestamps_.empty());
  const FrameDecodeTimestamp& frame_timer = frame_decode_timestamps_.front();

  if (picture.decode_id != frame_timer.frame_id) {
    LOG(ERROR)
        << "Received a video packet that didn't contain a complete frame.";
    event_handler_->OnVideoDecodeError();
    return;
  }

  base::TimeDelta decode_time =
      base::TimeTicks::Now() - frame_timer.decode_started_time;
  stats_.video_decode_ms()->Record(decode_time.InMilliseconds());
  frame_decode_timestamps_.pop_front();

  next_picture_.reset(new Picture(&video_decoder_, picture));

  PaintIfNeeded();
  GetNextPicture();
}
