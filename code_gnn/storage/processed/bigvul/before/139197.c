void RenderProcessHostImpl::PurgeAndSuspend() {
  Send(new ChildProcessMsg_PurgeAndSuspend());
}
