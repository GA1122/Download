RenderViewHost* RenderViewHost::From(RenderWidgetHost* rwh) {
  return static_cast<RenderViewHostImpl*>(RenderWidgetHostImpl::From(rwh));
}
