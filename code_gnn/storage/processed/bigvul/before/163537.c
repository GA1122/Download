static size_t ChooseBufferSize(size_t callback_buffer_size) {
  size_t buffer_size =
      1 << static_cast<unsigned>(log2(4 * callback_buffer_size) + 0.5);

  if (buffer_size < 256)
    return 256;
  if (buffer_size > 16384)
    return 16384;

  return buffer_size;
}
