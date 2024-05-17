webkit::npapi::WebPluginDelegate* RenderView::CreatePluginDelegate(
    const FilePath& file_path,
    const std::string& mime_type) {
  if (!PluginChannelHost::IsListening())
    return NULL;

  bool in_process_plugin = RenderProcess::current()->UseInProcessPlugins();
  if (in_process_plugin) {
#if defined(OS_WIN)   
    return webkit::npapi::WebPluginDelegateImpl::Create(
        file_path, mime_type, gfx::NativeViewFromId(host_window_));
#else
    NOTIMPLEMENTED();
    return NULL;
#endif
  }

  return new WebPluginDelegateProxy(mime_type, AsWeakPtr());
}
