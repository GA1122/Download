int AudioRendererAlgorithm::QueueCapacity() {
  return audio_buffer_.forward_capacity();
}
