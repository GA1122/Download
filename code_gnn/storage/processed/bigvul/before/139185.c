void RenderProcessHostImpl::OnChannelError() {
  ProcessDied(true  , nullptr);
}
