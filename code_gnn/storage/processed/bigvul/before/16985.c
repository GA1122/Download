int WebContext::devtoolsPort() const {
  if (IsInitialized()) {
    return DevToolsManager::Get(context_.get())->port();
  }

  return construct_props_->devtools_port;
}
