void RenderMessageFilter::OnOpenChannelToPepperPlugin(
    const FilePath& path,
    IPC::Message* reply_msg) {
  plugin_service_->OpenChannelToPpapiPlugin(
      path, profile_data_directory_, new OpenChannelToPpapiPluginCallback(
          this, resource_context_, reply_msg));
}
