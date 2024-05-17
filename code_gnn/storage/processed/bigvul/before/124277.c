void ExtensionSystemImpl::Shared::RegisterManagementPolicyProviders() {
  DCHECK(standard_management_policy_provider_.get());
  management_policy_->RegisterProvider(
      standard_management_policy_provider_.get());
}
