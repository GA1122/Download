void RenderProcessHostImpl::GetRoute(
    int32_t routing_id,
    mojom::AssociatedInterfaceProviderAssociatedRequest request) {
  DCHECK(request.is_pending());
  associated_interface_provider_bindings_.AddBinding(
      this, std::move(request), routing_id);
}
