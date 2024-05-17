gfx::NativeViewId RenderWidgetHostImpl::GetNativeViewId() const {
  if (view_)
    return view_->GetNativeViewId();
  return 0;
}
