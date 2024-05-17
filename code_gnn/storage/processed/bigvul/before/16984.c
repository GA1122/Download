bool WebContext::devtoolsEnabled() const {
  if (IsInitialized()) {
    return DevToolsManager::Get(context_.get())->enabled();
  }

  return construct_props_->devtools_enabled;
}
