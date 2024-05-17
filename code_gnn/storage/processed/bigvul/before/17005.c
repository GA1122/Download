void WebContext::setDevtoolsEnabled(bool enabled) {
  if (IsInitialized()) {
    DevToolsManager::Get(context_.get())->SetEnabled(enabled);
  } else {
    construct_props_->devtools_enabled = enabled;
  }
}
