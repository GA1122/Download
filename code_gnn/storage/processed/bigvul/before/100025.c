bool WebPluginImpl::initialize(WebPluginContainer* container) {
  if (!page_delegate_)
    return false;

  std::string actual_mime_type;
  WebPluginDelegate* plugin_delegate = page_delegate_->CreatePluginDelegate(
      plugin_url_, mime_type_, &actual_mime_type);
  if (!plugin_delegate)
    return NULL;

  SetContainer(container);
  bool ok = plugin_delegate->Initialize(
      plugin_url_, arg_names_, arg_values_, this, load_manually_);
  if (!ok) {
    plugin_delegate->PluginDestroyed();
    return false;
  }

  if (!actual_mime_type.empty())
    mime_type_ = actual_mime_type;
  delegate_ = plugin_delegate;

  return true;
}
