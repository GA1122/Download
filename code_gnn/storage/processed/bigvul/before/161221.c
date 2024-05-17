static bool TruncateSize(const size_t max_size, size_t* size) {
  DCHECK_LE(static_cast<int>(max_size), kMaxBytesToSniff);

  if (*size >= max_size) {
    *size = max_size;
    return true;
  }
  return false;
}
