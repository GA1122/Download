void test_decrypt_cb(void *decrypt_state, const uint8_t *input,
 uint8_t *output, int count) {
 const size_t offset = input - reinterpret_cast<uint8_t*>(decrypt_state);
 for (int i = 0; i < count; i++) {
    output[i] = input[i] ^ secret_key[(offset + i) & 15];
 }
}
