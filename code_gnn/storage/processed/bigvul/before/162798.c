size_t ContextState::GetMaxWindowRectangles() const {
  size_t size = window_rectangles_.size();
  DCHECK_EQ(0ull, size % 4);
  return size / 4;
}
