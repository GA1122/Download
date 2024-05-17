bool DrawingBuffer::Resize(const IntSize& new_size) {
  ScopedStateRestorer scoped_state_restorer(this);
  return ResizeFramebufferInternal(new_size);
}
