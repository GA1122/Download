FreeList::GetAllowedAndForbiddenCounts(Address address,
                                       size_t size,
                                       size_t& allowed_count,
                                       size_t& forbidden_count) {
  for (size_t i = sizeof(FreeListEntry); i < size; i++) {
    if (address[i] == kReuseAllowedZapValue)
      allowed_count++;
    else if (address[i] == kReuseForbiddenZapValue)
      forbidden_count++;
    else
      NOTREACHED();
  }
}
