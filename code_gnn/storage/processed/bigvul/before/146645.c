void DrawingBuffer::SetBufferClearNeeded(bool flag) {
  if (preserve_drawing_buffer_ == kDiscard) {
    buffer_clear_needed_ = flag;
  } else {
    DCHECK(!buffer_clear_needed_);
  }
}
