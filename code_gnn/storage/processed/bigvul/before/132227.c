NOINLINE void MaybeTriggerAsanError(const GURL& url) {
  const char kCrashDomain[] = "crash";
  const char kHeapOverflow[] = "/heap-overflow";
  const char kHeapUnderflow[] = "/heap-underflow";
  const char kUseAfterFree[] = "/use-after-free";
#if defined(SYZYASAN)
  const char kCorruptHeapBlock[] = "/corrupt-heap-block";
  const char kCorruptHeap[] = "/corrupt-heap";
#endif

  if (!url.DomainIs(kCrashDomain))
    return;

  if (!url.has_path())
    return;

  std::string crash_type(url.path());
  if (crash_type == kHeapOverflow) {
    base::debug::AsanHeapOverflow();
  } else if (crash_type == kHeapUnderflow ) {
    base::debug::AsanHeapUnderflow();
  } else if (crash_type == kUseAfterFree) {
    base::debug::AsanHeapUseAfterFree();
#if defined(SYZYASAN)
  } else if (crash_type == kCorruptHeapBlock) {
    base::debug::AsanCorruptHeapBlock();
  } else if (crash_type == kCorruptHeap) {
    base::debug::AsanCorruptHeap();
#endif
  }
}
