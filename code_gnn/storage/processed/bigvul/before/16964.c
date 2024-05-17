bool WebContext::IsInitialized() const {
  return context_.get() != nullptr;
}
