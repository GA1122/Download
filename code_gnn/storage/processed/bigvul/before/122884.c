bool RenderProcessHostImpl::HasConnection() const {
  return channel_.get() != NULL;
}
