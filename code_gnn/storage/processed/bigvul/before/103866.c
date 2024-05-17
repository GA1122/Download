WebPlugin* RenderView::createPlugin(WebFrame* frame,
                                    const WebPluginParams& params) {
  return content::GetContentClient()->renderer()->CreatePlugin(
      this, frame, params);
}
