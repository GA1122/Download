bool ChromeContentRendererClient::OverrideCreatePlugin(
    content::RenderView* render_view,
    WebFrame* frame,
    const WebPluginParams& params,
    WebPlugin** plugin) {
  ChromeViewHostMsg_GetPluginInfo_Status status;
  webkit::WebPluginInfo plugin_info;
  std::string actual_mime_type;
  render_view->Send(new ChromeViewHostMsg_GetPluginInfo(
      render_view->GetRoutingID(), GURL(params.url),
      frame->top()->document().url(), params.mimeType.utf8(),
      &status, &plugin_info, &actual_mime_type));
  *plugin = CreatePlugin(render_view, frame, params,
                         status, plugin_info, actual_mime_type);
  return true;
}
