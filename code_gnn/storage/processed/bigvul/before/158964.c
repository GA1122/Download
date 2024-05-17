bool IsAboveOrDirectlyLeftOf(const S& lhs, const S& rhs) {
  return lhs.y() < rhs.y() || (lhs.y() == rhs.y() && lhs.x() < rhs.x());
}
