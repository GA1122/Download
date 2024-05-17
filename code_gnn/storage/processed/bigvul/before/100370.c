bool BrowserRenderProcessHost::FastShutdownIfPossible() {
  if (!process_.handle())
    return false;   
  if (run_renderer_in_process())
    return false;   

  if (!sudden_termination_allowed())
    return false;

  listeners_iterator iter(ListenersIterator());
  while (!iter.IsAtEnd()) {
    const RenderWidgetHost* widget =
        static_cast<const RenderWidgetHost*>(iter.GetCurrentValue());
    DCHECK(widget);
    if (widget && widget->IsRenderView()) {
      const RenderViewHost* rvh = static_cast<const RenderViewHost*>(widget);
      if (rvh->delegate()->IsExternalTabContainer())
        return false;
    }

    iter.Advance();
  }

  process_.Terminate(ResultCodes::NORMAL_EXIT);
#if defined(OS_POSIX)
  if (zygote_child_) {
#if defined(OS_LINUX)
    Singleton<ZygoteHost>()->EnsureProcessTerminated(process_.handle());
#endif   
  } else {
    ProcessWatcher::EnsureProcessGetsReaped(process_.handle());
  }
#endif   
  process_.Close();

  fast_shutdown_started_ = true;
  return true;
}
