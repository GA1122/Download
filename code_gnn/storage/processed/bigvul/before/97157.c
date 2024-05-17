bool WebFrameLoaderClient::canShowMIMEType(const String& mime_type) const {

  if (net::IsSupportedMimeType(
          webkit_glue::CStringToStdString(mime_type.latin1())))
    return true;

  WebCore::PluginData* plugin_data = webframe_->frame()->page()->pluginData();

  return !mime_type.isEmpty() && plugin_data && plugin_data->supportsMimeType(mime_type);
}
