void AudioRendererAlgorithm::ResetWindow() {
  DCHECK_LE(index_into_window_, window_size_);
  index_into_window_ = 0;
   crossfade_frame_number_ = 0;
 }
