void RenderViewHostImpl::WasSwappedOut() {
  StopHangMonitorTimeout();

  bool hung = has_timed_out_on_unload_;

  SetSwappedOut(true);

  if (hung) {
    base::ProcessHandle process_handle = GetProcess()->GetHandle();
    int views = 0;

    RenderProcessHost::RenderWidgetHostsIterator iter(
        GetProcess()->GetRenderWidgetHostsIterator());
    for (; !iter.IsAtEnd(); iter.Advance())
      ++views;

    if (!RenderProcessHost::run_renderer_in_process() &&
        process_handle && views <= 1) {
      if (SuddenTerminationAllowed()) {

        UMA_HISTOGRAM_PERCENTAGE(
            "BrowserRenderProcessHost.ChildKillsUnresponsive", 1);
      }
    }
  }

  Send(new ViewMsg_WasSwappedOut(GetRoutingID()));
}
