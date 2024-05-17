void ServiceManagerConnectionImpl::AddServiceRequestHandler(
    const std::string& name,
    const ServiceRequestHandler& handler) {
  context_->AddServiceRequestHandler(name, handler);
}
