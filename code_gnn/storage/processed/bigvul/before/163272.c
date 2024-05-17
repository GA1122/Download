void BrowserMainLoop::InitializeMemoryManagementComponent() {
#if defined(OS_CHROMEOS)
  if (chromeos::switches::MemoryPressureHandlingEnabled()) {
    memory_pressure_monitor_ =
        std::make_unique<base::chromeos::MemoryPressureMonitor>(
            chromeos::switches::GetMemoryPressureThresholds());
  }
#elif defined(OS_MACOSX)
  memory_pressure_monitor_ =
      std::make_unique<base::mac::MemoryPressureMonitor>();
#elif defined(OS_WIN)
  memory_pressure_monitor_ =
      CreateWinMemoryPressureMonitor(parsed_command_line_);
#endif

  if (base::FeatureList::IsEnabled(features::kMemoryCoordinator))
    MemoryCoordinatorImpl::GetInstance()->Start();

  std::unique_ptr<SwapMetricsDriver::Delegate> delegate(
      base::WrapUnique<SwapMetricsDriver::Delegate>(
          new SwapMetricsDelegateUma()));

#if !defined(OS_FUCHSIA)
  swap_metrics_driver_ =
      SwapMetricsDriver::Create(std::move(delegate), kSwapMetricsInterval);
  if (swap_metrics_driver_)
    swap_metrics_driver_->Start();
#endif   
}
