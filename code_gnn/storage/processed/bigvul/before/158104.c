bool LocalFrameClientImpl::IsPluginHandledExternally(
    HTMLPlugInElement& plugin_element,
    const KURL& resource_url,
    const String& suggesed_mime_type) {
  return web_frame_->Client()->IsPluginHandledExternally(
      &plugin_element, resource_url, suggesed_mime_type);
}
