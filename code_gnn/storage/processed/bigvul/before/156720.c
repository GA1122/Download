  explicit SwapoutACKReceivedFilter(RenderProcessHost* process)
      : BrowserMessageFilter(FrameMsgStart) {
    process->AddFilter(this);
  }
