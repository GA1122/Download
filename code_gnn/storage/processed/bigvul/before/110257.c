NaClIPCAdapter* ToAdapter(void* handle) {
  return static_cast<DescThunker*>(handle)->adapter.get();
}
