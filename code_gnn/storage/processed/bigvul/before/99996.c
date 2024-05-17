bool WebPluginImpl::ReinitializePluginForResponse(
    WebURLLoader* loader) {
  WebFrame* webframe = webframe_;
  if (!webframe)
    return false;

  WebView* webview = webframe->view();
  if (!webview)
    return false;

  WebPluginContainer* container_widget = container_;

  TearDownPluginInstance(loader);

  container_ = container_widget;
  webframe_ = webframe;

  std::string actual_mime_type;
  WebPluginDelegate* plugin_delegate = page_delegate_->CreatePluginDelegate(
      plugin_url_, mime_type_, &actual_mime_type);

  bool ok = plugin_delegate->Initialize(
      plugin_url_, arg_names_, arg_values_, this, load_manually_);

  if (!ok) {
    container_ = NULL;
    return false;
  }

  mime_type_ = actual_mime_type;
  delegate_ = plugin_delegate;

  container_->reportGeometry();

  container_->invalidate();
  return true;
}
