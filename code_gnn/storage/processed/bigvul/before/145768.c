BrotliResult BrotliDecompressBuffer(size_t encoded_size,
                                    const uint8_t* encoded_buffer,
                                    size_t* decoded_size,
                                    uint8_t* decoded_buffer) {
  BrotliState s;
  BrotliResult result;
  size_t total_out = 0;
  size_t available_in = encoded_size;
  const uint8_t* next_in = encoded_buffer;
  size_t available_out = *decoded_size;
  uint8_t* next_out = decoded_buffer;
  BrotliStateInit(&s);
  result = BrotliDecompressStream(&available_in, &next_in, &available_out,
      &next_out, &total_out, &s);
  *decoded_size = total_out;
  BrotliStateCleanup(&s);
  if (result != BROTLI_RESULT_SUCCESS) {
    result = BROTLI_RESULT_ERROR;
  }
  return result;
}
