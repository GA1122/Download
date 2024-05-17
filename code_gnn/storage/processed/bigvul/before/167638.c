  explicit SwapoutACKReceivedFilter(RenderProcessHost* process)
      : BrowserMessageFilter(FrameMsgStart) {
    process->AddObserver(&shutdown_observer_);
    process->AddFilter(this);
  }
