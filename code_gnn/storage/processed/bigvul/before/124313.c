ExtensionSystemImpl::~ExtensionSystemImpl() {
  if (rules_registry_service_.get())
    rules_registry_service_->Shutdown();
}
