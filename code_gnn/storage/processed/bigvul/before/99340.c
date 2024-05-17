void ResourceMessageFilter::OnV8HeapStats(int v8_memory_allocated,
                                          int v8_memory_used) {
  ChromeThread::PostTask(
      ChromeThread::UI, FROM_HERE,
      NewRunnableFunction(&ResourceMessageFilter::OnV8HeapStatsOnUIThread,
                          v8_memory_allocated,
                          v8_memory_used,
                          base::GetProcId(handle())));
}
