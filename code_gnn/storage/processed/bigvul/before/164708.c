  explicit PostMessageIpcInterceptor(RenderProcessHost* process)
      : BrowserMessageFilter(FrameMsgStart) {
    process->AddFilter(this);
  }
