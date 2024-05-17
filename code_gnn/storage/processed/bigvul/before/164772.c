int BitReaderCore::PeekBitsMsbAligned(int num_bits, uint64_t* out) {
  if (nbits_ < num_bits)
    Refill(num_bits);

  *out = reg_;
  return nbits_;
}
