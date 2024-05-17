void ChromeRenderProcessObserver::OnGetV8HeapStats() {
  v8::HeapStatistics heap_stats;
  v8::V8::GetHeapStatistics(&heap_stats);
  RenderThread::Get()->Send(new ChromeViewHostMsg_V8HeapStats(
      heap_stats.total_heap_size(), heap_stats.used_heap_size()));
}
