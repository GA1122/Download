void WebPluginAcceleratedSurfaceProxy::EndDrawing() {
  if (!surface_)
    return;

  surface_->SwapBuffers();
  plugin_proxy_->AcceleratedFrameBuffersDidSwap(
      window_handle_, surface_->GetSurfaceId());
}
