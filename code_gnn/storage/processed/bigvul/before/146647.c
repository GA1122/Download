void DrawingBuffer::SetIsHidden(bool hidden) {
  if (is_hidden_ == hidden)
    return;
  is_hidden_ = hidden;
  if (is_hidden_)
    recycled_color_buffer_queue_.clear();
}
