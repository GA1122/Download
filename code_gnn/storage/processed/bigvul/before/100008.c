void WebPluginImpl::WillDestroyWindow(gfx::PluginWindowHandle window) {
  DCHECK_EQ(window, window_);
  window_ = NULL;
  if (page_delegate_)
    page_delegate_->WillDestroyPluginWindow(window);
}
