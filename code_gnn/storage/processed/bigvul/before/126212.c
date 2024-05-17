void Browser::OnWindowDidShow() {
  if (window_has_shown_)
    return;
  window_has_shown_ = true;

#if defined(OS_MACOSX) || defined(OS_WIN)
  static bool is_first_browser_window = true;
  if (is_first_browser_window &&
      !startup_metric_utils::WasNonBrowserUIDisplayed()) {
    is_first_browser_window = false;
    const base::Time* process_creation_time =
        base::CurrentProcessInfo::CreationTime();

    if (process_creation_time) {
      UMA_HISTOGRAM_LONG_TIMES(
          "Startup.BrowserWindowDisplay",
          base::Time::Now() - *process_creation_time);
    }
  }
#endif   

  if (!is_type_tabbed())
    return;

  GlobalErrorService* service =
      GlobalErrorServiceFactory::GetForProfile(profile());
  GlobalError* error = service->GetFirstGlobalErrorWithBubbleView();
  if (error)
    error->ShowBubbleView(this);
}
