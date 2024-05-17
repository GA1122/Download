static inline IntRect normalizeRect(const IntRect& rect) {
  return IntRect(std::min(rect.x(), rect.maxX()),
                 std::min(rect.y(), rect.maxY()),
                 std::max(rect.width(), -rect.width()),
                 std::max(rect.height(), -rect.height()));
}
