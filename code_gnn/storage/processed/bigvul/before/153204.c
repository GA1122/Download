void DesktopWindowTreeHostX11::CloseNow() {
  if (xwindow_ == x11::None)
    return;

  ReleaseCapture();
  native_widget_delegate_->OnNativeWidgetDestroying();

  std::set<DesktopWindowTreeHostX11*> window_children_copy = window_children_;
  for (auto it = window_children_copy.begin(); it != window_children_copy.end();
       ++it) {
    (*it)->CloseNow();
  }
  DCHECK(window_children_.empty());

  if (window_parent_) {
    window_parent_->window_children_.erase(this);
    window_parent_ = nullptr;
  }

  desktop_native_widget_aura_->root_window_event_filter()->RemoveHandler(
      x11_non_client_event_filter_.get());
  x11_non_client_event_filter_.reset();

  DestroyCompositor();

  open_windows().remove(xwindow_);
  if (ui::PlatformEventSource::GetInstance())
    ui::PlatformEventSource::GetInstance()->RemovePlatformEventDispatcher(this);
  XDestroyWindow(xdisplay_, xwindow_);
  xwindow_ = x11::None;

  desktop_native_widget_aura_->OnHostClosed();
}
