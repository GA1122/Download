void AudioRendererAlgorithm::CopyWithoutAdvance(
    uint8* dest, int offset) {
  if (muted_) {
    memset(dest, 0, bytes_per_frame_);
    return;
  }
  int copied = audio_buffer_.Peek(dest, bytes_per_frame_, offset);
  DCHECK_EQ(bytes_per_frame_, copied);
}
