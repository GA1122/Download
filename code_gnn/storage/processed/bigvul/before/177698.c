void test_decrypt_cb(void *decrypt_state, const uint8_t *input,
 uint8_t *output, int count) {
  encrypt_buffer(input, output, count,
                 input - reinterpret_cast<uint8_t *>(decrypt_state));
}
