void DrawingBuffer::FlipVertically(uint8_t* framebuffer,
                                   int width,
                                   int height) {
  std::vector<uint8_t> scanline(width * 4);
  unsigned row_bytes = width * 4;
  unsigned count = height / 2;
  for (unsigned i = 0; i < count; i++) {
    uint8_t* row_a = framebuffer + i * row_bytes;
    uint8_t* row_b = framebuffer + (height - i - 1) * row_bytes;
    memcpy(scanline.data(), row_b, row_bytes);
    memcpy(row_b, row_a, row_bytes);
    memcpy(row_a, scanline.data(), row_bytes);
  }
}
