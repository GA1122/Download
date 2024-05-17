gfx::PluginWindowHandle RenderViewImpl::AllocateFakePluginWindowHandle(
    bool opaque, bool root) {
  gfx::PluginWindowHandle window = NULL;
  Send(new ViewHostMsg_AllocateFakePluginWindowHandle(
      routing_id(), opaque, root, &window));
  if (window) {
    fake_plugin_window_handles_.insert(window);
  }
  return window;
}
