static inline FloatRect NormalizeRect(const FloatRect& rect) {
  return FloatRect(std::min(rect.X(), rect.MaxX()),
                   std::min(rect.Y(), rect.MaxY()),
                   std::max(rect.Width(), -rect.Width()),
                   std::max(rect.Height(), -rect.Height()));
}
