QString WebContext::devtoolsBindIp() const {
  if (IsInitialized()) {
    return QString::fromStdString(
        DevToolsManager::Get(context_.get())->address());
  }

  return QString::fromStdString(construct_props_->devtools_ip);
}
