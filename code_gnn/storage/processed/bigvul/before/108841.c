webkit::ppapi::WebPluginImpl* RenderViewImpl::CreateBrowserPlugin(
    const IPC::ChannelHandle& channel_handle,
    int guest_process_id,
    const WebKit::WebPluginParams& params) {
  scoped_refptr<webkit::ppapi::PluginModule> pepper_module(
      pepper_delegate_.CreateBrowserPluginModule(channel_handle,
                                                 guest_process_id));
  return new webkit::ppapi::WebPluginImpl(
      pepper_module.get(), params, pepper_delegate_.AsWeakPtr());
}
