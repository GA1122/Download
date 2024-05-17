void OutOfProcessInstance::LoadUrl(const std::string& url) {
  LoadUrlInternal(url, &embed_loader_, &OutOfProcessInstance::DidOpen);
}
