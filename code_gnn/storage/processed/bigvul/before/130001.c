void ChromeBrowserMainParts::RecordBrowserStartupTime() {
  if (startup_metric_utils::WasNonBrowserUIDisplayed())
    return;

#if defined(OS_ANDROID)
  bool is_first_run = false;
#else
  bool is_first_run = first_run::IsChromeFirstRun();
#endif   

#if defined(OS_MACOSX) || defined(OS_WIN) || defined(OS_LINUX)
  const base::Time process_creation_time =
      base::CurrentProcessInfo::CreationTime();

  if (!is_first_run && !process_creation_time.is_null()) {
    base::TimeDelta delay = base::Time::Now() - process_creation_time;
    UMA_HISTOGRAM_LONG_TIMES_100("Startup.BrowserMessageLoopStartTime", delay);
  }
#endif   

  startup_metric_utils::OnBrowserStartupComplete(is_first_run);
}
