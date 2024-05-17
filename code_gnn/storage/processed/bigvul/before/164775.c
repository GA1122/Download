void BitReaderCore::RefillCurrentRegister() {
  if (nbits_ == kRegWidthInBits || nbits_next_ == 0)
    return;

  reg_ |= (reg_next_ >> nbits_);

  int free_nbits = kRegWidthInBits - nbits_;
  if (free_nbits >= nbits_next_) {
    nbits_ += nbits_next_;
    reg_next_ = 0;
    nbits_next_ = 0;
    return;
  }

  nbits_ += free_nbits;
  reg_next_ <<= free_nbits;
  nbits_next_ -= free_nbits;
}
