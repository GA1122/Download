int ServiceManagerConnectionImpl::AddConnectionFilter(
    std::unique_ptr<ConnectionFilter> filter) {
  return context_->AddConnectionFilter(std::move(filter));
}
