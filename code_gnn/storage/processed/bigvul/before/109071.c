WebPlugin* RenderViewImpl::createPluginReplacement(
    WebFrame* frame,
    const WebPluginParams& params) {
  webkit::WebPluginInfo info;
  std::string mime_type;
  GetPluginInfo(params.url, frame->top()->document().url(),
                params.mimeType.utf8(), &info, &mime_type);
  return content::GetContentClient()->renderer()->CreatePluginReplacement(
      this, info.path);
}
