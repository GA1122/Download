void ExtensionSystemImpl::InitForOTRProfile() {
  if (extension_service()) {
    rules_registry_service_.reset(new RulesRegistryService(profile_));
    rules_registry_service_->RegisterDefaultRulesRegistries();
  }
}
