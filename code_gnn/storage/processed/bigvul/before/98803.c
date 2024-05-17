void WebPluginDelegateProxy::OnSetWindow(gfx::PluginWindowHandle window) {
  uses_shared_bitmaps_ = !window;
  window_ = window;
  if (plugin_)
    plugin_->SetWindow(window);
}
