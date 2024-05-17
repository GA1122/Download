BrowserRenderProcessHost::~BrowserRenderProcessHost() {
  VLOG_IF(1, g_log_bug53991) << "~BrowserRenderProcessHost: " << this;

  ChildProcessSecurityPolicy::GetInstance()->Remove(id());

  channel_.reset();
  while (!queued_messages_.empty()) {
    delete queued_messages_.front();
    queued_messages_.pop();
  }

  ClearTransportDIBCache();
}
