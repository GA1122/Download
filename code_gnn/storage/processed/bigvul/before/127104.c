void AudioRendererAlgorithm::EnqueueBuffer(Buffer* buffer_in) {
  DCHECK(!buffer_in->IsEndOfStream());
  audio_buffer_.Append(buffer_in);
  needs_more_data_ = false;

  if (!IsQueueFull())
    request_read_cb_.Run();
}
