T ClipToRange(T value, T lower_boundary, T upper_boundary) {
  DCHECK(lower_boundary <= upper_boundary);
  return std::max<T>(lower_boundary, std::min<T>(value, upper_boundary));
}
