bool AudioRendererAlgorithm::IsQueueFull() {
  return audio_buffer_.forward_bytes() >= audio_buffer_.forward_capacity();
}
