WebPlugin* RenderViewImpl::createPlugin(WebFrame* frame,
                                        const WebPluginParams& params) {
  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kEnableBrowserPlugin) &&
      UTF16ToASCII(params.mimeType) == kBrowserPluginMimeType)
    return BrowserPluginPlaceholder::Create(this, frame, params);

  WebPlugin* plugin = NULL;
  if (content::GetContentClient()->renderer()->OverrideCreatePlugin(
          this, frame, params, &plugin)) {
    return plugin;
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
}
