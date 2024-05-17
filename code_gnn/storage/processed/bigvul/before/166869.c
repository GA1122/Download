void BlinkGCFreeHook(uint8_t* address) {
  base::SamplingHeapProfiler::RecordFree(address);
}
