void RenderView::DestroyFakePluginWindowHandle(gfx::PluginWindowHandle window) {
  if (window && fake_plugin_window_handles_.find(window) !=
      fake_plugin_window_handles_.end()) {
    Send(new ViewHostMsg_DestroyFakePluginWindowHandle(routing_id(), window));
    fake_plugin_window_handles_.erase(window);
  }
}
