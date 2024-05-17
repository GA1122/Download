void AudioRendererAlgorithm::DropFrame() {
  audio_buffer_.Seek(bytes_per_frame_);

  if (!IsQueueFull())
    request_read_cb_.Run();
}
