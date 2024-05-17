bool BitReaderCore::ReadFlag(bool* flag) {
  if (nbits_ == 0 && !Refill(1))
    return false;

  *flag = (reg_ & (UINT64_C(1) << (kRegWidthInBits - 1))) != 0;
  reg_ <<= 1;
  nbits_--;
  bits_read_++;
  return true;
}
