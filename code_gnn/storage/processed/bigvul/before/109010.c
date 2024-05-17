void RenderViewImpl::PluginFocusChanged(bool focused, int plugin_id) {
  if (focused)
    focused_plugin_id_ = plugin_id;
  else
    focused_plugin_id_ = -1;
}
