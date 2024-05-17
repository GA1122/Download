void WebPluginDelegateProxy::OnAcceleratedPluginEnabledRendering() {
  uses_compositor_ = true;
  OnSetWindow(gfx::kNullPluginWindow);
}
