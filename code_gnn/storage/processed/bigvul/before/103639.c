WebPlugin* ChromeContentRendererClient::CreatePlugin(
      RenderView* render_view,
      WebFrame* frame,
      const WebPluginParams& original_params) {
  bool is_default_plugin;
  WebPlugin* plugin = CreatePluginImpl(render_view,
                                       frame,
                                       original_params,
                                       &is_default_plugin);
  if (!plugin || is_default_plugin)
    MissingPluginReporter::GetInstance()->ReportPluginMissing(
        original_params.mimeType.utf8(),
        original_params.url);
  return plugin;
}
