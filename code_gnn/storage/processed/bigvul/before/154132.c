uint32_t GLES2DecoderImpl::GetAndClearBackbufferClearBitsForTest() {
  uint32_t clear_bits = backbuffer_needs_clear_bits_;
  backbuffer_needs_clear_bits_ = 0;
  return clear_bits;
}
