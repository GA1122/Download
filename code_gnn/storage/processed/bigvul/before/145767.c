BrotliResult BrotliDecompress(BrotliInput input, BrotliOutput output) {
  BrotliState s;
  BrotliResult result;
  BrotliStateInit(&s);
  result = BrotliDecompressStreaming(input, output, 1, &s);
  if (result == BROTLI_RESULT_NEEDS_MORE_INPUT) {
     
    result = BROTLI_FAILURE();
  }
  BrotliStateCleanup(&s);
  return result;
}
