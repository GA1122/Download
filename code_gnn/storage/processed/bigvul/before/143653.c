void RenderWidgetHostImpl::LockBackingStore() {
  if (view_)
    view_->LockCompositingSurface();
}
