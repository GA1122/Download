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
#if defined(OS_MACOSX)
  if (!transparent_ && mime_type_ == "application/x-shockwave-flash") {
    bool force_opaque_mode = false;
    if (StartsWith(info_.version, L"10.0", false) ||
        StartsWith(info_.version, L"9.", false)) {
      force_opaque_mode = true;
    } else if (CommandLine::ForCurrentProcess()->HasSwitch(
                  switches::kDisableFlashCoreAnimation)) {
      force_opaque_mode = true;
    } else {
      int32 major, minor, bugfix;
      base::SysInfo::OperatingSystemVersionNumbers(&major, &minor, &bugfix);
      if (major < 10 || (major == 10 && minor < 6))
        force_opaque_mode = true;
    }

    if (force_opaque_mode) {
      params.arg_names.push_back("wmode");
      params.arg_values.push_back("opaque");
    }
  }

  params.containing_window_frame = render_view_->rootWindowRect();
  if (params.containing_window_frame.IsEmpty()) {
    params.containing_content_frame = gfx::Rect();
    params.containing_window_has_focus = false;
  } else {
    params.containing_content_frame = render_view_->windowRect();
    WebKit::WebView* webview = render_view_->webview();
    params.containing_window_has_focus = webview && webview->isActive();
  }
#endif
  params.load_manually = load_manually;

  plugin_ = plugin;

  result = false;
  IPC::Message* msg = new PluginMsg_Init(instance_id_, params, &result);
  Send(msg);

#if defined(OS_MACOSX)
  render_view_->RegisterPluginDelegate(this);
#endif

  return result;
}
