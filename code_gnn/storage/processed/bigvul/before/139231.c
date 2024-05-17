void RenderProcessHostImpl::UpdateProcessPriority() {
  if (!child_process_launcher_.get() || child_process_launcher_->IsStarting()) {
    is_process_backgrounded_ = false;
    return;
  }

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableRendererPriorityManagement)) {
    return;
  }

  const bool should_background =
      visible_widgets_ == 0 && audio_stream_count_ == 0 &&
      !base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableRendererBackgrounding);

#if !defined(OS_ANDROID)
  if (is_process_backgrounded_ == should_background)
    return;
#endif

  TRACE_EVENT1("renderer_host", "RenderProcessHostImpl::UpdateProcessPriority",
               "should_background", should_background);
  is_process_backgrounded_ = should_background;

#if defined(OS_WIN)
  if (GetModuleHandle(L"cbstext.dll"))
    return;
#endif   

  child_process_launcher_->SetProcessBackgrounded(should_background);

  Send(new ChildProcessMsg_SetProcessBackgrounded(should_background));
}
