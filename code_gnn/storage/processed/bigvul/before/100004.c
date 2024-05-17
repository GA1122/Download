void WebPluginImpl::SetWindow(gfx::PluginWindowHandle window) {
  if (window) {
    DCHECK(!windowless_);
    window_ = window;
    accepts_input_events_ = false;
    if (page_delegate_) {
      page_delegate_->CreatedPluginWindow(window);
    }
  } else {
    DCHECK(!window_);   
    windowless_ = true;
    accepts_input_events_ = true;
  }
}
