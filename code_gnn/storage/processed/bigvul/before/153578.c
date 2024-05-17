void GLES2Implementation::BufferShadowWrittenCallback(
    const ReadbackBufferShadowTracker::BufferList& buffers,
    uint64_t serial) {
  for (const auto& buffer : buffers) {
    if (buffer) {
      buffer->UpdateSerialTo(serial);
    }
  }
}
