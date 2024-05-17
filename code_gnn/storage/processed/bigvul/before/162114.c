void RenderProcessHostImpl::OnAssociatedInterfaceRequest(
    const std::string& interface_name,
    mojo::ScopedInterfaceEndpointHandle handle) {
  if (associated_interfaces_ &&
      associated_interfaces_->CanBindRequest(interface_name)) {
    associated_interfaces_->BindRequest(interface_name, std::move(handle));
  } else {
    LOG(ERROR) << "Request for unknown Channel-associated interface: "
               << interface_name;
  }
}
