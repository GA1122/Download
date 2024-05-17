void GpuProcessHost::RecordProcessCrash() {
#if !defined(OS_ANDROID) && !defined(OS_CHROMEOS)
  constexpr int kGpuFallbackCrashCount = 3;
#else
  constexpr int kGpuFallbackCrashCount = 6;
#endif

  if (!process_launched_ || kind_ != GPU_PROCESS_KIND_SANDBOXED)
    return;

  base::subtle::NoBarrier_AtomicIncrement(&gpu_crash_count_, 1);

  int recent_crash_count = 0;
  if (mode_ == gpu::GpuMode::HARDWARE_ACCELERATED) {
    IncrementCrashCount(kForgiveGpuCrashMinutes,
                        &hardware_accelerated_recent_crash_count_);
    UMA_HISTOGRAM_EXACT_LINEAR(
        kProcessLifetimeEventsHardwareAccelerated,
        DIED_FIRST_TIME + hardware_accelerated_recent_crash_count_ - 1,
        static_cast<int>(GPU_PROCESS_LIFETIME_EVENT_MAX));
    recent_crash_count = hardware_accelerated_recent_crash_count_;
  } else if (mode_ == gpu::GpuMode::SWIFTSHADER) {
    IncrementCrashCount(kForgiveGpuCrashMinutes,
                        &swiftshader_recent_crash_count_);
    UMA_HISTOGRAM_EXACT_LINEAR(
        kProcessLifetimeEventsSwiftShader,
        DIED_FIRST_TIME + swiftshader_recent_crash_count_ - 1,
        static_cast<int>(GPU_PROCESS_LIFETIME_EVENT_MAX));
    recent_crash_count = swiftshader_recent_crash_count_;
  } else if (mode_ == gpu::GpuMode::DISPLAY_COMPOSITOR) {
    IncrementCrashCount(kForgiveDisplayCompositorCrashMinutes,
                        &display_compositor_recent_crash_count_);
    UMA_HISTOGRAM_EXACT_LINEAR(
        kProcessLifetimeEventsDisplayCompositor,
        DIED_FIRST_TIME + display_compositor_recent_crash_count_ - 1,
        static_cast<int>(GPU_PROCESS_LIFETIME_EVENT_MAX));
    recent_crash_count = display_compositor_recent_crash_count_;
  }

  int hardware_accelerated_crash_count =
      hardware_accelerated_recent_crash_count_;
  base::debug::Alias(&hardware_accelerated_crash_count);
  int swiftshader_crash_count = swiftshader_recent_crash_count_;
  base::debug::Alias(&swiftshader_crash_count);
  int display_compositor_crash_count = display_compositor_recent_crash_count_;
  base::debug::Alias(&display_compositor_crash_count);

  if (status_ == FAILURE)
    return;

  bool disable_crash_limit = base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kDisableGpuProcessCrashLimit);

  if (recent_crash_count >= kGpuFallbackCrashCount && !disable_crash_limit)
    GpuDataManagerImpl::GetInstance()->FallBackToNextGpuMode();
}
