bool AudioRendererAlgorithm::CanFillBuffer() {
  return audio_buffer_.forward_bytes() > 0 && !needs_more_data_;
}
