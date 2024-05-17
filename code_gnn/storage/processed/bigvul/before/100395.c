void BrowserRenderProcessHost::SetBackgrounded(bool backgrounded) {
  if (process_.handle()) {
    bool should_set_backgrounded = true;

#if defined(OS_WIN)
    should_set_backgrounded = (GetModuleHandle(L"cbstext.dll") == NULL);
#endif   

    if (should_set_backgrounded) {
      process_.SetProcessBackgrounded(backgrounded);
    }
  }

  backgrounded_ = backgrounded;
}
