bool RenderWidgetHostImpl::IsKeyboardLocked() const {
  return view_ ? view_->IsKeyboardLocked() : false;
}
