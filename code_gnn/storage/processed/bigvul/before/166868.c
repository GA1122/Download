void BlinkGCAllocHook(uint8_t* address, size_t size, const char*) {
  base::SamplingHeapProfiler::RecordAlloc(address, size);
}
