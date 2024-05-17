void RenderFrameHostImpl::OnAssociatedInterfaceRequest(
    const std::string& interface_name,
    mojo::ScopedInterfaceEndpointHandle handle) {
  if (associated_registry_->CanBindRequest(interface_name)) {
    associated_registry_->BindRequest(interface_name, std::move(handle));
  } else {
    delegate_->OnAssociatedInterfaceRequest(this, interface_name,
                                            std::move(handle));
  }
}
