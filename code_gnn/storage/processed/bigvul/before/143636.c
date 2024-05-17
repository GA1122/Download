    RenderWidgetHostImpl::GetParentNativeViewAccessible() {
  return delegate_ ? delegate_->GetParentNativeViewAccessible() : NULL;
}
