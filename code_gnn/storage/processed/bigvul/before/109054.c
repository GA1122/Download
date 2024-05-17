void RenderViewImpl::WillDestroyPluginWindow(gfx::PluginWindowHandle window) {
#if defined(USE_X11)
  Send(new ViewHostMsg_DestroyPluginContainer(routing_id(), window));
#endif
  CleanupWindowInPluginMoves(window);
}
