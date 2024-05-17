double EglRenderingVDAClient::frames_per_second() {
  base::TimeDelta delta = last_frame_delivered_ticks_ - initialize_done_ticks_;
  if (delta.InSecondsF() == 0)
    return 0;
  return num_decoded_frames_ / delta.InSecondsF();
}
