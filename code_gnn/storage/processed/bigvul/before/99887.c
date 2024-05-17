void WebPluginDelegateProxy::OnSetWindow(gfx::PluginWindowHandle window) {
  windowless_ = !window;
  window_ = window;
  if (plugin_)
    plugin_->SetWindow(window);
}
