void VP9Decoder::Reset() {
  curr_frame_hdr_ = nullptr;
  for (auto& ref_frame : ref_frames_)
    ref_frame = nullptr;

  parser_.Reset();

  if (state_ == kDecoding)
    state_ = kAfterReset;
}
