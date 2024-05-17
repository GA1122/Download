void NEVER_INLINE FreeList::zapFreedMemory(Address address, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (address[i] != reuseAllowedZapValue)
      address[i] = reuseForbiddenZapValue;
  }
}
