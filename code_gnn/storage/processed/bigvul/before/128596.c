void Instance::LoadUrl(const std::string& url) {
  LoadUrlInternal(url, &embed_loader_, &Instance::DidOpen);
}
