BitReaderCore::BitReaderCore(ByteStreamProvider* byte_stream_provider)
    : byte_stream_provider_(byte_stream_provider),
      bits_read_(0),
      nbits_(0),
      reg_(0),
      nbits_next_(0),
      reg_next_(0) {
}
