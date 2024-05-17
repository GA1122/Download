void WebPluginDelegateStub::OnInit(const PluginMsg_Init_Params& params,
                                   bool* result) {
  page_url_ = params.page_url;
  child_process_logging::SetActiveURL(page_url_);

  *result = false;
  if (params.arg_names.size() != params.arg_values.size()) {
    NOTREACHED();
    return;
  }

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  FilePath path = FilePath::FromWStringHack(
      command_line.GetSwitchValue(switches::kPluginPath));


  gfx::PluginWindowHandle parent = NULL;
#if defined(OS_WIN)
  parent = gfx::NativeViewFromId(params.containing_window);
#elif defined(OS_LINUX)
#endif

  webplugin_ = new WebPluginProxy(
      channel_, instance_id_, page_url_, params.containing_window,
      params.host_render_view_routing_id);
  delegate_ = WebPluginDelegateImpl::Create(path, mime_type_, parent);
  if (delegate_) {
    webplugin_->set_delegate(delegate_);
    *result = delegate_->Initialize(params.url,
                                    params.arg_names,
                                    params.arg_values,
                                    webplugin_,
                                    params.load_manually);
#if defined(OS_MACOSX)
    delegate_->SetFocusNotifier(FocusNotifier);
#endif
  }
}
