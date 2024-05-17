bool DrawingBuffer::MarkContentsChanged() {
  if (contents_change_resolved_ || !contents_changed_) {
    contents_change_resolved_ = false;
    contents_changed_ = true;
    return true;
  }
  return false;
}
