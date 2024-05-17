DrawingBuffer::ColorBuffer::ColorBuffer(
    DrawingBuffer* drawing_buffer,
    const ColorBufferParameters& parameters,
    const IntSize& size,
    GLuint texture_id,
    GLuint image_id,
    std::unique_ptr<gfx::GpuMemoryBuffer> gpu_memory_buffer)
    : drawing_buffer(drawing_buffer),
      parameters(parameters),
      size(size),
      texture_id(texture_id),
      image_id(image_id),
      gpu_memory_buffer(std::move(gpu_memory_buffer)) {
  drawing_buffer->ContextGL()->GenMailboxCHROMIUM(mailbox.name);
}
