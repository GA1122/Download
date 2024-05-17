static void discardPages(Address begin, Address end) {
  uintptr_t beginAddress =
      WTF::RoundUpToSystemPage(reinterpret_cast<uintptr_t>(begin));
  uintptr_t endAddress =
      WTF::RoundDownToSystemPage(reinterpret_cast<uintptr_t>(end));
  if (beginAddress < endAddress)
    WTF::DiscardSystemPages(reinterpret_cast<void*>(beginAddress),
                            endAddress - beginAddress);
}
