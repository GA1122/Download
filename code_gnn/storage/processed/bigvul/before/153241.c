void DesktopWindowTreeHostX11::HideImpl() {
  if (window_mapped_in_client_) {
    XWithdrawWindow(xdisplay_, xwindow_, 0);
    window_mapped_in_client_ = false;
    native_widget_delegate_->OnNativeWidgetVisibilityChanged(false);
  }
}
