static void DiscardPages(Address begin, Address end) {
  uintptr_t begin_address =
      WTF::RoundUpToSystemPage(reinterpret_cast<uintptr_t>(begin));
  uintptr_t end_address =
      WTF::RoundDownToSystemPage(reinterpret_cast<uintptr_t>(end));
  if (begin_address < end_address)
    WTF::DiscardSystemPages(reinterpret_cast<void*>(begin_address),
                            end_address - begin_address);
}
