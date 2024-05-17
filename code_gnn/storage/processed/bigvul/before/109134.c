void RenderViewImpl::initializeHelperPluginWebFrame(
    WebKit::WebHelperPlugin* plugin) {
  plugin->initializeFrame(this);
}
