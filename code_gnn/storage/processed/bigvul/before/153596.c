void CopyRectToBuffer(const void* pixels,
                      uint32_t height,
                      uint32_t unpadded_row_size,
                      uint32_t pixels_padded_row_size,
                      void* buffer,
                      uint32_t buffer_padded_row_size) {
  if (height == 0)
    return;
  const int8_t* source = static_cast<const int8_t*>(pixels);
  int8_t* dest = static_cast<int8_t*>(buffer);
  if (pixels_padded_row_size != buffer_padded_row_size) {
    for (uint32_t ii = 0; ii < height; ++ii) {
      memcpy(dest, source, unpadded_row_size);
      dest += buffer_padded_row_size;
      source += pixels_padded_row_size;
    }
  } else {
    uint32_t size = (height - 1) * pixels_padded_row_size + unpadded_row_size;
    memcpy(dest, source, size);
  }
}
