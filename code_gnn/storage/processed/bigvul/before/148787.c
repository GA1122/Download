void InterstitialPageImpl::HandleKeyboardEvent(
      const NativeWebKeyboardEvent& event) {
  if (enabled())
    render_widget_host_delegate_->HandleKeyboardEvent(event);
}
