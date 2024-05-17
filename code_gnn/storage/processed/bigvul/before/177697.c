void encrypt_buffer(const uint8_t *src, uint8_t *dst, size_t size,
 ptrdiff_t offset) {
 for (size_t i = 0; i < size; ++i) {
    dst[i] = src[i] ^ test_key[(offset + i) & 15];
 }
}
