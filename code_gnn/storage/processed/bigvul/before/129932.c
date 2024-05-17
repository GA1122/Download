scoped_ptr<NativeStackSampler> NativeStackSampler::Create(
    PlatformThreadId thread_id) {
#if _WIN64
  HANDLE thread_handle = ::OpenThread(
      THREAD_GET_CONTEXT | THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION,
      FALSE,
      thread_id);

  if (thread_handle) {
    return scoped_ptr<NativeStackSampler>(new NativeStackSamplerWin(
        win::ScopedHandle(thread_handle)));
  }
#endif
  return scoped_ptr<NativeStackSampler>();
}
