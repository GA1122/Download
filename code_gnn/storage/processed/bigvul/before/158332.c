void RenderWidgetHostImpl::OnTextInputStateChanged(
    const TextInputState& params) {
  if (view_)
    view_->TextInputStateChanged(params);
}
