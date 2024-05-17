static bool StartsWith(const uint8_t* buffer,
                       size_t buffer_size,
                       const uint8_t* prefix,
                       size_t prefix_size) {
  return (prefix_size <= buffer_size &&
          memcmp(buffer, prefix, prefix_size) == 0);
 }
