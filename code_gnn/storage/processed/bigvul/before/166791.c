void NEVER_INLINE FreeList::ZapFreedMemory(Address address, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (address[i] != kReuseAllowedZapValue)
      address[i] = kReuseForbiddenZapValue;
  }
}
