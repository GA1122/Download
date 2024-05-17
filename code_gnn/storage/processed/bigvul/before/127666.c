void* TransportDIB::memory() const {
  DCHECK_NE(address_, kInvalidAddress);
  return address_;
}
