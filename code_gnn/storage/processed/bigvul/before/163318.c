bool RenderThreadImpl::EnableStreamTextureCopy() {
  return sync_compositor_message_filter_.get();
}
