  DroppedInterfaceRequestLogger(
      service_manager::mojom::InterfaceProviderRequest request)
      : binding_(this) {
    binding_.Bind(std::move(request));
  }
