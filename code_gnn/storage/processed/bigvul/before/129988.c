void ChromeBrowserMainParts::PostDestroyThreads() {
#if defined(OS_ANDROID)
  NOTREACHED();
#else
  browser_process_->PostDestroyThreads();
  ignore_result(browser_process_.release());
  browser_shutdown::ShutdownPostThreadsStop(restart_last_session_);
  master_prefs_.reset();
  process_singleton_.reset();
  device_event_log::Shutdown();

  CHECK(metrics::MetricsService::UmaMetricsProperlyShutdown());

#if defined(OS_CHROMEOS)
  chromeos::CrosSettings::Shutdown();
#endif   
#endif   
}
