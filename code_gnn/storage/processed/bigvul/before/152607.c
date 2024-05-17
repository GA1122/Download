  void PassLastInterfaceProviderRequest(
      service_manager::mojom::InterfaceProviderRequest
          interface_provider_request) {
    last_interface_provider_request_ = std::move(interface_provider_request);
  }
