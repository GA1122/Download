static inline bool ShouldUpdateHeaderAfterRevalidation(
    const AtomicString& header) {
  for (size_t i = 0; i < WTF_ARRAY_LENGTH(kHeadersToIgnoreAfterRevalidation);
       i++) {
    if (DeprecatedEqualIgnoringCase(header,
                                    kHeadersToIgnoreAfterRevalidation[i]))
      return false;
  }
  for (size_t i = 0;
       i < WTF_ARRAY_LENGTH(kHeaderPrefixesToIgnoreAfterRevalidation); i++) {
    if (header.StartsWithIgnoringASCIICase(
            kHeaderPrefixesToIgnoreAfterRevalidation[i]))
      return false;
  }
  return true;
}
