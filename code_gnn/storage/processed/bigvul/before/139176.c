bool RenderProcessHostImpl::IsReady() const {
  return GetHandle() && channel_connected_;
}
