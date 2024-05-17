static size_t UsedHeapSize() {
  v8::HeapStatistics heap_statistics;
  v8::Isolate::GetCurrent()->GetHeapStatistics(&heap_statistics);
  return heap_statistics.used_heap_size();
}
