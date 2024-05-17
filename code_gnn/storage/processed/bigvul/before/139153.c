void RenderProcessHostImpl::GetAssociatedInterface(
    const std::string& name,
    mojom::AssociatedInterfaceAssociatedRequest request) {
  int32_t routing_id =
      associated_interface_provider_bindings_.dispatch_context();
  IPC::Listener* listener = listeners_.Lookup(routing_id);
  if (listener)
    listener->OnAssociatedInterfaceRequest(name, request.PassHandle());
}
