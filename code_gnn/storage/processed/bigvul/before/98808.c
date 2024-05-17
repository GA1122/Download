void WebPluginDelegateProxy::PluginDestroyed() {
  if (window_)
    WillDestroyWindow();

#if defined(OS_MACOSX)
  render_view_->UnregisterPluginDelegate(this);
#endif

  if (channel_host_) {
    Send(new PluginMsg_DestroyInstance(instance_id_));

    channel_host_->RemoveRoute(instance_id_);

    channel_host_ = NULL;
  }

  if (window_script_object_) {
    window_script_object_->OnPluginDestroyed();
  }

  plugin_ = NULL;

  MessageLoop::current()->DeleteSoon(FROM_HERE, this);
}
