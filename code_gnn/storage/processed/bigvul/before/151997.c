  JavaInterfaceProvider(
      const BindCallback& bind_callback,
      service_manager::mojom::InterfaceProviderRequest request)
      : bind_callback_(bind_callback), binding_(this, std::move(request)) {}
