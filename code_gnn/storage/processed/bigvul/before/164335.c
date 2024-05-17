void ReportRequestedWindowState(windows::WindowState state) {
  UMA_HISTOGRAM_ENUMERATION("TabsApi.RequestedWindowState", state,
                            windows::WINDOW_STATE_LAST + 1);
}
