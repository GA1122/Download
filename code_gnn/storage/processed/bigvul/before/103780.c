WebPlugin* RenderView::CreatePluginNoCheck(WebFrame* frame,
                                           const WebPluginParams& params) {
  webkit::npapi::WebPluginInfo info;
  bool found;
  std::string mime_type;
  Send(new ViewHostMsg_GetPluginInfo(
      routing_id_, params.url, frame->top()->document().url(),
      params.mimeType.utf8(), &found, &info, &mime_type));
  if (!found || !webkit::npapi::IsPluginEnabled(info))
    return NULL;

  bool pepper_plugin_was_registered = false;
  scoped_refptr<webkit::ppapi::PluginModule> pepper_module(
      pepper_delegate_.CreatePepperPlugin(info.path,
                                          &pepper_plugin_was_registered));
  if (pepper_plugin_was_registered) {
    if (pepper_module)
      return CreatePepperPlugin(frame, params, info.path, pepper_module.get());
    return NULL;
  }
  return CreateNPAPIPlugin(frame, params, info.path, mime_type);
}
