void GpuProcessHost::DidCreateContextSuccessfully() {
#if defined(OS_ANDROID)
  hardware_accelerated_recent_crash_count_ = 0;
#endif
}
