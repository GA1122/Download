int BrowserMainLoop::PreMainMessageLoopRun() {
#if defined(OS_ANDROID)
  ui::SetScreenAndroid();
#endif

  if (parts_) {
    TRACE_EVENT0("startup",
        "BrowserMainLoop::CreateThreads:PreMainMessageLoopRun");

    parts_->PreMainMessageLoopRun();
  }

  base::ThreadRestrictions::SetIOAllowed(false);
  base::ThreadRestrictions::DisallowWaiting();
  return result_code_;
}
