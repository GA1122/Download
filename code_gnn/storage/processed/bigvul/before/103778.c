WebPlugin* RenderView::CreatePepperPlugin(
    WebFrame* frame,
    const WebPluginParams& params,
    const FilePath& path,
    webkit::ppapi::PluginModule* pepper_module) {
  return new webkit::ppapi::WebPluginImpl(
      pepper_module, params, pepper_delegate_.AsWeakPtr());
}
