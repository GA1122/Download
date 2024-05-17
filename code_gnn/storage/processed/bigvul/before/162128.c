void RenderProcessHostImpl::PurgeAndSuspend() {
  GetRendererInterface()->ProcessPurgeAndSuspend();
}
