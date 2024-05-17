bool WebPluginDelegateProxy::Initialize(const GURL& url,
    const std::vector<std::string>& arg_names,
    const std::vector<std::string>& arg_values,
    webkit_glue::WebPlugin* plugin,
    bool load_manually) {
  IPC::ChannelHandle channel_handle;
  if (!RenderThread::current()->Send(new ViewHostMsg_OpenChannelToPlugin(
          url, mime_type_, webkit_glue::GetWebKitLocale(),
          &channel_handle, &info_))) {
    return false;
  }

  if (channel_handle.name.empty()) {
    if (!info_.path.empty()) {
      render_view_->PluginCrashed(info_.path);

      return true;
    }
    return false;
  }

#if defined(OS_POSIX)
  if (channel_handle.socket.fd >= 0)
    IPC::AddChannelSocket(channel_handle.name, channel_handle.socket.fd);
#endif

  scoped_refptr<PluginChannelHost> channel_host =
      PluginChannelHost::GetPluginChannelHost(
          channel_handle.name, ChildProcess::current()->io_message_loop());
  if (!channel_host.get())
    return false;

  int instance_id;
  bool result = channel_host->Send(new PluginMsg_CreateInstance(
      mime_type_, &instance_id));
  if (!result)
    return false;

  channel_host_ = channel_host;
  instance_id_ = instance_id;

  channel_host_->AddRoute(instance_id_, this, false);

  PluginMsg_Init_Params params;
  params.containing_window = render_view_->host_window();
  params.url = url;
  params.page_url = page_url_;
  params.arg_names = arg_names;
  params.arg_values = arg_values;
  params.host_render_view_routing_id = render_view_->routing_id();
  for (size_t i = 0; i < arg_names.size(); ++i) {
    if (LowerCaseEqualsASCII(arg_names[i], "wmode") &&
        LowerCaseEqualsASCII(arg_values[i], "transparent")) {
      transparent_ = true;
    }
  }
#if defined(OS_MACOSX)
  if (!transparent_ && mime_type_ == "application/x-shockwave-flash") {
    params.arg_names.push_back("wmode");
    params.arg_values.push_back("opaque");
  }
#endif
  params.load_manually = load_manually;

  plugin_ = plugin;

  result = false;
  IPC::Message* msg = new PluginMsg_Init(instance_id_, params, &result);
  Send(msg);

  return result;
}
