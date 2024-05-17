DWORD GetNextChunkSize(size_t current_pos, size_t max_size) {
  return static_cast<DWORD>(((max_size - current_pos) <= UINT_MAX) ?
      (max_size - current_pos) : UINT_MAX);
}
