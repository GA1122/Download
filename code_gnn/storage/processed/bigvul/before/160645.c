void RenderFrameImpl::OnAssociatedInterfaceRequest(
    const std::string& interface_name,
    mojo::ScopedInterfaceEndpointHandle handle) {
  associated_interfaces_.BindRequest(interface_name, std::move(handle));
}
