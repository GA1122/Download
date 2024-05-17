void WebPluginDelegateProxy::WillDestroyWindow() {
  DCHECK(window_);
  plugin_->WillDestroyWindow(window_);
#if defined(OS_MACOSX)
  if (window_) {
    if (render_view_)
      render_view_->DestroyFakePluginWindowHandle(window_);
  }
#endif
  window_ = gfx::kNullPluginWindow;
}
