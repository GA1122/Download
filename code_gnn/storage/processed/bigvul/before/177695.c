void encrypt_buffer(uint8_t *buffer, size_t size) {
 for (size_t i = 0; i < size; ++i) {
    buffer[i] ^= secret_key[i & 15];
 }
}
