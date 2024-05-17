FreeList::getAllowedAndForbiddenCounts(Address address,
                                       size_t size,
                                       size_t& allowedCount,
                                       size_t& forbiddenCount) {
  for (size_t i = sizeof(FreeListEntry); i < size; i++) {
    if (address[i] == reuseAllowedZapValue)
      allowedCount++;
    else if (address[i] == reuseForbiddenZapValue)
      forbiddenCount++;
    else
      NOTREACHED();
  }
}
