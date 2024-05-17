void WebPluginDelegateProxy::OnSetWindow(gfx::PluginWindowHandle window) {
#if defined(OS_MACOSX)
  uses_shared_bitmaps_ = !window && !uses_compositor_;
#else
  uses_shared_bitmaps_ = !window;
#endif
  window_ = window;
  if (plugin_)
    plugin_->SetWindow(window);
}
