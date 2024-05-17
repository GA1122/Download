  explicit ProfilingClientBinder(service_manager::Connector* connector)
      : ProfilingClientBinder() {
    connector->BindInterface(content::mojom::kBrowserServiceName,
                             std::move(request_));
  }
