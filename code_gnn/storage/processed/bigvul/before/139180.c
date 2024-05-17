void RenderProcessHostImpl::OnAssociatedInterfaceRequest(
    const std::string& interface_name,
    mojo::ScopedInterfaceEndpointHandle handle) {
  if (interface_name == mojom::RouteProvider::Name_) {
    if (route_provider_binding_.is_bound())
      return;
    mojom::RouteProviderAssociatedRequest request;
    request.Bind(std::move(handle));
    route_provider_binding_.Bind(std::move(request));
  } else {
    LOG(ERROR) << "Request for unknown Channel-associated interface: "
               << interface_name;
  }
}
