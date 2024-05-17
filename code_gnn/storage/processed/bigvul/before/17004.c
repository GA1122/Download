void WebContext::setDevtoolsBindIp(const QString& ip) {
  if (IsInitialized()) {
    DevToolsManager::Get(context_.get())->SetAddress(ip.toStdString());
  } else {
    construct_props_->devtools_ip = ip.toStdString();
  }
}
