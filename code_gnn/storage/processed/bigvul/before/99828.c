int WebPluginProxy::GetRendererId() {
  if (channel_.get())
    return channel_->renderer_id();
  return -1;
}
