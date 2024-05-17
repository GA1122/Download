void ChromeBrowserMainParts::PostMainMessageLoopRun() {
  TRACE_EVENT0("startup", "ChromeBrowserMainParts::PostMainMessageLoopRun");
#if defined(OS_ANDROID)
  NOTREACHED();
#else
  metrics::MetricsService::SetExecutionPhase(
      metrics::MetricsService::SHUTDOWN_TIMEBOMB_ARM,
      g_browser_process->local_state());
  shutdown_watcher_->Arm(base::TimeDelta::FromSeconds(300));

  startup_watcher_->Disarm();

  process_power_collector_.reset();

  for (size_t i = 0; i < chrome_extra_parts_.size(); ++i)
    chrome_extra_parts_[i]->PostMainMessageLoopRun();

  TranslateService::Shutdown(parameters().ui_task == NULL);

  if (notify_result_ == ProcessSingleton::PROCESS_NONE)
    process_singleton_->Cleanup();

  ThreadWatcherList::StopWatchingAll();

  browser_process_->metrics_service()->Stop();

  restart_last_session_ = browser_shutdown::ShutdownPreThreadsStop();
  browser_process_->StartTearDown();
#endif   
}
