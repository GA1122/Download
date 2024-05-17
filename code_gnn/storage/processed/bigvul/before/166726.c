void NEVER_INLINE FreeList::CheckFreedMemoryIsZapped(Address address,
                                                     size_t size) {
  for (size_t i = 0; i < size; i++) {
    DCHECK(address[i] == kReuseAllowedZapValue ||
           address[i] == kReuseForbiddenZapValue);
  }
}
