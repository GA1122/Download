static bool StartsWith(const uint8* buffer,
                       size_t buffer_size,
                       const uint8* prefix,
                       size_t prefix_size) {
  return (prefix_size <= buffer_size &&
          memcmp(buffer, prefix, prefix_size) == 0);
}
