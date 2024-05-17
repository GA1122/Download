void ShellContentUtilityClient::RegisterNetworkBinders(
    service_manager::BinderRegistry* registry) {
  network_service_test_helper_->RegisterNetworkBinders(registry);
}
