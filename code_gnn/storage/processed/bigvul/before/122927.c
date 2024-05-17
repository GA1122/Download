RenderProcessHostImpl::~RenderProcessHostImpl() {
  DCHECK(!run_renderer_in_process());
  ChildProcessSecurityPolicyImpl::GetInstance()->Remove(GetID());

  channel_.reset();
  while (!queued_messages_.empty()) {
    delete queued_messages_.front();
    queued_messages_.pop();
  }

  ClearTransportDIBCache();
  UnregisterHost(GetID());
}
