void BrowserRenderProcessHost::SetBackgrounded(bool backgrounded) {
  backgrounded_ = backgrounded;
  if (!child_process_launcher_.get() || child_process_launcher_->IsStarting())
    return;

#if defined(OS_WIN)
  if (GetModuleHandle(L"cbstext.dll"))
    return;
#endif   

  child_process_launcher_->SetProcessBackgrounded(backgrounded);
}
