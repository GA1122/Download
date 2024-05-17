blink::WebPlugin* RenderFrameImpl::createPlugin(
    blink::WebLocalFrame* frame,
    const blink::WebPluginParams& params) {
  DCHECK_EQ(frame_, frame);
  blink::WebPlugin* plugin = NULL;
  if (GetContentClient()->renderer()->OverrideCreatePlugin(
          this, frame, params, &plugin)) {
    return plugin;
  }

  if (base::UTF16ToUTF8(base::StringPiece16(params.mimeType)) ==
      kBrowserPluginMimeType) {
    return BrowserPluginManager::Get()->CreateBrowserPlugin(
        this, GetContentClient()
                  ->renderer()
                  ->CreateBrowserPluginDelegate(this, kBrowserPluginMimeType,
                                                GURL(params.url))
                  ->GetWeakPtr());
  }

#if defined(ENABLE_PLUGINS)
  WebPluginInfo info;
  std::string mime_type;
  bool found = false;
  Send(new FrameHostMsg_GetPluginInfo(
      routing_id_, params.url, frame->top()->document().url(),
      params.mimeType.utf8(), &found, &info, &mime_type));
  if (!found)
    return NULL;

  WebPluginParams params_to_use = params;
  params_to_use.mimeType = WebString::fromUTF8(mime_type);
  return CreatePlugin(frame, info, params_to_use, nullptr  );
#else
  return NULL;
#endif   
}
