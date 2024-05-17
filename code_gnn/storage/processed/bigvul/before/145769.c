BrotliResult BrotliDecompressBufferStreaming(size_t* available_in,
                                             const uint8_t** next_in,
                                             int finish,
                                             size_t* available_out,
                                             uint8_t** next_out,
                                             size_t* total_out,
                                             BrotliState* s) {
  BrotliResult result = BrotliDecompressStream(available_in, next_in,
      available_out, next_out, total_out, s);
  if (finish && result == BROTLI_RESULT_NEEDS_MORE_INPUT) {
    result = BROTLI_FAILURE();
  }
  return result;
}
