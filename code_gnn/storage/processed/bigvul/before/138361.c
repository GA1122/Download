void ServiceManagerConnectionImpl::AddEmbeddedService(
    const std::string& name,
    const service_manager::EmbeddedServiceInfo& info) {
  context_->AddEmbeddedService(name, info);
}
