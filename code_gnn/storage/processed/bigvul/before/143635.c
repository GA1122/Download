    RenderWidgetHostImpl::GetOrCreateRootBrowserAccessibilityManager() {
  return delegate_ ?
      delegate_->GetOrCreateRootBrowserAccessibilityManager() : NULL;
}
