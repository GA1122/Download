base::TimeDelta AudioRendererAlgorithm::GetTime() {
  return audio_buffer_.current_time();
}
