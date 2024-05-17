void RenderWidgetHostImpl::UnlockBackingStore() {
  if (view_)
    view_->UnlockCompositingSurface();
}
