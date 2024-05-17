bool BitReaderCore::SkipBitsSmall(int num_bits) {
  DCHECK_GE(num_bits, 0);
  uint64_t dummy;
  while (num_bits >= kRegWidthInBits) {
    if (!ReadBitsInternal(kRegWidthInBits, &dummy))
      return false;
    num_bits -= kRegWidthInBits;
  }
  return ReadBitsInternal(num_bits, &dummy);
}
