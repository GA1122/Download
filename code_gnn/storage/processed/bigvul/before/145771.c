BrotliResult BrotliDecompressStreaming(BrotliInput input, BrotliOutput output,
                                       int finish, BrotliState* s) {
  const size_t kBufferSize = 65536;
  BrotliResult result;
  uint8_t* input_buffer;
  uint8_t* output_buffer;
  size_t avail_in;
  const uint8_t* next_in;
  size_t total_out;

  if (s->legacy_input_buffer == 0) {
    s->legacy_input_buffer = (uint8_t*)BROTLI_ALLOC(s, kBufferSize);
  }
  if (s->legacy_output_buffer == 0) {
    s->legacy_output_buffer = (uint8_t*)BROTLI_ALLOC(s, kBufferSize);
  }
  if (s->legacy_input_buffer == 0 || s->legacy_output_buffer == 0) {
    return BROTLI_FAILURE();
  }
  input_buffer = s->legacy_input_buffer;
  output_buffer = s->legacy_output_buffer;

   
  if (s->legacy_output_len > s->legacy_output_pos) {
    size_t to_write = s->legacy_output_len - s->legacy_output_pos;
    int num_written = BrotliWrite(
        output, output_buffer + s->legacy_output_pos, to_write);
    if (num_written < 0) {
      return BROTLI_FAILURE();
    }
    s->legacy_output_pos += (size_t)num_written;
    if ((size_t)num_written < to_write) {
      return BROTLI_RESULT_NEEDS_MORE_OUTPUT;
    }
  }
  s->legacy_output_pos = 0;

  avail_in = s->legacy_input_len - s->legacy_input_pos;
  next_in = input_buffer + s->legacy_input_pos;

  while (1) {
    size_t to_write;
    int num_written;
    size_t avail_out = kBufferSize;
    uint8_t* next_out = output_buffer;
    result = BrotliDecompressStream(&avail_in, &next_in,
        &avail_out, &next_out, &total_out, s);
    s->legacy_input_pos = (size_t)(next_out - input_buffer);
    to_write = (size_t)(next_out - output_buffer);
    num_written = BrotliWrite(output, output_buffer, to_write);
    if (num_written < 0) {
      return BROTLI_FAILURE();
    }
    if ((size_t)num_written < to_write) {
      s->legacy_output_len = to_write;
      s->legacy_output_pos = (size_t)num_written;
      return BROTLI_RESULT_NEEDS_MORE_OUTPUT;
    }
    if (result == BROTLI_RESULT_NEEDS_MORE_INPUT) {
      int num_read = BrotliRead(input, input_buffer, kBufferSize);
      if (num_read < 0 || (num_read == 0 && finish)) {
        return BROTLI_FAILURE();
      }
      if (num_read == 0) {
        s->legacy_input_len = 0;
        s->legacy_input_pos = 0;
        return BROTLI_RESULT_NEEDS_MORE_INPUT;
      }
      avail_in = (size_t)num_read;
      next_in = input_buffer;
      s->legacy_input_len = avail_in;
      s->legacy_input_pos = 0;
    } else if (result != BROTLI_RESULT_NEEDS_MORE_OUTPUT) {
       
      return result;
    }
  }
}
