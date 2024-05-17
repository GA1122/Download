  explicit OpenUrlIpcInterceptor(RenderProcessHost* process)
      : BrowserMessageFilter(FrameMsgStart) {
    process->AddFilter(this);
  }
