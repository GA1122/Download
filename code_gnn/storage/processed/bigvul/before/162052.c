void RenderProcessHostImpl::BindRouteProvider(
    mojom::RouteProviderAssociatedRequest request) {
  if (route_provider_binding_.is_bound())
    return;
  route_provider_binding_.Bind(std::move(request));
}
