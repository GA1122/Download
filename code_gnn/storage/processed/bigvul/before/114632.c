bool WebPluginDelegateProxy::Initialize(
    const GURL& url,
    const std::vector<std::string>& arg_names,
    const std::vector<std::string>& arg_values,
    webkit::npapi::WebPlugin* plugin,
    bool load_manually) {
  IPC::ChannelHandle channel_handle;
  if (!RenderThreadImpl::current()->Send(new ViewHostMsg_OpenChannelToPlugin(
          render_view_->routing_id(), url, page_url_, mime_type_,
          &channel_handle, &info_))) {
    return false;
  }

  if (channel_handle.name.empty()) {
    if (!info_.path.empty()) {
      render_view_->PluginCrashed(info_.path);
      LOG(ERROR) << "Plug-in crashed on start";

      return true;
    }
    LOG(ERROR) << "Plug-in couldn't be found";
    return false;
  }

  scoped_refptr<PluginChannelHost> channel_host(
      PluginChannelHost::GetPluginChannelHost(
          channel_handle, ChildProcess::current()->io_message_loop_proxy()));
  if (!channel_host.get()) {
    LOG(ERROR) << "Couldn't get PluginChannelHost";
    return false;
  }

  int instance_id;
  bool result = channel_host->Send(new PluginMsg_CreateInstance(
      mime_type_, &instance_id));
  if (!result) {
    LOG(ERROR) << "Couldn't send PluginMsg_CreateInstance";
    return false;
  }

  channel_host_ = channel_host;
  instance_id_ = instance_id;

  channel_host_->AddRoute(instance_id_, this, NULL);

  PluginMsg_Init_Params params;
  params.containing_window = render_view_->host_window();
  params.url = url;
  params.page_url = page_url_;
  params.arg_names = arg_names;
  params.arg_values = arg_values;
  params.host_render_view_routing_id = render_view_->routing_id();

  bool flash =
      LowerCaseEqualsASCII(mime_type_, "application/x-shockwave-flash");
  bool silverlight =
      StartsWithASCII(mime_type_, "application/x-silverlight", false);
  for (size_t i = 0; i < arg_names.size(); ++i) {
    if ((flash && LowerCaseEqualsASCII(arg_names[i], "wmode") &&
        LowerCaseEqualsASCII(arg_values[i], "transparent")) ||
        (silverlight && LowerCaseEqualsASCII(arg_names[i], "background") &&
         SilverlightColorIsTransparent(arg_values[i]))) {
      transparent_ = true;
    }
  }
  params.load_manually = load_manually;

  plugin_ = plugin;

  result = false;
  IPC::Message* msg = new PluginMsg_Init(instance_id_, params, &result);
  Send(msg);

  if (!result)
    LOG(ERROR) << "PluginMsg_Init returned false";

  render_view_->RegisterPluginDelegate(this);

  return result;
}
