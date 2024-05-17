WebPlugin* RenderViewImpl::createPlugin(WebFrame* frame,
                                        const WebPluginParams& params) {
  WebPlugin* plugin = NULL;
  if (GetContentClient()->renderer()->OverrideCreatePlugin(
          this, frame, params, &plugin)) {
    return plugin;
  }

#if defined(ENABLE_PLUGINS)
  if (UTF16ToASCII(params.mimeType) == kBrowserPluginMimeType) {
    return browser_plugin_manager()->CreateBrowserPlugin(this, frame, params);
  }

  webkit::WebPluginInfo info;
  std::string mime_type;
  bool found = GetPluginInfo(params.url, frame->top()->document().url(),
                             params.mimeType.utf8(), &info, &mime_type);
  if (!found)
    return NULL;

  WebPluginParams params_to_use = params;
  params_to_use.mimeType = WebString::fromUTF8(mime_type);
  return CreatePlugin(frame, info, params_to_use);
#else
  return NULL;
#endif   
}
