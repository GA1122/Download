void AudioRendererAlgorithm::FlushBuffers() {
  ResetWindow();

  audio_buffer_.Clear();
  request_read_cb_.Run();
}
